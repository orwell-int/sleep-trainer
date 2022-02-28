#include "Clock.hpp"

#include <iomanip>
#include <ostream>

#include <TimeLib.h>
#include <SoftwareSerial.h>

#include "Hour.hpp"
#include "Minute.hpp"
#include "Interval.hpp"

namespace sleep
{

CompactTime NightWeek{19, 0};
CompactTime DayWeek{5, 45};
Minute const DurationBeforeNight(30);
Minute const DurationAfterDay(60);
CompactTime NightWeekend{19, 0};
CompactTime DayWeekend{6, 0};

// Default values for the configuration are set in Clock::GetConfig
WeekClockConfig Clock::m_weekClockConfig = {};

Clock::Clock(WallClock const & clock, Day const day)
  : WallClock(clock)
  , m_day(day)
{
}

Clock::Clock(CompactTime const & compactTime, Day const day)
  : WallClock(compactTime)
  , m_day(day)
{
}

Clock::Clock(StrictHour const & hours, StrictMinute const & minutes, Day const day)
  : WallClock(hours, minutes)
  , m_day(day)
{
}

Clock::Clock(Minute const & totalMinutes, Day const day)
  : WallClock(totalMinutes)
  , m_day(day)
{
}

Clock::Clock(unsigned long const epoch)
  : WallClock(epoch)
  , m_day(EpochToDay(epoch))
{
}

Clock::Clock(Clock const & other)
  : WallClock(other)
  , m_day(other.m_day)
{
}

Clock & Clock::operator =(Clock const & other)
{
  WallClock::operator =(other);
  if (this != &other)
  {
    m_day = other.m_day;
  }
  return *this;
}

Period Clock::getPeriod() const
{
  for (auto const period: Period_All)
  {
    Interval interval = Interval::Get(period, m_day);
    if (interval.contains(*this))
    {
      return period;
    }
  }
  Serial.print("Could not get period for clock: ");
  print();
  Serial.println("");
  // there is always a matching period
  assert(false);
  // return WallClock::getPeriod(m_day);
}

Day Clock::day() const
{
  return m_day;
}

Clock Clock::operator +(Hour const & hour) const
{
  Minute const minute(totalMinutes() + hour.minutes());
  Day const day = (minute < StrictMinute::DayDuration)
    ? m_day
    : NextDay(m_day);
  return Clock(minute, day);
}

Clock Clock::operator +(Minute const & minute) const
{
  Minute const newMinute(totalMinutes() + minute);
  Day const day = (newMinute < StrictMinute::DayDuration)
    ? m_day
    : NextDay(m_day);
  return Clock(newMinute, day);
}

Clock Clock::operator -(Hour const & hour) const
{
  Minute const minute(totalMinutes() - hour.minutes());
  Day const day = (minute.get() >= 0) ? m_day : PreviousDay(m_day);
  return Clock(minute, day);
}

Clock Clock::operator -(Minute const & minute) const
{
  Minute const newMinute(totalMinutes() - minute);
  Day const day = (newMinute.get() >= 0) ? m_day : PreviousDay(m_day);
  return Clock(newMinute, day);
}

Minute Clock::operator -(Clock const & rhs) const
{
  return WallClock::operator -(rhs);
}

void Clock::print() const
{
  PrintDay(m_day);
  Serial.print(", ");
  WallClock::print();
}

Clock Clock::Get(Period const period, Day const day)
{
  switch (period)
  {
    case Period::Day:
    {
      Clock clock(Clock::AccessConfig().m_week.at(day).m_nightEnd, day);
      return clock + Clock::AccessConfig().m_week.at(day).m_delayAfterNight;
    }
    case Period::BeforeNight:
    {
      Clock clock(Clock::AccessConfig().m_week.at(day).m_nightStart, day);
      return clock - Clock::AccessConfig().m_week.at(day).m_delayBeforeNight;
    }
    case Period::Night:
    {
      Clock clockNight(
        Clock::AccessConfig().m_week.at(day).m_nightStart,
        day);
      return clockNight;
    }
    case Period::AfterNight:
    {
      Clock clockAfterNight(
        Clock::AccessConfig().m_week.at(day).m_nightEnd,
        day);
      return clockAfterNight;
    }
  }
}

void Clock::SetNightStart(int const hours, int const minutes)
{
  for (auto const & day: Week())
  {
    Clock::SetNightStart(hours, minutes, day);
  }
}

void Clock::SetNightEnd(int const hours, int const minutes)
{
  for (auto const & day: Week())
  {
    Clock::SetNightEnd(hours, minutes, day);
  }
}

void Clock::SetDelayBeforeNight(int const minutes)
{
  for (auto const & day: Week())
  {
    Clock::SetDelayBeforeNight(minutes, day);
  }
}

void Clock::SetDelayAfterNight(int const minutes)
{
  for (auto const & day: Week())
  {
    Clock::SetDelayAfterNight(minutes, day);
  }
}

void Clock::SetNightStart(int const hours, int const minutes, Day const day)
{
  Clock::AccessConfig().m_week.at(day).m_nightStart = CompactTime
    { StrictHour(hours), StrictMinute(minutes) };
}

void Clock::SetNightEnd(int const hours, int const minutes, Day const day)
{
  Clock::AccessConfig().m_week.at(day).m_nightEnd = CompactTime
    { StrictHour(hours), StrictMinute(minutes) };
}

void Clock::SetDelayBeforeNight(int const minutes, Day const day)
{
  Clock::AccessConfig().m_week.at(day).m_delayBeforeNight = Minute(minutes);
}

void Clock::SetDelayAfterNight(int const minutes, Day const day)
{
  Clock::AccessConfig().m_week.at(day).m_delayAfterNight = Minute(minutes);
}

WeekClockConfig const & Clock::GetConfig()
{
  return Clock::AccessConfig();
}

void Clock::SetConfig(WeekClockConfig const & clockConfig)
{
  Clock::m_weekClockConfig = clockConfig;
}

WeekClockConfig & Clock::AccessConfig()
{
  if (Clock::m_weekClockConfig.m_week.empty())
  {
    ClockConfig config
    {
      NightWeek,
      DayWeek,
      Minute(DurationBeforeNight),
      Minute(DurationAfterDay)
    };
    ClockConfig weekendConfig
    {
      NightWeekend,
      DayWeekend,
      Minute(DurationBeforeNight),
      Minute(DurationAfterDay)
    };
    for (Day const & day: Week())
    {
      if (IsWeekDay(day))
      {
        Clock::m_weekClockConfig.m_week.insert(std::make_pair<>(day, config));
      }
      else
      {
        Clock::m_weekClockConfig.m_week.insert(std::make_pair<>(day, weekendConfig));
      }
    }
  }
  return Clock::m_weekClockConfig;
}

std::ostream & operator <<(std::ostream & stream, Clock const & clock)
{
  stream << dynamic_cast< WallClock const & >(clock) << " " << clock.day();
  return stream;
}

}
