#ifndef BET_OPTION_HPP
#define BET_OPTION_HPP

/*
 *  Header file - BetOption class
 *                BetOptionModifier enum
 *
 *    Each Bet object holds unique_ptrs of its options.
 *    Instantiating the BetOption class outside of a bet
 *    makes no sense, since all the data belongs to a
 *    specific bet, defined in the XML file.
 *    The BetOption is highly configurable so it fits to
 *    any type of bets using min-max value and the
 *    BetOptionModifier.
 *
 *    Using the BetOptionModifier enum one can make
 *    the bet options more flexible and the betting
 *    more exciting. It covers all the usual cases.
 */


#include <map>
#include <string>

namespace robobet
{

// VALUE, MIN_VALUE, MAX_VALUE - specified in the XML file
// value - the value the option is checked against
enum BetOptionModifier
{
  EXACT_MATCH = 0,  // the matched value must be EXACTLY the same as the VALUE
  AFTER_MORE  = 1,  // the value must be LARGER than the VALUE
  BEFORE_LESS = 2,  // the value must be SMALLER than the VALUE
  BETWEEN     = 3,  // the value must be BETWEEN MIN_VALUE AND MAX_VALUE (INCLUSIVE)
  OR          = 4   // the value must be EITHER MIN_VALUE OR MAX_VALUE
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

  // indicates that this option was chosen
  void Activate(void);

  // checks whether val satisfies the requirements
  bool isMatch(double val);

  // checks whether val is in the bettable range (the choice is not impossible)
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
