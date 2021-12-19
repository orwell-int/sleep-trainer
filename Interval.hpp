#pragma once

#include "global.hpp"

#include "WallClock.hpp"
#include "Period.hpp"

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
};

std::ostream & operator <<(std::ostream & stream, Interval const & interval);
}
