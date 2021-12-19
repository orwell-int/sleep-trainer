#pragma once

#include "global.hpp"

#include "Period.hpp"

namespace sleep
{

#ifdef RUN_TESTS
class TestResult;
#endif // #ifdef RUN_TESTS

/// Very simplistic class to store a time, ignoring leap seconds (as TimeLib).
class Time
{
public:
#ifdef RUN_TESTS
  static void Test(TestResult & testResult);
#endif // #ifdef RUN_TESTS

  Time(int const epoch);

  int getHours() const;
  int getMinutes() const;

  Period getPeriod() const;

  bool operator ==(Time const & rhs) const;
  bool operator !=(Time const & rhs) const;
  bool operator <(Time const & rhs) const;
  bool operator >(Time const & rhs) const;
  bool operator <=(Time const & rhs) const;
  bool operator >=(Time const & rhs) const;

private:
  int const m_epoch;
  int const m_minutesInDay;
};

}
