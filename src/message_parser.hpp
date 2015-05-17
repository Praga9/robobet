#ifndef MESSAGE_PARSER_HPP
#define MESSAGE_PARSER_HPP

#include <memory>
#include <cstdio>

#include "match.hpp"
#include "legacy_types.hpp"

namespace robobet
{

class MessageParser
{
public:
  MessageParser(std::shared_ptr<Match> match):
    match_(match)
  {}
  ~MessageParser()
  {}

  void Parse(char *msg_buffer, int msg_length)
  {
    int delimiter_pos = FindNthDelimiter(msg_buffer, ' ', 7);

    char *parse_buffer = new char[delimiter_pos + 1];

    std::snprintf(parse_buffer, delimiter_pos + 1, "%s", msg_buffer);

    int game_time, play_mode;

    char team_left[64], team_right[64];

    if (std::strstr(parse_buffer, "(show") != NULL)
    {
      std::sscanf(parse_buffer, "(show %d (pm %d) (tm %s %s",
                                                     &game_time,
                                                     &play_mode,
                                                     team_left,
                                                     team_right);

      std::string team_left_str(team_left), team_right_str(team_right);

      match_->ProcessMessage(static_cast<GameTime>(game_time),
                             static_cast<PlayMode>(play_mode));

      if (!match_->is_team_names_set())
      {
        match_->set_team_names(team_left_str, team_right_str);
      }
    }
  }

private:
  std::shared_ptr<Match> match_;

  size_t FindNthDelimiter(char *buffer, char character, int nth)
  {
    char *copy = buffer;
    int occurrences = 0;
    int pos = 0;

    while(copy++ != 0)
    {
      pos++;

      if (*copy == character)
      {
        if (++occurrences == nth)
          return pos;
      }
    }

    return -1;
  }
};

}

#endif
