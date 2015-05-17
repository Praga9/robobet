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
  std::vector<std::unique_ptr<Bet>> parseFile()
  {
    namespace prop_tree = boost::property_tree;

    try
    {
      // may throw an exception is fails
      //  eg.: XML file markup is invalid or file not exists etc.
      prop_tree::read_xml(bet_file_, tree);
    }
    catch (prop_tree::xml_parser::xml_parser_error &e)
    {
      std::cerr << "ERROR: " << e.what() << std::endl;

      // in case of error, we'll return an empty vector
      //
      // the caller will take care of the situation
      return std::vector<std::unique_ptr<Bet>>();
    }

    std::vector<std::unique_ptr<Bet>> bets;

    for (prop_tree::ptree::value_type &sub_tree : tree.get_child("bets"))
    {
      try
      {
        if (sub_tree.first == "handicap")
        {
          bets.push_back(std::move(parseHandicap(sub_tree)));
        }
        else if (sub_tree.first == "who")
        {
          bets.push_back(std::move(parseWho(sub_tree)));
        }
        else if (sub_tree.first == "when")
        {
          bets.push_back(std::move(parseWhen(sub_tree)));
        }
        else if (sub_tree.first == "howmany")
        {
          bets.push_back(std::move(parseHowMany(sub_tree)));
        }
        else
        {
          bets.push_back(std::move(parseMore(sub_tree)));
        }

        bets.back()->setInterval(parseInterval(sub_tree));

        for (prop_tree::ptree::value_type &bet_option : sub_tree.second.get_child("options"))
        {
          bets.back()->addOption(parseBetOption(bet_option));
        }

        //std::cout << bets.back()->get_display_text();
        //std::cin.get();
      }
      catch (std::exception &e)
      {
        std::cerr << "Exception: " << e.what() << "\n";
      }
    }

    return bets;
  }

private:
  std::string bet_file_;

  boost::property_tree::ptree tree;

  std::unique_ptr<Interval> parseInterval(boost::property_tree::ptree::value_type &sub_tree)
  {
    namespace prop_tree = boost::property_tree;

    // boost optional it only set if the requested node exists
    boost::optional<std::string> default_interval =
      sub_tree.second.get_optional<std::string>("interval.default_interval");

    // we check if the underlying std::string is valid
    if (default_interval)
    {
      DefaultInterval interval_type = Interval::TextToIntervalMap.at(*default_interval);

      return std::unique_ptr<Interval>(new Interval(interval_type));
    }
    else
    {
      int from = sub_tree.second.get<int>("interval.time_span.from");
      int to   = sub_tree.second.get<int>("interval.time_span.to");

      //std::cout << from << "\t" << to << std::endl;

      return std::unique_ptr<Interval>(new Interval(from, to));
    }
  }

  std::unique_ptr<BetOption> parseBetOption(
      boost::property_tree::ptree::value_type &sub_tree)
  {
    double odds = sub_tree.second.get<double>("odds");
    std::string display_text(sub_tree.second.get<std::string>("display_text"));

    std::string option_modifier(sub_tree.second.get<std::string>("<xmlattr>.modifier"));

    BetOptionModifier modifier_type = BetOption::TextToBetOptionModifierMap.at(option_modifier);

    if (sub_tree.second.count("value") != 0)
    {
      double value = sub_tree.second.get<double>("value");

      return std::unique_ptr<BetOption>(new BetOption(value, odds, display_text, modifier_type));
    }
    else
    {
      double min_value = sub_tree.second.get<double>("min_value");
      double max_value = sub_tree.second.get<double>("max_value");

      return std::unique_ptr<BetOption>(new BetOption(min_value, max_value,
                                                      odds, display_text, modifier_type));
    }
  }

  std::unique_ptr<Bet> parseHandicap(boost::property_tree::ptree::value_type &sub_tree)
  {
    int team_left =
      sub_tree.second.get<int>("<xmlattr>.team_left");
    int team_right =
      sub_tree.second.get<int>("<xmlattr>.team_right");

    std::unique_ptr<HandicapBet> handicap_bet(new HandicapBet(team_left, team_right));

    return std::unique_ptr<Bet>(static_cast<HandicapBet*>(handicap_bet.release()));
  }

  std::unique_ptr<Bet> parseWho(boost::property_tree::ptree::value_type &sub_tree)
  {
    std::string event(sub_tree.second.get<std::string>("<xmlattr>.event_type"));

    GameEventType event_type = TextToEventTypeMap.at(event);

    int nth(sub_tree.second.get<int>("<xmlattr>.nth"));

    std::unique_ptr<WhoBet> who_bet(new WhoBet(event_type, nth));

    return std::unique_ptr<Bet>(static_cast<WhoBet*>(who_bet.release()));
  }

  std::unique_ptr<Bet> parseWhen(boost::property_tree::ptree::value_type &sub_tree)
  {
    std::string event(sub_tree.second.get<std::string>("<xmlattr>.event_type"));

    GameEventType event_type = TextToEventTypeMap.at(event);

    int nth(sub_tree.second.get<int>("<xmlattr>.nth"));

    std::unique_ptr<WhenBet> when_bet(new WhenBet(event_type, nth));

    return std::unique_ptr<Bet>(static_cast<WhenBet*>(when_bet.release()));
  }

  std::unique_ptr<Bet> parseHowMany(boost::property_tree::ptree::value_type &sub_tree)
  {
    std::string event(sub_tree.second.get<std::string>("<xmlattr>.event_type"));

    GameEventType event_type = TextToEventTypeMap.at(event);

    std::unique_ptr<HowManyBet> howmany_bet(new HowManyBet(event_type));

    return std::unique_ptr<Bet>(static_cast<HowManyBet*>(howmany_bet.release()));
  }

  std::unique_ptr<Bet> parseMore(boost::property_tree::ptree::value_type &sub_tree)
  {
    std::string event(sub_tree.second.get<std::string>("<xmlattr>.event_type"));

    GameEventType event_type = TextToEventTypeMap.at(event);

    std::unique_ptr<MoreBet> more_bet(new MoreBet(event_type));

    return std::unique_ptr<Bet>(static_cast<MoreBet*>(more_bet.release()));
  }
};

}
#endif
