#include "Interval.hpp"

#include <ostream>

namespace sleep
{

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

std::ostream & operator <<(std::ostream & stream, Interval const & interval)
{
  stream << "[" << interval.begin << "-" << interval.end << "]";
  return stream;
}

}