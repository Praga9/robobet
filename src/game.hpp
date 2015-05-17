#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <memory>

#include "bet.hpp"
#include "menu.hpp"
#include "bet_xml_parser.hpp"
#include "match.hpp"
#include "message_listener.hpp"
#include "message_parser.hpp"

namespace robobet
{

class Game
{
public:
  Game(std::string server_address, std::string server_port,
       std::string xml_file, int money):
    server_address_(server_address),
    server_port_(server_port),
    xml_file_(xml_file),
    user_money_(money),
    match_(std::make_shared<Match>())
  {}
  ~Game()
  {}

  void Initialize(void)
  {
    std::unique_ptr<BetXMLParser> xml_parser(new BetXMLParser(xml_file_));

    bets_ = std::move(xml_parser->parseFile());
  }

  void GameLoop(void)
  {
    std::unique_ptr<Menu> menu(new Menu());

    std::unique_ptr<MessageListener> message_listener(
          new MessageListener(server_address_, server_port_));

    auto message_parser = std::make_shared<MessageParser>(match_);

    message_listener->Connect();

    message_listener->Listen(message_parser);

    menu->MenuLoop();
  }

private:
  std::string server_address_;
  std::string server_port_;

  std::string xml_file_;

  std::atomic<int> user_money_;
  std::vector<std::unique_ptr<Bet>> bets_;

  std::shared_ptr<Match> match_;
};

}
#endif
