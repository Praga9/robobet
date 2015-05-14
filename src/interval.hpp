#ifndef INTERVAL_HPP
#define INTERVAL_HPP

/*
 *  Header file - Interval class
 *                DefaultInterval enum
 *
 *    The Interval class represents a time span. A bet is "valid"
 *    between the Interval it's associated to.
 *
 *    The DefaultInterval enum makes it possible to easily define
 *    bets with the traditional time span: first half, second half
 *    or the overall game time.
 *    IMPORTANT: to define a bet valid only in the additional time,
 *               one must use the ADDITIONAL_TIME value.
 */

#include <stdexcept>
#include <string>
#include <sstream>
#include <map>

#include "legacy_types.hpp"

namespace robobet
{

enum DefaultInterval
{
  FIRST_HALF      = 0,
  SECOND_HALF     = 1,
  ADDITIONAL_TIME = 2,
  OVERALL         = 3
};

class Interval
{
public:
  Interval(DefaultInterval default_interval);
  // from is 0
  Interval(GameTime to);
  // Note: if from is larger than to, then they're swapped
  Interval(GameTime from, GameTime to);

  ~Interval()
  {}

  // GameTime might be more than the length of the first half,
  // so we have to determine which half we're in
  int checkIfInside(GameTime time, int half);

  std::string get_display_text(void);

  static const std::map<std::string, DefaultInterval> TextToIntervalMap;
  static const std::string DefaultIntervalNames[];
private:
  bool is_default_interval_set_;

  GameTime from_, to_;
  DefaultInterval default_interval_;
};

}
#endif
