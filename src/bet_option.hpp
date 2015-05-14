#ifndef BET_OPTION_HPP
#define BET_OPTION_HPP

#include <map>
#include <string>

namespace robobet
{

enum BetOptionModifier
{
  EXACT_MATCH = 0,
  AFTER_MORE  = 1,
  BEFORE_LESS = 2,
  BETWEEN     = 3,
  OR          = 4
};

class BetOption
{
public:
  BetOption(double value, double odds, std::string display_text);
  BetOption(double value, double odds, std::string display_text,
            BetOptionModifier option_modifier);
  BetOption(double min_value, double max_value, double odds, std::string display_text,
            BetOptionModifier option_modifier);

  ~BetOption()
  {}

  void Activate(void);

  bool isMatch(double val);

  bool isInRange(double val) const;

  double get_odds(void) const;

  bool get_is_active(void) const;

  std::string get_display_text(void) const;

  static const std::map<std::string, BetOptionModifier> TextToBetOptionModifierMap;
private:
  double min_value_;
  double max_value_;
  double odds_;

  bool is_active_;

  std::string display_text_;

  BetOptionModifier option_modifier_;
};

}

#endif
