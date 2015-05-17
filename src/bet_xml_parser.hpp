#ifndef BET_XML_PARSER_HPP
#define BET_XML_PARSER_HPP

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/optional/optional.hpp>

#include "legacy_types.hpp"
#include "bet.hpp"
#include "bet_option.hpp"
#include "interval.hpp"
#include "handicap_bet.hpp"
#include "who_bet.hpp"
#include "when_bet.hpp"
#include "howmany_bet.hpp"
#include "more_bet.hpp"

namespace robobet
{

class BetXMLParser
{
public:
  BetXMLParser(std::string bet_file);

  ~BetXMLParser()
  {}

  // should be moved, not copied
  std::vector<std::unique_ptr<Bet>> parseFile();

private:
  std::string bet_file_;

  boost::property_tree::ptree tree;

  std::unique_ptr<Interval> parseInterval(boost::property_tree::ptree::value_type &sub_tree);

  std::unique_ptr<BetOption> parseBetOption(
      boost::property_tree::ptree::value_type &sub_tree);

  std::unique_ptr<Bet> parseHandicap(boost::property_tree::ptree::value_type &sub_tree);

  std::unique_ptr<Bet> parseWho(boost::property_tree::ptree::value_type &sub_tree);

  std::unique_ptr<Bet> parseWhen(boost::property_tree::ptree::value_type &sub_tree);

  std::unique_ptr<Bet> parseHowMany(boost::property_tree::ptree::value_type &sub_tree);

  std::unique_ptr<Bet> parseMore(boost::property_tree::ptree::value_type &sub_tree);
};

}
#endif
