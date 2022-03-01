#pragma once

#include "global.hpp"

#include "Minute.hpp"

namespace sleep
{

#ifdef RUN_TESTS
class TestResult;
#endif // #ifdef RUN_TESTS

class StrictMinute : public Minute
{
public:
#ifdef RUN_TESTS
  static void Test(TestResult & testResult);
#endif // #ifdef RUN_TESTS

  static int const Max = 60;
  static int const Seconds = 60;
  static Minute const DayDuration;
  static Minute const WeekDuration;

  StrictMinute(int const value);
};

}