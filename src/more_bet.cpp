#include "more_bet.hpp"

robobet::MoreBet::MoreBet(GameEventType event_type):
  event_type_(event_type)
{}


void robobet::MoreBet::Notify(void)
{
  int active_option = get_active_option();

  // if there's no active option, there's no need to check whether
  // we have a winner or not
  if (active_option == -1)
    return;

  std::lock_guard<std::mutex> lock(match_data_->events_mutex_);

  int team_right =
    std::count_if(match_data_->events_.begin(),
                  match_data_->events_.end(),
    [this](std::shared_ptr<GameEvent> game_event)
    {
      if (interval_->checkIfInside(game_event->time, game_event->half) == 0)
      {
        if ((game_event->team == Team::RIGHT) &&
            (game_event->event_type == event_type_))
        {
          return true;
        }
      }

      return false;
    });

  int team_left =
    std::count_if(match_data_->events_.begin(),
                  match_data_->events_.end(),
    [this](std::shared_ptr<GameEvent> game_event)
    {
      if (interval_->checkIfInside(game_event->time, game_event->half) == 0)
      {
        if ((game_event->team == Team::LEFT) &&
            (game_event->event_type == event_type_))
        {
          return true;
        }
      }

      return false;
    });

  is_winning_ =
    ((team_left > team_right) && (bet_options_[active_option]->isMatch(Team::LEFT))) ||
    ((team_right > team_left) && (bet_options_[active_option]->isMatch(Team::RIGHT)));
}

bool robobet::MoreBet::isAvailable(int option_index)
{
  if ((option_index >= static_cast<int>(bet_options_.size())) || option_index < 0)
    return false;

  // this type of bet is always available since there's no such condition
  // that betting TEAM_LEFT or TEAM_RIGHT would be wrong
  // (it's not like betting that there will be 0 goals when it's 1:1)
  return true;
}

std::string robobet::MoreBet::get_display_text(void)
{
  std::stringstream ss;

  ss << "Melyik csapat szerzi a többet a következő eseményből: "
     << GameEventTypeNames[static_cast<int>(event_type_)];

  ss << interval_->get_display_text() << std::endl;

  ss << "?" << std::endl;

  ss << listAvailableOptions();

  return ss.str();
}
