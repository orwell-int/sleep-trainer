#pragma once

#include "global.hpp"

#include "Hour.hpp"

namespace sleep
{

#ifdef RUN_TESTS
class TestResult;
#endif // #ifdef RUN_TESTS

class StrictHour : public Hour
{
public:
#ifdef RUN_TESTS
  static void Test(TestResult & testResult);
#endif // #ifdef RUN_TESTS

  static int const Max = 24;

  StrictHour(int const value);
};

}