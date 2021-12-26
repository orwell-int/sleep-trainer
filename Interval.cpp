#include "Interval.hpp"

#include <ostream>

namespace sleep
{

namespace
{

Interval MakeInterval(Period const period)
{
  switch (period)
  {
    case Period::Day:
    {
      return Interval(WallClock::Get(Period::Day), WallClock::Get(Period::BeforeNight));
    }
    case Period::BeforeNight:
    {
      return Interval(WallClock::Get(Period::BeforeNight), WallClock::Get(Period::Night));
    }
    case Period::Night:
    {
      return Interval(WallClock::Get(Period::Night), WallClock::Get(Period::AfterNight));
    }
    case Period::AfterNight:
    {
      return Interval(WallClock::Get(Period::AfterNight), WallClock::Get(Period::Day));
    }
  }
}

Interval & AccessInterval(Period const period)
{
  switch (period)
  {
    case Period::Day:
    {
      static Interval day = MakeInterval(period);
      return day;
    }
    case Period::BeforeNight:
    {
      static Interval beforeNight = MakeInterval(period);
      return beforeNight;
    }
    case Period::Night:
    {
      static Interval night = MakeInterval(period);
      return night;
    }
    case Period::AfterNight:
    {
      static Interval afterNight = MakeInterval(period);
      return afterNight;
    }
  }
}

}

Interval::Interval(WallClock const & o_begin, WallClock const & o_end)
  : begin(o_begin)
  , end(o_end)
{
}

Interval & Interval::operator =(Interval const & other)
{
  if (this != &other)
  {
    begin = other.begin;
    end = other.end;
  }
  return *this;
}

bool Interval::contains(WallClock const & wallClock) const
{
  if (end < begin)
  {
    // cross 00:00
    return ((wallClock >= begin) or (wallClock < end));
  }
  else
  {
    return ((wallClock >= begin) and (wallClock < end));
  }
}

Minute Interval::duration() const
{
  return end - begin;
}

Interval const & Interval::Get(Period const period)
{
  return AccessInterval(period);
}

void Interval::UpdateIntervals()
{
  for (auto const period: Period_All)
  {
    AccessInterval(period) = MakeInterval(period);
  }
}

std::ostream & operator <<(std::ostream & stream, Interval const & interval)
{
  stream << "[" << interval.begin << "-" << interval.end << "]";
  return stream;
}

}
