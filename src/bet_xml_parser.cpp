#include "bet_xml_parser.hpp"

robobet::BetXMLParser::BetXMLParser(std::string bet_file):
  bet_file_(bet_file)
{}

// should be moved, not copied
std::vector<std::unique_ptr<robobet::Bet> > robobet::BetXMLParser::parseFile()
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
    return std::vector<std::unique_ptr<robobet::Bet>>();
  }

  std::vector<std::unique_ptr<robobet::Bet>> bets;

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

std::unique_ptr<robobet::Interval> robobet::BetXMLParser::parseInterval(boost::property_tree::ptree::value_type &sub_tree)
{
  namespace prop_tree = boost::property_tree;

  // boost optional it only set if the requested node exists
  boost::optional<std::string> default_interval =
    sub_tree.second.get_optional<std::string>("interval.default_interval");

  // we check if the underlying std::string is valid
  if (default_interval)
  {
    robobet::DefaultInterval interval_type =
      robobet::Interval::TextToIntervalMap.at(*default_interval);

    return std::unique_ptr<robobet::Interval>(new robobet::Interval(interval_type));
  }
  else
  {
    int from = sub_tree.second.get<int>("interval.time_span.from");
    int to   = sub_tree.second.get<int>("interval.time_span.to");

    //std::cout << from << "\t" << to << std::endl;

    return std::unique_ptr<robobet::Interval>(new robobet::Interval(from, to));
  }
}

std::unique_ptr<robobet::BetOption> robobet::BetXMLParser::parseBetOption(
    boost::property_tree::ptree::value_type &sub_tree)
{
  double odds = sub_tree.second.get<double>("odds");
  std::string display_text(sub_tree.second.get<std::string>("display_text"));

  std::string option_modifier(sub_tree.second.get<std::string>("<xmlattr>.modifier"));

  robobet::BetOptionModifier modifier_type =
    robobet::BetOption::TextToBetOptionModifierMap.at(option_modifier);

  if (sub_tree.second.count("value") != 0)
  {
    double value = sub_tree.second.get<double>("value");

    return std::unique_ptr<robobet::BetOption>(new robobet::BetOption(value, odds, display_text, modifier_type));
  }
  else
  {
    double min_value = sub_tree.second.get<double>("min_value");
    double max_value = sub_tree.second.get<double>("max_value");

    return std::unique_ptr<robobet::BetOption>(new robobet::BetOption(
                                                    min_value, max_value,
                                                    odds, display_text, modifier_type));
  }
}

std::unique_ptr<robobet::Bet> robobet::BetXMLParser::parseHandicap(boost::property_tree::ptree::value_type &sub_tree)
{
  int team_left =
    sub_tree.second.get<int>("<xmlattr>.team_left");
  int team_right =
    sub_tree.second.get<int>("<xmlattr>.team_right");

  std::unique_ptr<robobet::HandicapBet> handicap_bet(new robobet::HandicapBet(team_left, team_right));

  return std::unique_ptr<robobet::Bet>(static_cast<robobet::HandicapBet*>(handicap_bet.release()));
}

std::unique_ptr<robobet::Bet> robobet::BetXMLParser::parseWho(boost::property_tree::ptree::value_type &sub_tree)
{
  std::string event(sub_tree.second.get<std::string>("<xmlattr>.event_type"));

  robobet::GameEventType event_type = robobet::TextToEventTypeMap.at(event);

  int nth(sub_tree.second.get<int>("<xmlattr>.nth"));

  std::unique_ptr<robobet::WhoBet> who_bet(new robobet::WhoBet(event_type, nth));

  return std::unique_ptr<robobet::Bet>(static_cast<robobet::WhoBet*>(who_bet.release()));
}

std::unique_ptr<robobet::Bet> robobet::BetXMLParser::parseWhen(boost::property_tree::ptree::value_type &sub_tree)
{
  std::string event(sub_tree.second.get<std::string>("<xmlattr>.event_type"));

  robobet::GameEventType event_type = robobet::TextToEventTypeMap.at(event);

  int nth(sub_tree.second.get<int>("<xmlattr>.nth"));

  std::unique_ptr<robobet::WhenBet> when_bet(new robobet::WhenBet(event_type, nth));

  return std::unique_ptr<robobet::Bet>(static_cast<robobet::WhenBet*>(when_bet.release()));
}

std::unique_ptr<robobet::Bet> robobet::BetXMLParser::parseHowMany(boost::property_tree::ptree::value_type &sub_tree)
{
  std::string event(sub_tree.second.get<std::string>("<xmlattr>.event_type"));

  robobet::GameEventType event_type = robobet::TextToEventTypeMap.at(event);

  std::unique_ptr<robobet::HowManyBet> howmany_bet(new robobet::HowManyBet(event_type));

  return std::unique_ptr<robobet::Bet>(static_cast<robobet::HowManyBet*>(howmany_bet.release()));
}

std::unique_ptr<robobet::Bet> robobet::BetXMLParser::parseMore(boost::property_tree::ptree::value_type &sub_tree)
{
  std::string event(sub_tree.second.get<std::string>("<xmlattr>.event_type"));

  robobet::GameEventType event_type = robobet::TextToEventTypeMap.at(event);

  std::unique_ptr<robobet::MoreBet> more_bet(new robobet::MoreBet(event_type));

  return std::unique_ptr<robobet::Bet>(static_cast<robobet::MoreBet*>(more_bet.release()));
}
