#include "StrictHour.hpp"

namespace sleep
{

#ifdef RUN_TESTS
void Test(TestResult & testResult)
{
  StrictHour five(5);
}
#endif // #ifdef RUN_TESTS

StrictHour::StrictHour(int const value)
  : Hour(value % StrictHour::Max)
{
}

}