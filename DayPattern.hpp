#pragma once

#include "global.hpp"

#include <set>
#include <iosfwd>

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

class DayPattern
{
  public:
#ifdef RUN_TESTS
    static void Test(TestResult & testResult);
#endif // #ifdef RUN_TESTS

    DayPattern(std::set< Day > const & days);

    DayPattern & operator =(DayPattern const & other);

    bool operator ==(DayPattern const & rhs) const;
    bool operator !=(DayPattern const & rhs) const;

    bool operator <(DayPattern const & rhs) const;

    bool contains(DayPattern const & other) const;

  private:
    std::set< Day > m_days;
};

std::ostream & operator <<(std::ostream & stream, Day const & day);
std::ostream & operator <<(std::ostream & stream, DayPattern const & dayPattern);

}