#ifndef WHO_BET_HPP
#define WHO_BET_HPP

/*
 *  Header file - WhoBet class
 *
 *    The WhoBet class represents a bet like this:
 *      Who will score the <NTH> <EVENT> <IN A GIVEN INTERVAL>?
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

class WhoBet: public Bet
{
public:
  WhoBet(GameEventType event_type, int nth);

  ~WhoBet()
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
