#include "game.hpp"

robobet::Game::Game(std::string server_address, std::string server_port,
     std::string xml_file, int money):
  server_address_(server_address),
  server_port_(server_port),
  xml_file_(xml_file),
  user_money_(money),
  is_exiting(false),
  text_resolver_(std::make_shared<TextResolver>()),
  match_(std::make_shared<Match>(text_resolver_))
{}

void robobet::Game::Initialize(void)
{
  std::unique_ptr<BetXMLParser> xml_parser(new BetXMLParser(xml_file_));

  bets_ = std::move(xml_parser->parseFile());

  for (auto it = bets_.begin(); it != bets_.end(); ++it)
    (*it)->set_match(match_);
}

void robobet::Game::GameLoop(void)
{
  auto message_listener = std::make_shared<MessageListener>(server_address_,
                                                            server_port_);

  message_listener->Connect();

  std::thread event_thread(&Game::EventLoop,
                           this,
                           message_listener);

  this->MenuLoop();

  is_exiting = true;

  event_thread.join();
}

void robobet::Game::MenuLoop(void)
{
  MenuHandler menu_handler;

  std::string selection(" ");

  int bet_placed = 0;

  while (selection[0] != '0')
  {
    std::system("clear");

    menu_handler.PrintMainMenu(user_money_);

    std::getline(std::cin, selection);

    switch (selection[0])
    {
      case '1': menu_handler.PrintBetHistory(bets_mutex_, bets_);
                break;
      case '2': bet_placed = menu_handler.PrintAvailableBets(bets_mutex_, bets_, user_money_);
                user_money_ -= bet_placed;
                break;
      case '3': menu_handler.PrintEventLoop(match_);
                break;
      case '4': menu_handler.PrintStatistics(match_);
                break;
      case '5': menu_handler.PrintHelp();
                break;
    }
  }
}

void robobet::Game::EventLoop(std::shared_ptr<MessageListener> message_listener)
{
  auto message_parser = std::make_shared<MessageParser>(match_);

  for(;;)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    message_listener->Receive(message_parser);

    bets_mutex_.lock();

    this->Notify();

    bets_mutex_.unlock();

    if ((is_exiting) || (match_->get_current_play_mode() == robobet::PlayMode::PM_TimeOver))
      break;
  }
}

void robobet::Game::Notify(void)
{
  for (auto it = bets_.begin(); it != bets_.end(); ++it)
  {
    (*it)->Notify();

    if ((*it)->isExpired() && (!(*it)->isPaid()))
    {
      user_money_ += (*it)->Pay();
    }
  }
}
