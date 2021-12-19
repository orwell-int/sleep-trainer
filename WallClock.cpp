#include "WallClock.hpp"

#include <iomanip>
#include <ostream>

#include "Hour.hpp"
#include "Minute.hpp"

namespace sleep
{

WallClock::WallClock(Hour const hours, Minute const minutes)
  : m_hours(hours.get() % 24)
  , m_minutes(minutes.get() % 60)
{
}

Hour const& WallClock::hours() const
{
  return m_hours;
}

Minute const& WallClock::minutes() const
{
  return m_minutes;
}

Minute WallClock::totalMinutes() const
{
  return Minute(m_hours.get() * 60) + m_minutes;
}

WallClock WallClock::operator +(Hour const & hour) const
{
  return WallClock(totalMinutes() + hour.minutes());
}

WallClock WallClock::operator +(Minute const & minute) const
{
  return WallClock(totalMinutes() + minute);
}

WallClock WallClock::operator -(Hour const & hour) const
{
  return WallClock(totalMinutes() - hour.minutes());
}

WallClock WallClock::operator -(Minute const & minute) const
{
  return WallClock(totalMinutes() - minute);
}

Minute WallClock::operator -(WallClock const & rhs) const
{
  int const mine = rawTotalMinutes();
  int const others = rhs.rawTotalMinutes();
  if (mine > others)
  {
    return Minute(mine - others);
  }
  else
  {
    return Minute(mine - others + 60 * 24);
  }
}

WallClock::WallClock(Minute const& totalMinutes)
  : m_hours((totalMinutes.get() / 60) % 24)
  , m_minutes(totalMinutes.get() % 60)
{
}

int WallClock::rawTotalMinutes() const
{
  return m_minutes.get() + m_hours.get() * 60;
}

std::ostream & operator <<(std::ostream & stream, WallClock const& wallClock)
{
  stream << std::setw(2) << std::setfill('0') << wallClock.hours().get() << ":" << std::setw(2) << wallClock.minutes().get();
  return stream;
}

}
