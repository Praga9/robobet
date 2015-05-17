#ifndef MESSAGE_LISTENER_HPP
#define MESSAGE_LISTENER_HPP

#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <stdexcept>

#include <boost/asio.hpp>

#include "message_parser.hpp"

namespace robobet
{

class MessageListener
{
public:
  MessageListener(std::string server_address, std::string server_port);

  ~MessageListener()
  {}

  void Connect(void);

  void Receive(std::shared_ptr<MessageParser> message_parser);

private:
  std::string server_address_;
  std::string server_port_;

  boost::asio::io_service io_service_;
  boost::asio::ip::udp::socket socket_;
  boost::asio::ip::udp::endpoint receiver_endpoint_;
};

}
#endif
