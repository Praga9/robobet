#include "message_parser.hpp"

robobet::MessageParser::MessageParser(std::shared_ptr<robobet::Match> match):
  match_(match)
{}

void robobet::MessageParser::Parse(char *msg_buffer, int msg_length)
{
  int delimiter_pos = FindNthDelimiter(msg_buffer, ' ', 7);

  char parse_buffer[delimiter_pos + 1];

  std::snprintf(parse_buffer, delimiter_pos + 1, "%s", msg_buffer);

  parse_buffer[delimiter_pos] = '\0';

  int game_time, play_mode;

  char team_left[64], team_right[64];

  if (std::strstr(parse_buffer, "(show") != NULL)
  {
    std::sscanf(parse_buffer, "(show %d (pm %d) (tm %s %s",
                                                   &game_time,
                                                   &play_mode,
                                                   team_left,
                                                   team_right);

    team_left[63]  = '\0';
    team_right[63] = '\0';

    std::string team_left_str(team_left), team_right_str(team_right);

    //std::cout << game_time << " " << play_mode << " "
    //          << team_left_str << " " << team_right_str << std::endl;

    match_->ProcessMessage(static_cast<robobet::GameTime>(game_time),
                           static_cast<robobet::PlayMode>(play_mode));

    if (!match_->is_team_names_set())
    {
      match_->set_team_names(team_left_str, team_right_str);
    }
  }
}

size_t robobet::MessageParser::FindNthDelimiter(char *buffer, char character, int nth)
{
  char *copy = buffer;
  int occurrences = 0;

  while (occurrences < nth)
  {
    copy = strchr(copy, character);

    if (copy == 0)
      return -1;

    occurrences++;
    copy++;
  }

  return copy - buffer;
}
