#pragma once

#include "WallClock.hpp"
#include "Interval.hpp"
#include "LedArray.hpp"

#include <iosfwd>
#include <map>
#include <memory>

namespace sleep
{

struct LedDescriptor
{
  Interval m_interval;
  LedArray m_leds;

  LedDescriptor(Interval const & interval, LedArray const & leds);

  LedDescriptor & operator =(LedDescriptor const & other);

  bool contains(WallClock const & wallClock) const;
};

std::ostream & operator <<(std::ostream & stream, LedDescriptor const & ledDescriptor);
}