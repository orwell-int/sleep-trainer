#pragma once

#include "global.hpp"

#include <iosfwd>
#include <set>

namespace sleep
{

#ifdef RUN_TESTS
class TestResult;
#endif // #ifdef RUN_TESTS

enum class Day
{
  Monday,
  Tuesday,
  Wednesday,
  Thursday,
  Friday,
  Saturday,
  Sunday
};

std::ostream & operator <<(std::ostream & stream, Day const & day);

void PrintDay(Day const day);

Day EpochToDay(unsigned long const epoch);

std::set< Day > const & Week();
bool IsWeekDay(Day const day);
Day PreviousDay(Day const day);
Day NextDay(Day const day);

}
