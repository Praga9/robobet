#ifndef WHEN_BET_HPP
#define WHEN_BET_HPP

/*
 *  Header file - WhenBet class
 *
 *    The WhenBet class represents a bet like this:
 *      When will the <NTH> <EVENT> happen?
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

class WhenBet: public Bet
{
public:
  WhenBet(GameEventType event_type, int nth);

  ~WhenBet()
  {}

  void Notify(void);

  bool isAvailable(int option_index);

  std::string get_display_text(void);

private:
  GameEventType event_type_;
  int nth_;
};

}
#endif
