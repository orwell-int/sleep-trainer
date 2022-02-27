#include "Interval.hpp"

#include <ostream>

#include <SoftwareSerial.h>

#include "Clock.hpp"

namespace sleep
{

namespace
{

Interval MakeInterval(Period const period, Day const day)
{
  switch (period)
  {
    case Period::Day:
    {
      return Interval(Clock::Get(Period::Day, day), Clock::Get(Period::BeforeNight, day));
    }
    case Period::BeforeNight:
    {
      return Interval(Clock::Get(Period::BeforeNight, day), Clock::Get(Period::Night, day));
    }
    case Period::Night:
    {
      return Interval(Clock::Get(Period::Night, day), Clock::Get(Period::AfterNight, day));
    }
    case Period::AfterNight:
    {
      return Interval(Clock::Get(Period::AfterNight, day), Clock::Get(Period::Day, day));
    }
  }
}

Interval AccessInterval(Period const period, Day const day)
{
  switch (period)
  {
    case Period::Day:
    {
      return MakeInterval(period, day);
    }
    case Period::BeforeNight:
    {
      return MakeInterval(period, day);
    }
    case Period::Night:
    {
      return MakeInterval(period, day);
    }
    case Period::AfterNight:
    {
      return MakeInterval(period, day);
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

void Interval::print() const
{
  Serial.print("[");
  begin.print();
  Serial.print("-");
  end.print();
  Serial.print("]");
}

Interval Interval::Get(Period const period, Day const day)
{
  return AccessInterval(period, day);
}

void Interval::UpdateIntervals()
{
  for (auto const period: Period_All)
  {
    for (Day const day: Week())
    {
      AccessInterval(period, day) = MakeInterval(period, day);
    }
  }
}

std::ostream & operator <<(std::ostream & stream, Interval const & interval)
{
  stream << "[" << interval.begin << "-" << interval.end << "]";
  return stream;
}

}
