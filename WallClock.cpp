#include "WallClock.hpp"

#include <iomanip>
#include <ostream>

#include <TimeLib.h>

#include "Hour.hpp"
#include "Minute.hpp"
#include "Interval.hpp"

namespace sleep
{

// Default values for the configuration
ClockConfig WallClock::m_clockConfig =
{
  Hour(19), Minute(0),
  Hour(5), Minute(45),
  Minute(30),
  Minute(60)
};

WallClock::WallClock(Hour const & hours, Minute const & minutes)
  : m_hours(hours.get() % 24)
  , m_minutes(minutes.get() % 60)
{
}

WallClock::WallClock(Minute const & totalMinutes)
  : m_hours((totalMinutes.get() / 60) % 24)
  , m_minutes(totalMinutes.get() % 60)
{
}

WallClock::WallClock(int const epoch)
  : m_hours(hour(epoch))
  , m_minutes(minute(epoch))
{
}

WallClock::WallClock(WallClock const& other)
  : m_hours(other.m_hours)
  , m_minutes(other.m_minutes)
{
}

WallClock & WallClock::operator =(WallClock const & other)
{
  if (this != &other)
  {
    m_hours = other.m_hours;
    m_minutes = other.m_minutes;
  }
  return *this;
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

Period WallClock::getPeriod() const
{
  for (auto const period: Period_All)
  {
    if (Interval::Get(period).contains(*this))
    {
      return period;
    }
  }
  // there is always a matching period
  assert(false);
}

WallClock WallClock::operator +(Hour const & hour) const
{
  return WallClock(totalMinutes() + hour.minutes());
}

WallClock WallClock::operator +(Minute const & minute) const
{
  return WallClock(Minute(rawTotalMinutes() + minute.get()));
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

bool WallClock::operator ==(WallClock const& rhs) const
{
  return (m_hours == rhs.m_hours) and (m_minutes == rhs.m_minutes);
}

bool WallClock::operator !=(WallClock const& rhs) const
{
  return (m_hours != rhs.m_hours) and (m_minutes != rhs.m_minutes);
}

bool WallClock::operator <(WallClock const& rhs) const
{
  return ((m_hours == rhs.m_hours) and (m_minutes < rhs.m_minutes))
    or (m_hours < rhs.m_hours);
}

bool WallClock::operator >(WallClock const& rhs) const
{
  return ((m_hours == rhs.m_hours) and (m_minutes > rhs.m_minutes))
    or (m_hours > rhs.m_hours);
}

bool WallClock::operator <=(WallClock const& rhs) const
{
  return ((m_hours == rhs.m_hours) and (m_minutes <= rhs.m_minutes))
    or (m_hours < rhs.m_hours);
}

bool WallClock::operator >=(WallClock const& rhs) const
{
  return ((m_hours == rhs.m_hours) and (m_minutes >= rhs.m_minutes))
    or (m_hours > rhs.m_hours);
}

WallClock const & WallClock::Get(Period const period)
{
  switch (period)
  {
    case Period::Day:
    {
      static WallClock const clockDay(
        WallClock::Get(Period::AfterNight)
         + Minute(WallClock::m_clockConfig.m_delayAfterNight));
      return clockDay;
    }
    case Period::BeforeNight:
    {
      static WallClock const clockBeforeNight(WallClock::Get(Period::Night)
       - Minute(WallClock::m_clockConfig.m_delayBeforeNight));
      return clockBeforeNight;
    }
    case Period::Night:
    {
      static WallClock const clockNight(
        WallClock::m_clockConfig.m_nightStartHours,
         WallClock::m_clockConfig.m_nightStartMinutes);
      return clockNight;
    }
    case Period::AfterNight:
    {
      static WallClock const clockAfterNight(
        WallClock::m_clockConfig.m_nightEndHours,
         WallClock::m_clockConfig.m_nightEndMinutes);
      return clockAfterNight;
    }
  }
}

void WallClock::SetNightStart(int const hours, int const minutes)
{
  WallClock::m_clockConfig.m_nightStartHours = Hour(hours);
  WallClock::m_clockConfig.m_nightStartMinutes = Minute(minutes);
}

void WallClock::SetNightEnd(int const hours, int const minutes)
{
  WallClock::m_clockConfig.m_nightEndHours = Hour(hours);
  WallClock::m_clockConfig.m_nightEndMinutes = Minute(minutes);
}

void WallClock::SetDelayBeforeNight(int const minutes)
{
  WallClock::m_clockConfig.m_delayBeforeNight = Minute(minutes);
}

void WallClock::SetDelayAfterNight(int const minutes)
{
  WallClock::m_clockConfig.m_delayAfterNight = Minute(minutes);
}

ClockConfig const & WallClock::GetConfig()
{
  return WallClock::m_clockConfig;
}

void WallClock::SetConfig(ClockConfig const & clockConfig)
{
  WallClock::m_clockConfig = clockConfig;
}

int WallClock::rawTotalMinutes() const
{
  return m_minutes.get() + m_hours.get() * 60;
}

std::ostream & operator <<(std::ostream & stream, WallClock const& wallClock)
{
  stream << std::setw(2) << std::setfill('0') << wallClock.hours().get();
  stream << ":" << std::setw(2) << std::setfill('0') << wallClock.minutes().get();
  return stream;
}

}
