#include "LedDescriptor.hpp"

#include <ostream>

namespace sleep
{

LedDescriptor::LedDescriptor(Interval const & interval, LedArray const & leds)
  : m_interval(interval)
  , m_leds(leds)
{
}

LedDescriptor & LedDescriptor::operator =(LedDescriptor const & other)
{
  if (this != &other)
  {
    m_interval = other.m_interval;
    m_leds = other.m_leds;
  }
  return *this;
}

bool LedDescriptor::contains(WallClock const & wallClock) const
{
  return m_interval.contains(wallClock);
}

//Period LedDescriptor::getPeriod() const
//{
//  return m_interval.getPeriod();
//}

std::ostream & operator <<(std::ostream & stream, LedDescriptor const & ledDescriptor)
{
  stream << ledDescriptor.m_interval << " -> " << ledDescriptor.m_leds;
  return stream;
}
  
}
