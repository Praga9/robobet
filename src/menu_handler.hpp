#ifndef MENU_HANDLER_HPP
#define MENU_HANDLER_HPP

#include <string>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <sstream>
#include <utility>
#include <thread>
#include <mutex>

#include "version.hpp"
#include "bet.hpp"
#include "match.hpp"
#include "legacy_types.hpp"

namespace robobet
{

class MenuHandler
{
public:
  MenuHandler()
  {}
  ~MenuHandler()
  {}

  void PrintMainMenu(int user_money)
  {
    std::cout << "Tedd próbára szerencsédet a Robobettel!" << std::endl
              << "Jelenleg " << user_money << " ponttal rendelkezel!" << std::endl
              << "Kérlek, válassz!";

    std::cout << std::string(2, '\n');

    std::cout << "\t1\tFogadási naplóm" << std::endl
              << "\t2\tÚj fogadás"      << std::endl
              << "\t3\tÉLŐ közvetítés"  << std::endl
              << "\t4\tStatisztika"     << std::endl
              << "\t5\tSegítség"        << std::string(2, '\n')
              << "\t0\tKilépés"         << std::string(2, '\n');

    std::cout << "Választás:\t";
  }

  void PrintBetHistory(std::mutex &bets_mutex, std::vector<std::unique_ptr<Bet>> &bets)
  {
    std::system("clear");

    std::cout << "Nyomj ENTER-t a menübe való visszatéréshez!"
              << std::string(2, '\n');

    std::cout << "Lejárt fogadások" << std::string(2, '\n');

    bets_mutex.lock();

    int available_bet_count = 0;

    for (auto it = bets.begin(); it != bets.end(); ++it)
    {
      if ((*it)->isExpired() && ((*it)->get_active_option() != -1))
      {
        available_bet_count++;

        std::cout << (*it)->get_display_text()
                  << (*it)->get_active_option_text()
                  << std::endl
                  << "Megjátszott tét: " << (*it)->get_bet_placed()
                  << ((*it)->isWinning() ? "NYERT" : "NEM NYERT")
                  << std::string(2, '\n');
      }
    }

    if (available_bet_count == 0)
    {
      std::cout << "Nincsenek elérhető fogadások!" << std::string(2, '\n');;
    }

    available_bet_count = 0;

    std::cout << "Aktív fogadások" << std::string(2, '\n');

    for (auto it = bets.begin(); it != bets.end(); ++it)
    {
      if (!(*it)->isExpired() && ((*it)->get_active_option() != -1))
      {
        available_bet_count++;

        std::cout << (*it)->get_display_text()
                  << (*it)->get_active_option_text()
                  << std::endl
                  << "Megjátszott tét: " << (*it)->get_bet_placed()
                  << std::string(2, '\n');

        std::cout << std::endl;
      }
    }

    if (available_bet_count == 0)
    {
      std::cout << "Nincsenek elérhető fogadások!" << std::string(2, '\n');;
    }

    bets_mutex.unlock();

    std::cin.get();
  }

