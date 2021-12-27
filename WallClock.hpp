#pragma once

#include "global.hpp"

#include <iosfwd>

#include "Hour.hpp"
#include "Minute.hpp"
#include "Period.hpp"

namespace sleep
{

#ifdef RUN_TESTS
class TestResult;
#endif // #ifdef RUN_TESTS

class Hour;
class Minute;
class Time;

struct ClockConfig
{
    Hour m_nightStartHours;
    Minute m_nightStartMinutes;
    Hour m_nightEndHours;
    Minute m_nightEndMinutes;
    Minute m_delayBeforeNight;
    Minute m_delayAfterNight;
};

class WallClock
{
  public:
#ifdef RUN_TESTS
    static void Test(TestResult & testResult);
#endif // #ifdef RUN_TESTS

    WallClock(Hour const & hours, Minute const & minutes);
    WallClock(Minute const & totalMinutes);
    WallClock(int const epoch);

    WallClock(WallClock const & other);

    WallClock & operator =(WallClock const & other);

    Hour const& hours() const;
    Minute const&  minutes() const;

    Minute totalMinutes() const;

    Period getPeriod() const;

    WallClock operator +(Hour const & hour) const;
    WallClock operator +(Minute const & minute) const;

    WallClock operator -(Hour const & hour) const;
    WallClock operator -(Minute const & minute) const;
    Minute operator -(WallClock const & rhs) const;

    bool operator ==(WallClock const& rhs) const;
    bool operator !=(WallClock const& rhs) const;
    bool operator <(WallClock const& rhs) const;
    bool operator >(WallClock const& rhs) const;
    bool operator <=(WallClock const& rhs) const;
    bool operator >=(WallClock const& rhs) const;

    static WallClock const & Get(Period const period);
    static void SetNightStart(int const hours, int const minutes);
    static void SetNightEnd(int const hours, int const minutes);
    static void SetDelayBeforeNight(int const minutes);
    static void SetDelayAfterNight(int const minutes);
    static ClockConfig const & GetConfig();
    static void SetConfig(ClockConfig const & clockConfig); 
  private:


    int rawTotalMinutes() const;

    Hour m_hours;
    Minute m_minutes;

    static ClockConfig m_clockConfig;
    
};

std::ostream & operator <<(std::ostream & stream, WallClock const& wallClock);

}
