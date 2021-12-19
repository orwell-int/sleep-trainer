#pragma once

#include "global.hpp"

#include <iosfwd>

#include "Hour.hpp"
#include "Minute.hpp"

namespace sleep
{

#ifdef RUN_TESTS
class TestResult;
#endif // #ifdef RUN_TESTS

class Hour;
class Minute;

class WallClock
{
  public:
#ifdef RUN_TESTS
    static void Test(TestResult & testResult);
#endif // #ifdef RUN_TESTS

    WallClock(Hour const hours, Minute const minutes);

    Hour const& hours() const;
    Minute const&  minutes() const;

    Minute totalMinutes() const;

    WallClock operator +(Hour const & hour) const;
    WallClock operator +(Minute const & minute) const;

    WallClock operator -(Hour const & hour) const;
    WallClock operator -(Minute const & minute) const;
    Minute operator -(WallClock const & rhs) const;

  private:

    WallClock(Minute const& totalMinutes);

    int rawTotalMinutes() const;

    Hour const m_hours;
    Minute const m_minutes;
};

std::ostream & operator <<(std::ostream & stream, WallClock const& wallClock);

}
