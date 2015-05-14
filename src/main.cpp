#include <iostream>
#include <string>
#include <utility>

#include <boost/program_options.hpp>

#include "bet.hpp"
#include "more_bet.hpp"
#include "howmany_bet.hpp"
#include "when_bet.hpp"
#include "who_bet.hpp"
#include "handicap_bet.hpp"
#include "interval.hpp"
#include "legacy_types.hpp"
#include "version.hpp"
#include "bet_xml_parser.hpp"

void print_version(void);

int main(int argc, char *argv[])
{
  namespace program_opts = boost::program_options;

  program_opts::options_description desc("Options");

  desc.add_options()
    ("version",    "produce version message")
    ("help,h",     "produce help message")
    ("betfile,b",  program_opts::value<std::string>()->default_value("bets.xml"), "XML file containing bet data")
    ("money,m",    program_opts::value<int>()->default_value(5000), "starting money");

  program_opts::variables_map vm;

  try
  {
    program_opts::store(
      program_opts::parse_command_line(argc, argv, desc), vm);

    program_opts::notify(vm);
  }
  catch (program_opts::error &e)
  {
    std::cerr << "ERROR: " << e.what() << std::endl << std::endl
              << desc << std::endl;

    return -1;
  }

  if (vm.count("version") != 0)
  {
    print_version();

    return 0;
  }

  if (vm.count("help") != 0)
  {
    std::cout << desc;

    return 0;
  }

  robobet::PredefinedTextMap.insert(std::pair<std::string, std::string>("TEAM_LEFT", "Ultrahekk"));

  //robobet::PredefinedTextMap["TEAM_LEFT"] = "Ultrahekk";

  std::string param_bet_file(vm["betfile"].as<std::string>());

  int param_starting_money = vm["money"].as<int>();

  robobet::BetXMLParser parser(param_bet_file);

  parser.parseFile();

  return 0;
}

void print_version(void)
{
  std::cout << "Current version: " << robobet::version << std::endl;
}
