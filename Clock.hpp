#pragma once

#include <iosfwd>
#include <map>

#include "global.hpp"

#include "WallClock.hpp"
#include "Hour.hpp"
#include "Minute.hpp"
#include "Day.hpp"
#include "Period.hpp"

namespace sleep
{

#ifdef RUN_TESTS
class TestResult;
#endif // #ifdef RUN_TESTS

struct ClockConfig
{
  CompactTime m_nightStart;
  CompactTime m_nightEnd;
  Minute m_delayBeforeNight;
  Minute m_delayAfterNight;
};

struct WeekClockConfig
{
  std::map< Day, ClockConfig > m_week;
};

class Clock : public WallClock
{
  public:
#ifdef RUN_TESTS
    static void Test(TestResult & testResult);
#endif // #ifdef RUN_TESTS

    Clock(WallClock const & clock, Day const day);
    Clock(CompactTime const & compactTime, Day const day);
    Clock(StrictHour const & hours, StrictMinute const & minutes, Day const day);
    Clock(Minute const & totalMinutes, Day const day);
    Clock(unsigned long const epoch);

    Clock(Clock const & other);

    Clock & operator =(Clock const & other);

    Period getPeriod() const;

    Day day() const;

    Clock operator +(Hour const & hour) const;
    Clock operator +(Minute const & minute) const;

    Clock operator -(Hour const & hour) const;
    Clock operator -(Minute const & minute) const;
    Minute operator -(Clock const & rhs) const;

    static Clock const & Get(Period const period, Day const day);
    static void SetNightStart(int const hours, int const minutes);
    static void SetNightEnd(int const hours, int const minutes);
    static void SetDelayBeforeNight(int const minutes);
    static void SetDelayAfterNight(int const minutes);
    static void SetNightStart(int const hours, int const minutes, Day const day);
    static void SetNightEnd(int const hours, int const minutes, Day const day);
    static void SetDelayBeforeNight(int const minutes, Day const day);
    static void SetDelayAfterNight(int const minutes, Day const day);
    static WeekClockConfig const & GetConfig();
    static void SetConfig(WeekClockConfig const & weekClockConfig); 
  private:
    static WeekClockConfig & AccessConfig();

    Day m_day;
    static WeekClockConfig m_weekClockConfig;
    
};

std::ostream & operator <<(std::ostream & stream, Clock const& Clock);

}