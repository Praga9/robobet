#include "interval.hpp"

const std::map<std::string, robobet::DefaultInterval>
  robobet::Interval::TextToIntervalMap =
  { {"FIRST_HALF",      robobet::DefaultInterval::FIRST_HALF},
    {"SECOND_HALF",     robobet::DefaultInterval::SECOND_HALF},
    {"ADDITIONAL_TIME", robobet::DefaultInterval::ADDITIONAL_TIME},
    {"OVERALL",         robobet::DefaultInterval::OVERALL},         };

const std::string robobet::Interval::DefaultIntervalNames[] = {
  "első félidő",    // FIRST_HALF
  "második félidő", // SECOND_HALF
  "hosszabbítás",   // ADDITIONAL_TIME
  "az egész meccs", // OVERALL
};

robobet::Interval::Interval(robobet::DefaultInterval default_interval):
  is_default_interval_set_(true),
  default_interval_(default_interval)
{}

robobet::Interval::Interval(GameTime to):
  is_default_interval_set_(false)
{
  if ((to < 0) || (to > 6000))
  {
    throw std::invalid_argument("value out of boundaries");
  }
  else
  {
    to_ = to;
  }
}


robobet::Interval::Interval(GameTime from, GameTime to):
  is_default_interval_set_(false)
{
  if (from > to)
  {
    std::swap(from, to);
  }

  if ((from < 0) || (to < 0) || (from > 6000) || (to > 6000))
  {
    throw std::invalid_argument("value out of boundaries");
  }
  else
  {
    from_ = from;
    to_   = to;
  }
}

// -1 before
//  0 on time
//  1 after
int robobet::Interval::checkIfInside(GameTime time, int half)
{
  using namespace robobet;

  if (is_default_interval_set_)
  {
    switch (default_interval_)
    {
      case DefaultInterval::FIRST_HALF:
        if (half == 1)
          return 0;
        else
          return 1;
      case DefaultInterval::SECOND_HALF:
        if (half == 2)
          return 0;
        else if (half == 1)
          return -1;
        else
          return 1;
      case DefaultInterval::ADDITIONAL_TIME:
        if (half < 3)
          return -1;
        else
          return 1;
      default:  // OVERALL
        return 0;
    }
  }
  else
  {
    if (time < from_)
      return -1;
    if (time > to_)
      return 1;

    return 0;
  }
}

std::string robobet::Interval::get_display_text(void)
{
  using namespace robobet;

  std::stringstream ss;

  if (is_default_interval_set_)
  {
    switch (default_interval_)
    {
      case DefaultInterval::FIRST_HALF:
        ss << " az első félidőben.";
        break;
      case DefaultInterval::SECOND_HALF:
        ss << " a második félidőben.";
        break;
      case DefaultInterval::ADDITIONAL_TIME:
        ss << " a hosszabbításban.";
        break;
      default:  // OVERALL
        break;
    }
  }
  else
  {
    ss << " ekkor: "<< from_ << " - " << to_ << ".";
  }

  return ss.str();
}
