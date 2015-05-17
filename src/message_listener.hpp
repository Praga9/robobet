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
  MessageListener(std::string server_address, std::string server_port):
    server_address_(server_address),
    server_port_(server_port),
    socket_(io_service_),
    is_shutdown_called(false)
  {
  }

  ~MessageListener()
  {}

  void Shutdown(void)
  {
    is_shutdown_called = true;
  }

  void Connect(void)
  {
    boost::asio::ip::udp::resolver resolver(io_service_);

    boost::asio::ip::udp::resolver::query query(boost::asio::ip::udp::v4(),
                                                server_address_,
                                                server_port_);

    receiver_endpoint_ = *resolver.resolve(query);

    socket_.open(boost::asio::ip::udp::v4());

    char disp_message_buffer[]  = { "(dispinit version 4)" };

    socket_.send_to(boost::asio::buffer(disp_message_buffer), receiver_endpoint_);
  }

  void Listen(std::shared_ptr<MessageParser> message_parser)
  {
    char message_buffer[128];

    for (;;)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));

      boost::asio::ip::udp::endpoint sender_endpoint;

      size_t msg_length = socket_.receive_from(boost::asio::buffer(message_buffer),
                                               sender_endpoint);

      message_buffer[msg_length - 1] = '\0';

      message_parser->Parse(message_buffer, msg_length);

      if (is_shutdown_called)
        break;
    }
  }
private:
  std::string server_address_;
  std::string server_port_;

  boost::asio::io_service io_service_;
  boost::asio::ip::udp::socket socket_;
  boost::asio::ip::udp::endpoint receiver_endpoint_;

  std::atomic<bool> is_shutdown_called;
};

}
#endif
