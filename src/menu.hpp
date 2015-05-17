#ifndef MENU_HPP
#define MENU_HPP

#include <string>
#include <iostream>
#include <cstdlib>
#include <limits>

#include "version.hpp"

namespace robobet
{

class Menu
{
public:
  Menu()
  {}
  ~Menu()
  {}

  void MenuLoop()
  {
    std::string selection(" ");

    while (selection[0] != '0')
    {
      std::system("clear");

      PrintMainMenu();

      std::getline(std::cin, selection);

      switch (selection[0])
      {
        case '1': PrintBetHistory();
                  break;
        case '2': PrintAvailableBets();
                  break;
        case '3': PrintEventLoop();
                  break;
        case '4': PrintHelp();
                  break;
      }
    }
  }

private:
  void PrintMainMenu(void)
  {
    std::cout << "Tedd próbára szerencsédet a Robobettel!" << std::endl
              << "Kérlek, válassz!";

    std::cout << std::string(2, '\n');

    std::cout << "\t1\tFogadási naplóm" << std::endl
              << "\t2\tÚj fogadás"      << std::endl
              << "\t3\tÉLŐ közvetítés"      << std::endl
              << "\t4\tSegítség"        << std::string(2, '\n')
              << "\t0\tKilépés"         << std::string(2, '\n');

    std::cout << "Választás:\t";
  }

  void PrintBetHistory(void)
  {

  }

  void PrintAvailableBets(void)
  {

  }

  void PrintEventLoop(void)
  {

  }

  void PrintHelp(void)
  {
    std::system("clear");

    std::cout << "Valami segítség" << std::endl;

    std::cout << std::endl;

    std::cin.get();
  }
};

}
#endif
