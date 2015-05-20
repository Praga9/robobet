#ifndef MESSAGE_PARSER_HPP
#define MESSAGE_PARSER_HPP

/*
 *  Header file - MessageParser class
 *
 *    The MessageParser class parses the data
 *    received from the server and passes it to
 *    the associated Match object.
 */


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

  // msg_buffer points to the data to be parsed
  void Parse(char *msg_buffer, int msg_length);

private:
  std::shared_ptr<Match> match_;

  // finds the nth occurence of the specified character
  // in the buffer
  // returns the position or -1
  size_t FindNthDelimiter(char *buffer, char character, int nth);
};

}

#endif
