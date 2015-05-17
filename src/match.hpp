#ifndef MATCH_HPP
#define MATCH_HPP

/*
 *  Header file - Match class
 *
 *    Contains all information about the match including:
 *      - events
 *      - time
 *      - teams
 *
 *    Also represents the state of the match (lates play mode, half time).
 */


#include <memory>
#include <vector>
#include <atomic>
#include <string>
#include <mutex>
#include <utility>

#include <iostream>

#include "legacy_types.hpp"
#include "game_event.hpp"

namespace robobet
{

class Match
{
public:
  Match():
    team_left_("UNDEFINED"),
    team_right_("UNDEFINED"),
    current_play_mode(PlayMode::PM_BeforeKickOff),
    game_time_(0),
    half_(1),
    is_teams_set_(false)
  {}

  ~Match()
  {}

  std::string get_team_name(Team team)
  {
    switch (team)
    {
      case Team::LEFT:
        return team_left_;
      case Team::RIGHT:
        return team_right_;
      default:
        return std::string(); // return empty string
    }
  }

  std::pair<GameTime, int> get_game_time(void)
  {
    return std::pair<GameTime, int>(game_time_, half_);
  }

  inline bool is_team_names_set(void)
  {
    return is_teams_set_;
  }

  void set_team_names(std::string left, std::string right)
  {
    if (left == "null" || right == "null")
      return;

    team_left_.assign(left);
    team_right_.assign(right);

    is_teams_set_.store(true);
  }

  void ProcessMessage(GameTime game_time, PlayMode play_mode)
  {
    game_time_ = game_time;

    if (current_play_mode != play_mode)
    {
      std::lock_guard<std::mutex> lock(events_mutex_);

      switch (play_mode)
      {
        case PM_KickIn_Left:
        case PM_KickIn_Right:
          events_.push_back(std::make_shared<GameEvent>(
            GameEvent {game_time, half_.load(),
                          GameEventType::KICKIN,
                          play_mode == PlayMode::PM_KickIn_Left ?
                                       Team::LEFT : Team::RIGHT} ));
          break;
        case PM_FreeKick_Left:
        case PM_FreeKick_Right:
          events_.push_back(std::make_shared<GameEvent>(
            GameEvent {game_time, half_.load(),
                       GameEventType::FREEKICK,
                       play_mode == PlayMode::PM_FreeKick_Left ?
                                    Team::LEFT : Team::RIGHT} ));
          break;
        case PM_CornerKick_Left:
        case PM_CornerKick_Right:
          events_.push_back(std::make_shared<GameEvent>(
            GameEvent {game_time, half_.load(),
                       GameEventType::CORNER,
                       play_mode == PlayMode::PM_CornerKick_Left ?
                                    Team::LEFT : Team::RIGHT} ));
          break;
        case PM_AfterGoal_Left:
        case PM_AfterGoal_Right:
          events_.push_back(std::make_shared<GameEvent>(
            GameEvent {game_time, half_.load(),
                       GameEventType::GOAL,
                       play_mode == PlayMode::PM_AfterGoal_Left ?
                                    Team::LEFT : Team::RIGHT} ));
          break;
        case PM_OffSide_Left:
        case PM_OffSide_Right:
          events_.push_back(std::make_shared<GameEvent>(
            GameEvent {game_time, half_.load(),
                       GameEventType::OFFSIDE,
                       play_mode == PlayMode::PM_CornerKick_Left ?
                                    Team::LEFT : Team::RIGHT} ));
          break;
        default:
          break;
      }

      current_play_mode = play_mode;
    }
  }

  std::vector<std::shared_ptr<GameEvent>> events_;
  std::mutex events_mutex_;
private:
  std::string team_left_;
  std::string team_right_;

  PlayMode current_play_mode;

  // it might be read while being increased
  std::atomic<GameTime> game_time_;
  std::atomic<int> half_;
  std::atomic<bool> is_teams_set_;
};

}


#endif
