#ifndef HANDICAP_BET_HPP
#define HANDICAP_BET_HPP

/*
 *  Header file - HandicapBet class
 *
 *    The HandicapBet class represents a bet like this:
 *      Who will win <IN A GIVEN INTERVAL> with the <GIVEN HANDICAP>?
 *
 *    Note: The most traditional bet "Which team will win?" is a special
            HandicapBet, in which interval is OVERALL and handicap is 0:0.
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

class HandicapBet: public Bet
{
public:
  HandicapBet(int team_left, int team_right);

  ~HandicapBet()
  {}

  void Notify(void);

  bool isAvailable(int option_index);

  std::string get_display_text(void);

private:
  int team_left_;
  int team_right_;
};

}
#endif
