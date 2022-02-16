#include "StrictMinute.hpp"

#include "StrictHour.hpp"

namespace sleep
{

#ifdef RUN_TESTS
void Test(TestResult & testResult)
{
  StrictMinute five(5);
}
#endif // #ifdef RUN_TESTS

Minute const StrictMinute::DayDuration(StrictMinute::Max * StrictHour::Max);

StrictMinute::StrictMinute(int const value)
  : Minute(value % StrictMinute::Max)
{
}

}