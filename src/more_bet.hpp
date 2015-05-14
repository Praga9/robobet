#ifndef MORE_BET_HPP
#define MORE_BET_HPP

/*
 *  Header file - MoreBet class
 *
 *    The MoreBet class represents a bet like this:
 *      Which team has more <EVENT> <IN A GIVEN INTERVAL>?
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

class MoreBet: public Bet
{
public:
  MoreBet(GameEventType event_type);

  ~MoreBet()
  {}

  void Notify(void);

  bool isAvailable(int option_index);

  std::string get_display_text(void);
  
private:
  GameEventType event_type_;
};

}
#endif