  int PrintAvailableBets(std::mutex &bets_mutex,
                         std::vector<std::unique_ptr<Bet>> &bets,
                         int user_money)
  {
    int available_bet_count = 0;

    int chosen_bet = 0, chosen_option = 0;

    int stake = 0;

    std::system("clear");

    std::cout << "Jelenleg " << user_money << " ponttal rendelkezel!" << std::endl
              << "Írj 0-t a menübe való visszatéréshez!" << std::string(2, '\n');

    bets_mutex.lock();

    for (size_t i = 0; i < bets.size(); ++i)
    {
      if (!bets[i]->isExpired() && (bets[i]->get_active_option() == -1))
      {
        available_bet_count++;

        std::cout << "#" << i + 1 << " - "
                  << bets[i]->get_display_text()
                  << bets[i]->listAvailableOptions()
                  << std::endl;
      }
    }

    bets_mutex.unlock();

    if (available_bet_count == 0)
    {
      std::cout << "Nincsenek elérhető fogadások!";
    }

    std::cout << std::string(2, '\n');

    std::cout << "Választás: ";

    std::string input("");

    std::getline(std::cin, input);

    bets_mutex.lock();

    chosen_bet = std::stoi(input) - 1;

    if (chosen_bet < 0 || chosen_bet > static_cast<int>(bets.size()))
    {
      bets_mutex.unlock();

      return 0;
    }
    else if (!bets[chosen_bet]->isExpired() && (bets[chosen_bet]->get_active_option() == -1))
    {
      std::system("clear");

      std::cout << bets[chosen_bet]->get_display_text()
                << bets[chosen_bet]->listAvailableOptions()
                << std::endl;

      bets_mutex.unlock();

      std::cout << "Válaszd ki a megfelelő opciót: ";

      std::getline(std::cin, input);

      chosen_option = std::stoi(input) - 1;

      std::cout << "Add meg a tétet: ";

      std::getline(std::cin, input);

      stake = std::stoi(input);

      if (stake > user_money)
        return 0;

      bets_mutex.lock();

      std::cout << "lol";
      std::cin.get();

      bool success = bets[chosen_bet]->placeBet(chosen_option, stake);

      std::cout << success;
      std::cin.get();

      bets_mutex.unlock();

      return (success ? stake : 0);
    }
    else
    {
      bets_mutex.unlock();

      return 0;
    }
  }

  void DisplayEventLoop(std::shared_ptr<Match> match)
  {
    match->events_mutex_.lock();

    for (auto it = match->events_.begin(); it != match->events_.end(); ++it)
    {
      std::cout << match->text_resolver_->EventDataToStr(*it);
    }

    match->events_mutex_.unlock();

    for(;;)
    {
      if (match->is_have_new_event)
      {
        match->events_mutex_.lock();

        std::cout << match->text_resolver_->EventDataToStr(match->events_.back());

        match->is_have_new_event = false;

        match->events_mutex_.unlock();
      }

      if (!is_display_event_loop)
        break;
    }
  }

  void PrintEventLoop(std::shared_ptr<Match> match)
  {
    std::system("clear");

    match->is_have_new_event = false;

    is_display_event_loop = true;

    std::cout << "Nyomj ENTER-t a menübe való visszatéréshez!"
              << std::string(2, '\n');

    std::thread display_thread(&MenuHandler::DisplayEventLoop, this, match);

    std::cin.get();

    is_display_event_loop = false;

    display_thread.join();
  }

  void PrintStatistics(std::shared_ptr<Match> match)
  {
    std::system("clear");

    std::cout << "Nyomj ENTER-t a menübe való visszatéréshez!"
              << std::string(2, '\n');

    std::stringstream ss;

    ss << "Statisztika" << std::string(2, '\n');

    match->events_mutex_.lock();

    std::pair<GameTime, int> time = match->get_game_time();

    switch (time.second)
    {
      case 0: ss << "Kezdőrúgás előtt\t"; break;
      case 1: ss << "1. félidő\t"; break;
      case 2: ss << "2. félidő\t"; break;
      case 3: ss << "Hosszabbítás\t"; break;
      case 4: ss << "Vége a meccsnek\t"; break;
    }

    ss << time.first << std::endl;

    ss << "\t\t" << match->get_team_name(Team::LEFT)
       << "\t - \t"
       << match->get_team_name(Team::RIGHT)
       << std::string(2, '\n');

    for (auto event : TextToEventTypeMap)
    {
      int left  = match->get_event_count(Team::LEFT, event.second);
      int right = match->get_event_count(Team::RIGHT, event.second);

      ss << "\t\t  " << left << "\t"
         << match->text_resolver_->EventTypeToStr(event.second)
         << "      \t" << right << std::endl;
    }

    std::cout << ss.str();

    match->events_mutex_.unlock();

    std::cin.get();
  }

  void PrintHelp(void)
  {
    std::system("clear");

    std::cout << "Nyomj ENTER-t a menübe való visszatéréshez!"
              << std::string(2, '\n');

    std::cout << "Valami segítség" << std::endl;

    std::cout << std::endl;

    std::cin.get();
  }

private:
  std::atomic<bool>  is_display_event_loop;
};

}
#endif
