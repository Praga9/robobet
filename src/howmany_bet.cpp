#include "howmany_bet.hpp"

robobet::HowManyBet::HowManyBet(GameEventType event_type):
  event_type_(event_type)
{}


void robobet::HowManyBet::Notify(void)
{
  int active_option = get_active_option();

  // if there's no active option, there's no need to check whether
  // we have a winner or not
  if (active_option == -1)
    return;

  std::lock_guard<std::mutex> lock(match_data_->events_mutex_);

  int count =
    std::count_if(match_data_->events_.begin(),
                  match_data_->events_.end(),
    [this](std::shared_ptr<GameEvent> game_event)
    {
      if (interval_->checkIfInside(game_event->time, game_event->half) == 0)
      {
        return game_event->event_type == event_type_;
      }

      return false;
    });

  is_winning_ = bet_options_[active_option]->isMatch(count);
}

bool robobet::HowManyBet::isAvailable(int option_index)
{
  if ((option_index >= static_cast<int>(bet_options_.size())) || option_index < 0)
    return false;

  std::lock_guard<std::mutex> lock(match_data_->events_mutex_);

  int count =
    std::count_if(match_data_->events_.begin(),
                  match_data_->events_.end(),
    [this](std::shared_ptr<GameEvent> game_event)
    {
      if (interval_->checkIfInside(game_event->time, game_event->half) == 0)
      {
        return game_event->event_type == event_type_;
      }

      return false;
    });

  return bet_options_[option_index]->isInRange(count);
}

std::string robobet::HowManyBet::get_display_text(void)
{
  std::stringstream ss;

  ss << "Hány darab lesz a következő eseményből: "
     << match_data_->text_resolver_->EventTypeToStr(event_type_);

  ss << interval_->get_display_text();

  ss << "?" << std::endl;

  return ss.str();
}
