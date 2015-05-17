#include "message_listener.hpp"

robobet::MessageListener::MessageListener(std::string server_address, std::string server_port):
  server_address_(server_address),
  server_port_(server_port),
  socket_(io_service_)
{
}

void robobet::MessageListener::Connect(void)
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

void robobet::MessageListener::Receive(std::shared_ptr<robobet::MessageParser> message_parser)
{
  char message_buffer[128];

  boost::asio::ip::udp::endpoint sender_endpoint;

  size_t msg_length = socket_.receive_from(boost::asio::buffer(message_buffer),
                                           sender_endpoint);

  message_buffer[msg_length - 1] = '\0';

  message_parser->Parse(message_buffer, msg_length);
}
