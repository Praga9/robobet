#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <memory>
#include <thread>
#include <cstdlib>
#include <mutex>

#include "bet.hpp"
#include "menu_handler.hpp"
#include "bet_xml_parser.hpp"
#include "match.hpp"
#include "message_parser.hpp"
#include "message_listener.hpp"
#include "text_resolver.hpp"

namespace robobet
{

class Game
{
public:
  Game(std::string server_address, std::string server_port,
       std::string xml_file, int money);
  ~Game()
  {}

  void Initialize(void);

  void GameLoop(void);

  void MenuLoop(void);

  void EventLoop(std::shared_ptr<MessageListener> message_listener);

  void Notify(void);

  std::mutex bets_mutex_;

private:
  std::string server_address_;
  std::string server_port_;

  std::string xml_file_;

  std::atomic<int> user_money_;
  std::atomic<bool> is_exiting;
  std::vector<std::unique_ptr<Bet>> bets_;

  std::shared_ptr<TextResolver> text_resolver_;
  std::shared_ptr<Match> match_;
};

}
#endif
