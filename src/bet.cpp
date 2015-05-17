#include "bet.hpp"

int robobet::Bet::get_active_option(void)
{
  return active_option_index_;
}

bool robobet::Bet::isExpired(void)
{
  std::pair<GameTime, int> time_data =
    match_data_->get_game_time();

  return (interval_->checkIfInside(time_data.first, time_data.second) == 1);
}

void robobet::Bet::set_match(std::shared_ptr<Match> match)
{
  match_data_ = match;
}

bool robobet::Bet::isPaid(void)
{
  return is_paid_;
}

int robobet::Bet::Pay(void)
{
  if (is_paid_)
    return 0;

  if (active_option_index_ == -1)
    return 0;

  is_paid_ = true;

  return static_cast<int>((bet_money_ * bet_options_[active_option_index_]->get_odds()));
}

std::string robobet::Bet::get_active_option_text(void)
{
  std::stringstream ss;

  if (get_active_option() == -1)
    return std::string(); // returns az empty string

  std::string text(bet_options_[active_option_index_]->get_display_text());

  std::string resolved(match_data_->text_resolver_->XMLTextToStr(text));

  if (resolved != "")
    ss << resolved;
  else
    ss << text;

  ss << " (Odds: " << bet_options_[active_option_index_]->get_odds() << ")";

  return ss.str();
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

  std::cout << option_index << std::endl;

  bet_options_[option_index]->Activate();

  active_option_index_ = option_index;

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
    if (isAvailable(i))
    {
      std::string text = bet_options_[i]->get_display_text();

      std::string resolved = match_data_->text_resolver_->XMLTextToStr(text);

      ss << "\t" << (i + 1) << " - " << (resolved == "" ? text : resolved)
                    << " (" << bet_options_[i]->get_odds() << ")" << std::endl;
    }
  }

  return ss.str();
}

int robobet::Bet::get_bet_placed(void)
{
  return (active_option_index_ == -1) ? 0 : bet_money_;
}
