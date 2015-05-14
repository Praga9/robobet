#ifndef HOWMANY_BET_HPP
#define HOWMANY_BET_HPP

/*
 *  Header file - HowManyBet class
 *
 *    The HowManyBet class represents a bet like this:
 *      How many <EVENT>s will be <IN A GIVEN INTERVAL>?
 */

#include <mutex>
#include <algorithm>
#include <memory>
#include <string>
#include <sstream>

#include "legacy_types.hpp"
#include "bet.hpp"
#include "game_event.hpp"

namespace robobet
{

class HowManyBet: public Bet
{
public:
  HowManyBet(GameEventType event_type);

  ~HowManyBet()
  {}

  void Notify(void);

  bool isAvailable(int option_index);

  std::string get_display_text(void);

private:
  GameEventType event_type_;
};

}
#endif
