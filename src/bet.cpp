#include "bet.hpp"

int robobet::Bet::get_active_option(void)
{
  return active_option_index_;
}

bool robobet::Bet::isExpired(void)
{
  std::pair<GameTime, int> time_data =
    match_data_->get_game_time();

  return interval_->checkIfInside(time_data.first, time_data.second);
}

std::string robobet::Bet::get_active_option_text(void)
{
  if (get_active_option() == -1)
    return std::string(); // returns az empty string

  return bet_options_[active_option_index_]->get_display_text();
}

bool robobet::Bet::placeBet(int option_index, int bet_money)
{
  if ((get_active_option() != -1) || (!isAvailable(option_index)))
    return false;

  // we don't really want to use unsigned int for option_index,
  // so static_cast is appropiate
  if ((option_index >= static_cast<int>(bet_options_.size())) || option_index < 0)
    return false;

  bet_money_ = bet_money;

  bet_options_[option_index]->Activate();

  return true;
}

bool robobet::Bet::isWinning(void)
{
  return is_winning_;
}

void robobet::Bet::addOption(std::unique_ptr<BetOption> bet_option)
{
  bet_options_.push_back(std::move(bet_option));
}

void robobet::Bet::setInterval(std::unique_ptr<Interval> interval)
{
  interval_ = std::move(interval);
}

std::string robobet::Bet::listAvailableOptions(void)
{
  std::stringstream ss;

  int size = static_cast<int>(bet_options_.size());

  for (int i = 0; i < size; ++i)
  {
    //if (isAvailable(i))
    //{
      ss << (i + 1) << "\t - \t" << bet_options_[i]->get_display_text() << std::endl;
    //}
  }

  return ss.str();
}
