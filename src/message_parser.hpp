#ifndef MESSAGE_PARSER_HPP
#define MESSAGE_PARSER_HPP

#include <memory>
#include <cstdio>
#include <cstring>

#include "match.hpp"
#include "legacy_types.hpp"

namespace robobet
{

class MessageParser
{
public:
  MessageParser(std::shared_ptr<Match> match);

  ~MessageParser()
  {}

  void Parse(char *msg_buffer, int msg_length);

private:
  std::shared_ptr<Match> match_;

  size_t FindNthDelimiter(char *buffer, char character, int nth);
};

}

#endif
