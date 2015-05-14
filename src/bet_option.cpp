#include <sstream>
#include <iostream>

#include "legacy_types.hpp"
#include "bet_option.hpp"

const std::map<std::string, robobet::BetOptionModifier>
  robobet::BetOption::TextToBetOptionModifierMap =
  { {"EXACT_MATCH", robobet::BetOptionModifier::EXACT_MATCH},
    {"AFTER_MORE",  robobet::BetOptionModifier::AFTER_MORE},
    {"BEFORE_LESS", robobet::BetOptionModifier::BEFORE_LESS},
    {"BETWEEN",     robobet::BetOptionModifier::BETWEEN},
    {"OR",          robobet::BetOptionModifier::OR}           };

robobet::BetOption::BetOption(double value, double odds, std::string display_text):
  min_value_(value),
  max_value_(value),
  odds_(odds),
  is_active_(false),
  display_text_(display_text),
  option_modifier_(BetOptionModifier::EXACT_MATCH)
{}

robobet::BetOption::BetOption(double value, double odds, std::string display_text,
                              robobet::BetOptionModifier option_modifier):
  min_value_(value),
  max_value_(value),
  odds_(odds),
  is_active_(false),
  display_text_(display_text),
  option_modifier_(option_modifier)
{}

robobet::BetOption::BetOption(double min_value, double max_value, double odds, std::string display_text,
                              robobet::BetOptionModifier option_modifier):
  min_value_(min_value),
  max_value_(max_value),
  odds_(odds),
  is_active_(false),
  display_text_(display_text),
  option_modifier_(option_modifier)
{}

void robobet::BetOption::Activate(void)
{
  this->is_active_ = true;
}

bool robobet::BetOption::isMatch(double val)
{
  using namespace robobet;

  switch (option_modifier_)
  {
    case BetOptionModifier::EXACT_MATCH:
      return val == min_value_; // max_value_ is the same
    case BetOptionModifier::AFTER_MORE:
      return val > min_value_;
    case BetOptionModifier::BEFORE_LESS:
      return val < min_value_;
    case BetOptionModifier::BETWEEN:
      return ((min_value_ <= val) && (max_value_ >= val));
    case BetOptionModifier::OR:
      return ((min_value_ == val) || (max_value_ == val));
    default:
      return false;
  }
}

bool robobet::BetOption::isInRange(double val) const
{
  using namespace robobet;

  // here we make advantage of the fact that in some cases
  // min_value_ is the same as max_value_
  switch (option_modifier_)
  {
    case BetOptionModifier::EXACT_MATCH:
    case BetOptionModifier::BEFORE_LESS:
    case BetOptionModifier::BETWEEN:
    case BetOptionModifier::OR:
      return !(val > max_value_);
    case BetOptionModifier::AFTER_MORE:
        return true;
    default:
      return false;
  }
}

double robobet::BetOption::get_odds(void) const
{
  return odds_;
}

bool robobet::BetOption::get_is_active(void) const
{
  return is_active_;
}

std::string robobet::BetOption::get_display_text(void) const
{
  std::stringstream ss;

  for (auto kvp : robobet::PredefinedTextMap)
  {
    if (kvp.first == display_text_)
      return kvp.second;

    std::cout << kvp.second << std::endl;
  }

  if (display_text_ == "VALUE")
  {
    if (min_value_ == max_value_)
    {
      ss << min_value_;

      return ss.str();
    }
    else
    {
      ss << min_value_ << " - " << max_value_;

      return ss.str();
    }
  }

  return display_text_;
}
