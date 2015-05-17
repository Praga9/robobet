#include "text_resolver.hpp"

robobet::GameEventType robobet::TextResolver::StrToEventType(const std::string value)
{
  auto it = TextToEventTypeMap.find(value);

  if (it != TextToEventTypeMap.end())
    return it->second;
  else
    throw std::out_of_range("Value does not exist in map!");
}

std::string robobet::TextResolver::XMLTextToStr(const std::string value)
{
  auto it = XMLMacroMap.find(value);

  if (it != XMLMacroMap.end())
    return it->second;
  else
    throw std::out_of_range("Value does not exist in map!");
}

std::string robobet::TextResolver::EventDataToStr(std::shared_ptr<robobet::GameEvent> event)
{
  // clear the stringstream
  event_sstream.str(std::string());
  event_sstream.clear();

  return event_sstream.str();
}

void robobet::TextResolver::set_team(robobet::Team team, std::string team_name)
{
  if (team == Team::LEFT)
  {
    XMLMacroMap["TEAM_LEFT"] = team_name;
  }
  else
  {
    XMLMacroMap["TEAM_RIGHT"] = team_name;
  }
}

std::string robobet::TextResolver::get_team(robobet::Team team)
{
  if (team == Team::LEFT)
  {
    return XMLMacroMap["TEAM_LEFT"];
  }
  else
  {
    return XMLMacroMap["TEAM_RIGHT"];
  }
}
