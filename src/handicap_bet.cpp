#include "handicap_bet.hpp"

robobet::HandicapBet::HandicapBet(int team_left,
                                  int team_right):
  team_left_(team_left),
  team_right_(team_right)
{}


void robobet::HandicapBet::Notify(void)
{
  int active_option = get_active_option();

  // if there's no active option, there's no need to check whether
  // we have a winner or not
  if (active_option == -1)
    return;

  std::lock_guard<std::mutex> lock(match_data_->events_mutex_);

  int left_score =
    std::count_if(match_data_->events_.begin(),
                  match_data_->events_.end(),
                  [this](std::shared_ptr<GameEvent> game_event)
                  {
                    return (interval_->checkIfInside(game_event->time, game_event->half) == 0) &&
                           (game_event->event_type == GameEventType::GOAL) &&
                           (game_event->team == Team::LEFT);
                  });

  int right_score =
    std::count_if(match_data_->events_.begin(),
                  match_data_->events_.end(),
                  [this](std::shared_ptr<GameEvent> game_event)
                  {
                    return (interval_->checkIfInside(game_event->time, game_event->half) == 0) &&
                           (game_event->event_type == GameEventType::GOAL) &&
                           (game_event->team == Team::RIGHT);
                  });

  int result;

  // add the handicaps
  left_score  += team_left_;
  right_score += team_right_;

  result = (left_score == right_score) ?
            0 : (left_score > right_score ? -1 : 1);

  is_winning_ = bet_options_[active_option]->isMatch(result);
}

bool robobet::HandicapBet::isAvailable(int option_index)
{
  if ((option_index >= static_cast<int>(bet_options_.size())) || option_index < 0)
    return false;

  return true;
}

std::string robobet::HandicapBet::get_display_text(void)
{
  std::stringstream ss;

  ss << "Melyik csapat nyer " << team_left_ << ":" << team_right_
     << " hendikep mellett ";

  ss << interval_->get_display_text();

  ss << "?" << std::endl;

  return ss.str();
}
