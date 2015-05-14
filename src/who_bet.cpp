#include "who_bet.hpp"

robobet::WhoBet::WhoBet(GameEventType event_type, int nth):
  event_type_(event_type),
  nth_(nth)
{}


void robobet::WhoBet::Notify(void)
{
  int active_option = get_active_option();

  // if there's no active option, there's no need to check whether
  // we have a winner or not
  if (active_option == -1)
    return;

  std::lock_guard<std::mutex> lock(match_data_->events_mutex_);

  int count = 0;

  auto it = match_data_->events_.begin();

  auto predicate = [this](std::shared_ptr<GameEvent> game_event)
  {
    return (game_event->event_type == event_type_) &&
           (interval_->checkIfInside(game_event->time, game_event->half) == 0);
  };

  while (count < nth_)
  {
    it = std::find_if(it, match_data_->events_.end(), predicate);

    if (it != match_data_->events_.end())
      count++;
    else
      break;
  }

  if (count < nth_)
    is_winning_ = false;

  is_winning_ = bet_options_[active_option]->isMatch((*it)->team);
}

bool robobet::WhoBet::isAvailable(int option_index)
{
  if ((option_index >= static_cast<int>(bet_options_.size())) || option_index < 0)
    return false;

  std::lock_guard<std::mutex> lock(match_data_->events_mutex_);

  int count =
    std::count_if(match_data_->events_.begin(),
                  match_data_->events_.end(),
    [this](std::shared_ptr<GameEvent> game_event)
    {
      return (game_event->event_type == event_type_) &&
             (interval_->checkIfInside(game_event->time, game_event->half) == 0);

      return false;
    });

  return bet_options_[option_index]->isInRange(count);
}

std::string robobet::WhoBet::get_display_text(void)
{
  std::stringstream ss;

  ss << "Melyik csapat szerzi az " << nth_ << ".-t a következő eseményből: "
     << GameEventTypeNames[static_cast<int>(event_type_)];

  // no interval needed

  //ss << interval_->get_display_text();

  ss << "?" << std::endl;

  ss << listAvailableOptions();

  return ss.str();
}
