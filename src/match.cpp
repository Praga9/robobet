#include "match.hpp"

robobet::Match::Match(std::shared_ptr<TextResolver> text_resolver):
  text_resolver_(text_resolver),
  is_have_new_event(false),
  team_left_("UNDEFINED"),
  team_right_("UNDEFINED"),
  current_play_mode_(robobet::PlayMode::PM_Null),
  game_time_(0),
  half_(0),
  is_teams_set_(false)
{}

std::string robobet::Match::get_team_name(robobet::Team team)
{
  switch (team)
  {
    case robobet::Team::LEFT:
      return team_left_;
    case robobet::Team::RIGHT:
      return team_right_;
    default:
      return std::string(); // return empty string
  }
}

std::pair<robobet::GameTime, int> robobet::Match::get_game_time(void)
{
  return std::pair<GameTime, int>(game_time_, half_);
}

bool robobet::Match::is_team_names_set(void)
{
  return is_teams_set_;
}

robobet::PlayMode robobet::Match::get_current_play_mode(void)
{
  return current_play_mode_;
}

void robobet::Match::set_team_names(std::string left, std::string right)
{
  if (left == "null" || right == "null")
    return;

  team_left_.assign(left);
  team_right_.assign(right);

  text_resolver_->set_team(robobet::Team::LEFT,  team_left_);
  text_resolver_->set_team(robobet::Team::RIGHT, team_right_);

  is_teams_set_.store(true);
}

void robobet::Match::ProcessMessage(robobet::GameTime game_time, robobet::PlayMode play_mode)
{
  using namespace robobet;

  game_time_ = game_time;

  if (current_play_mode_ != play_mode)
  {
    std::lock_guard<std::mutex> lock(events_mutex_);

    switch (play_mode)
    {
      case PM_TimeOver:
        half_ = 4;
        break;
      case PM_BeforeKickOff:
        half_++;
        break;
      case PM_KickIn_Left:
      case PM_KickIn_Right:
        events_.push_back(std::make_shared<GameEvent>(
          GameEvent {game_time, half_.load(),
                        GameEventType::KICKIN,
                        play_mode == PlayMode::PM_KickIn_Left ?
                                     Team::LEFT : Team::RIGHT} ));
        is_have_new_event = true;
        break;
      case PM_FreeKick_Left:
      case PM_FreeKick_Right:
        events_.push_back(std::make_shared<GameEvent>(
          GameEvent {game_time, half_.load(),
                     GameEventType::FREEKICK,
                     play_mode == PlayMode::PM_FreeKick_Left ?
                                  Team::LEFT : Team::RIGHT} ));
        is_have_new_event = true;
        break;
      case PM_CornerKick_Left:
      case PM_CornerKick_Right:
        events_.push_back(std::make_shared<GameEvent>(
          GameEvent {game_time, half_.load(),
                     GameEventType::CORNER,
                     play_mode == PlayMode::PM_CornerKick_Left ?
                                  Team::LEFT : Team::RIGHT} ));
        is_have_new_event = true;
        break;
      case PM_AfterGoal_Left:
      case PM_AfterGoal_Right:
        events_.push_back(std::make_shared<GameEvent>(
          GameEvent {game_time, half_.load(),
                     GameEventType::GOAL,
                     play_mode == PlayMode::PM_AfterGoal_Left ?
                                  Team::LEFT : Team::RIGHT} ));
        is_have_new_event = true;
        break;
      case PM_OffSide_Left:
      case PM_OffSide_Right:
        events_.push_back(std::make_shared<GameEvent>(
          GameEvent {game_time, half_.load(),
                     GameEventType::OFFSIDE,
                     play_mode == PlayMode::PM_CornerKick_Left ?
                                  Team::LEFT : Team::RIGHT} ));
        is_have_new_event = true;
        break;
      default:
        break;
    }

    current_play_mode_ = play_mode;
  }
}

int robobet::Match::get_event_count(robobet::Team team, robobet::GameEventType event_type)
{
  int count =
    std::count_if(events_.begin(),
                  events_.end(),
    [team, event_type](std::shared_ptr<GameEvent> game_event)
    {
      return (game_event->event_type == event_type) &&
             (game_event->team == team);
    });

  return count;
}
