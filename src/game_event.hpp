#ifndef GAME_EVENT_HPP
#define GAME_EVENT_HPP

#include "legacy_types.hpp"

namespace robobet
{

struct GameEvent
{
  // when?
  GameTime time;
  int half; // 1 - 1st 2 - 2nd, 3 - additional time
  // what?
  GameEventType event_type;
  // who?
  Team team;

};

}
#endif
