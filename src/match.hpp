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
#include <algorithm>

#include <iostream>

#include "legacy_types.hpp"
#include "game_event.hpp"
#include "text_resolver.hpp"

namespace robobet
{

class Match
{
public:
  Match(std::shared_ptr<TextResolver> text_resolver);

  ~Match()
  {}

  std::string get_team_name(Team team);

  std::pair<GameTime, int> get_game_time(void);

  bool is_team_names_set(void);

  PlayMode get_current_play_mode(void);

  void set_team_names(std::string left, std::string right);

  void ProcessMessage(GameTime game_time, PlayMode play_mode);

  int get_event_count(Team team, GameEventType event_type);

  std::vector<std::shared_ptr<GameEvent>> events_;
  std::mutex events_mutex_;

  std::shared_ptr<TextResolver> text_resolver_;

  std::atomic<bool> is_have_new_event;
private:
  std::string team_left_;
  std::string team_right_;

  PlayMode current_play_mode_;

  // it might be read while being increased
  std::atomic<GameTime> game_time_;
  std::atomic<int> half_;
  std::atomic<bool> is_teams_set_;
};

}


#endif
