#ifndef MATCH_DATA_HPP
#define MATCH_DATA_HPP

/*
 *  Header file - MatchData class
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

#include "legacy_types.hpp"
#include "game_event.hpp"

namespace robobet
{

class MatchData
{
public:
  MatchData();

  ~MatchData()
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

  void AdvanceGameTime(void)
  {
    game_time_++;
  }

  std::pair<GameTime, int> get_game_time(void)
  {
    return std::pair<GameTime, int>(game_time_, half_);
  }

  std::vector<std::shared_ptr<GameEvent>> events_;
  std::mutex events_mutex_;
private:
  std::string team_left_;
  std::string team_right_;

  // it might be read while being increased
  std::atomic<GameTime> game_time_;
  std::atomic<int> half_;
};

}


#endif
