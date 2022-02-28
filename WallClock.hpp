#pragma once

#include <iosfwd>
#include <map>

#include "global.hpp"

#include "Hour.hpp"
#include "Minute.hpp"
#include "StrictHour.hpp"
#include "StrictMinute.hpp"
#include "Period.hpp"

namespace sleep
{

struct CompactTime
{
  StrictHour m_hours;
  StrictMinute m_minutes;

  CompactTime(StrictHour const & hours, StrictMinute const & minutes);
};

#ifdef RUN_TESTS
class TestResult;
#endif // #ifdef RUN_TESTS

class Time;

class WallClock
{
  public:
#ifdef RUN_TESTS
    static void Test(TestResult & testResult);
#endif // #ifdef RUN_TESTS

    WallClock(CompactTime const & compactTime);
    WallClock(StrictHour const & hours, StrictMinute const & minutes);
    WallClock(Minute const & totalMinutes);
    WallClock(unsigned long const epoch);

    WallClock(WallClock const & other);

    WallClock & operator =(WallClock const & other);

    StrictHour const & hours() const;
    StrictMinute const & minutes() const;

    Minute totalMinutes() const;

    // Period getPeriod(Day const day) const;

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

    void print() const;

    // static void SetNightStart(int const hours, int const minutes);
    // static void SetNightEnd(int const hours, int const minutes);
    // static void SetDelayBeforeNight(int const minutes);
    // static void SetDelayAfterNight(int const minutes);
  private:

    int rawTotalMinutes() const;

    StrictHour m_hours;
    StrictMinute m_minutes;
};

std::ostream & operator <<(std::ostream & stream, WallClock const& wallClock);

}
