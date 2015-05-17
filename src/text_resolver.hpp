#ifndef TEXT_RESOLVER_HPP
#define TEXT_RESOLVER_HPP

#include <string>
#include <map>
#include <algorithm>
#include <memory>
#include <sstream>
#include <stdexcept>

#include "legacy_types.hpp"
#include "game_event.hpp"

namespace robobet
{

class TextResolver
{
public:
  TextResolver()
  {}
  ~TextResolver()
  {}

  GameEventType StrToEventType(const std::string value);

  std::string XMLTextToStr(const std::string value);

  std::string EventDataToStr(std::shared_ptr<GameEvent> event);

  void set_team(Team team, std::string team_name);

  std::string get_team(Team team);

private:
  std::stringstream event_sstream;

  const std::map<std::string, GameEventType>
    TextToEventTypeMap =
    { {"KICKIN",    GameEventType::KICKIN},
      {"FREEKICK",  GameEventType::FREEKICK},
      {"CORNER",    GameEventType::CORNER},
      {"GOAL",      GameEventType::GOAL},
      {"OFFSIDE",   GameEventType::OFFSIDE}   };

  std::map<std::string, std::string>
    XMLMacroMap =
    { {"FIRST_HALF",   "Első félidő"},
      {"SECOND_HALF",  "Második félidő"},
      {"DRAW",         "Döntetlen"},
      {"TEAM_LEFT",    "UNDEFINED"},
      {"TEAM_RIGHT",   "UNDEFINED"},      };
};

}
#endif
