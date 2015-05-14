#ifndef LEGACY_TYPES_HPP
#define LEGACY_TYPES_HPP

#include <map>
#include <string>

namespace robobet
{
  using GameTime = short;

  enum GameEventType
  {
    KICKIN    = 0,
    FREEKICK  = 1,
    CORNER    = 2,
    GOAL      = 3,
    OFFSIDE   = 4
  };

  static const std::map<std::string, GameEventType>
    TextToEventTypeMap =
    { {"KICKIN",    GameEventType::KICKIN},
      {"FREEKICK",  GameEventType::FREEKICK},
      {"CORNER",    GameEventType::CORNER},
      {"GOAL",      GameEventType::GOAL},
      {"OFFSIDE",   GameEventType::OFFSIDE}   };

  static std::map<std::string, std::string>
    PredefinedTextMap =
    { {"FIRST_HALF",   "Első félidő"},
      {"SECOND_HALF",  "Második félidő"},
      {"DRAW",         "Döntetlen"},
      {"TEAM_LEFT",    "UNDEFINED"}      };

  static const std::string GameEventTypeNames[] = {
    "bedobás",      // KICKIN
    "szabadrúgás",  // FREEKICK
    "szöglet",      // CORNER
    "gól",          // GOAL
    "les"           // OFFSIDE
  };

  enum Team
  {
    LEFT  = 1,
    RIGHT = -1
  };

  enum PlayMode
  {
    PM_Null,
    PM_BeforeKickOff,
    PM_TimeOver,
    PM_PlayOn,
    PM_KickOff_Left,
    PM_KickOff_Right,
    PM_KickIn_Left,
    PM_KickIn_Right,
    PM_FreeKick_Left,
    PM_FreeKick_Right,
    PM_CornerKick_Left,
    PM_CornerKick_Right,
    PM_GoalKick_Left,
    PM_GoalKick_Right,
    PM_AfterGoal_Left,
    PM_AfterGoal_Right,
    PM_Drop_Ball,
    PM_OffSide_Left,
    PM_OffSide_Right,
    PM_MAX
  };
}

#endif
