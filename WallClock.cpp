#include "WallClock.hpp"

#include <iomanip>
#include <ostream>

#include <TimeLib.h>
#include <SoftwareSerial.h>

#include "Hour.hpp"
#include "Minute.hpp"
#include "Interval.hpp"

namespace sleep
{

CompactTime::CompactTime(StrictHour const & hours, StrictMinute const & minutes)
  : m_hours(hours)
  , m_minutes(minutes)
{
}

WallClock::WallClock(CompactTime const & compactTime)
  : m_hours(compactTime.m_hours)
  , m_minutes(compactTime.m_minutes)
{
}

WallClock::WallClock(StrictHour const & hours, StrictMinute const & minutes)
  : m_hours(hours)
  , m_minutes(minutes)
{
}

WallClock::WallClock(Minute const & totalMinutes)
  : m_hours((totalMinutes.get() / StrictMinute::Max) % StrictHour::Max)
  , m_minutes(totalMinutes.get() % StrictMinute::Max)
{
}

WallClock::WallClock(unsigned long const epoch)
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

StrictHour const & WallClock::hours() const
{
  return m_hours;
}

StrictMinute const & WallClock::minutes() const
{
  return m_minutes;
}

Minute WallClock::totalMinutes() const
{
  return Minute(m_hours.get() * StrictMinute::Max) + m_minutes;
}

// Period WallClock::getPeriod(Day const day) const
// {
//   for (auto const period: Period_All)
//   {
//     if (Interval::Get(period, day).contains(*this))
//     {
//       return period;
//     }
//   }
//   // there is always a matching period
//   assert(false);
// }

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
    return Minute(mine - others + StrictMinute::Max * StrictHour::Max);
  }
}

bool WallClock::operator ==(WallClock const& rhs) const
{
  return (m_hours == rhs.m_hours) and (m_minutes == rhs.m_minutes);
}

bool WallClock::operator !=(WallClock const& rhs) const
{
  return (m_hours != rhs.m_hours) or (m_minutes != rhs.m_minutes);
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

void WallClock::print() const
{
  int hour = m_hours.get();
  if (hour < 10)
  {
    Serial.print("0");
  }
  Serial.print(hour);
  Serial.print(":");
  int minute = m_minutes.get();
  if (minute < 10)
  {
    Serial.print("0");
  }
  Serial.print(minute);
}

// void WallClock::SetNightStart(int const hours, int const minutes)
// {
//   for (auto const & day: Week())
//   {
//     Clock::SetNightStart(hours, minutes, day);
//   }
// }

// void WallClock::SetNightEnd(int const hours, int const minutes)
// {
//   for (auto const & day: Week())
//   {
//     Clock::SetNightEnd(hours, minutes, day);
//   }
// }

// void WallClock::SetDelayBeforeNight(int const minutes)
// {
//   for (auto const & day: Week())
//   {
//     Clock::SetDelayBeforeNight(minutes, day);
//   }
// }

// void WallClock::SetDelayAfterNight(int const minutes)
// {
//   for (auto const & day: Week())
//   {
//     Clock::SetDelayAfterNight(minutes, day);
//   }
// }

int WallClock::rawTotalMinutes() const
{
  return m_minutes.get() + m_hours.get() * StrictMinute::Max;
}

std::ostream & operator <<(std::ostream & stream, WallClock const& wallClock)
{
  stream << std::setw(2) << std::setfill('0') << wallClock.hours().get();
  stream << ":" << std::setw(2) << std::setfill('0') << wallClock.minutes().get();
  return stream;
}

}
