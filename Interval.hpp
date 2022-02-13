#pragma once

#include "global.hpp"

#include "WallClock.hpp"
#include "Period.hpp"
#include "Day.hpp"

#include <iosfwd>

namespace sleep
{
struct Interval
{
  WallClock begin;
  WallClock end;

  Interval(WallClock const & begin, WallClock const & end);

  Interval & operator =(Interval const & other);

  bool contains(WallClock const & wallClock) const;

  Minute duration() const;

  static Interval const & Get(Period const period, Day const day);

  /// This should be called when the configuration for the periods is updated
  static void UpdateIntervals();
};

std::ostream & operator <<(std::ostream & stream, Interval const & interval);
}