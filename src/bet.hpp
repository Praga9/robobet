#ifndef BET_HPP
#define BET_HPP

#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <iostream>

#include "bet_option.hpp"
#include "interval.hpp"
#include "match.hpp"

namespace robobet
{

class Bet
{
public:
  Bet()
  {}

  virtual ~Bet()
  {}

  // sets isWinning
  virtual void Notify(void) = 0;

  // -1 should be given as the parameter if we want to test if the whole bet
  //  is available or not.
  //
  // use other values to test just one specified option
  //
  // A bet is considered to be "available" if there is at least
  //  one option to choose and the interval criteria are satisfied.
  virtual bool isAvailable(int option_index) = 0;

  // The bet is "expired" if the given interval is over
  // when expired, we should check if it's winning or not and pay
  //
  // If the bet is not expired it means, we can still bet, if
  // there are available options
  virtual bool isExpired(void);

  virtual void set_match(std::shared_ptr<Match> match);

  // -1 indicates that there's no active option
  // otherwise returns the active option's index
  virtual int get_active_option(void);

  virtual std::string get_active_option_text(void);

  virtual std::string get_display_text(void) = 0;

  virtual bool placeBet(int option_index, int bet_money);

  virtual bool isWinning(void);

  virtual bool isPaid(void);

  virtual int Pay(void);

  virtual void addOption(std::unique_ptr<BetOption> bet_option);

  virtual void setInterval(std::unique_ptr<Interval> interval);

  virtual std::string listAvailableOptions(void);

  virtual int get_bet_placed(void);

protected:
  std::shared_ptr<Match> match_data_;
  std::unique_ptr<Interval> interval_;

  std::vector<std::unique_ptr<BetOption>> bet_options_;

  int bet_money_;

  bool is_winning_ = false;

  bool is_paid_ = false;

private:
  int active_option_index_ = -1;
};

}
#endif
