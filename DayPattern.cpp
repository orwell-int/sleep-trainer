#include "DayPattern.hpp"

#include <ostream>
#include <numeric>
#include <algorithm>

namespace sleep
{

#ifdef RUN_TESTS
void Test(TestResult & testResult)
{
  DayPattern weekDays({Monday, Tuesday, Wednesday, Thursday, Friday});
  DayPattern weekEnd({Saturday, Sunday});
}
#endif // #ifdef RUN_TESTS

DayPattern::DayPattern(std::set< Day > const & days)
  : m_days(days)
{
}

DayPattern & DayPattern::operator =(DayPattern const & other) = default;

bool DayPattern::operator ==(DayPattern const & rhs) const
{
  return m_days == rhs.m_days;
}

bool DayPattern::operator !=(DayPattern const & rhs) const
{
  return m_days != rhs.m_days;
}

bool DayPattern::operator <(DayPattern const & rhs) const
{
  std::set< int > rhsInts;
  std::transform(
    rhs.m_days.begin(),
    rhs.m_days.end(),
    std::inserter(rhsInts, rhsInts.end()),
    [](Day const & day) { return static_cast< int >(day);});
  int const rhsSum = std::accumulate(rhsInts.begin(), rhsInts.end(), 0);
  std::set< int > ints;
  std::transform(
    m_days.begin(),
    m_days.end(),
    std::inserter(ints, ints.end()),
    [](Day const & day) { return static_cast< int >(day);});
  int const sum = std::accumulate(ints.begin(), ints.end(), 0);
  return sum < rhsSum;
}

std::ostream & operator <<(std::ostream & stream, Day const & day)
{
  switch (day)
  {
    case Day::Monday:
    {
      return stream << "Monday";
    }
    case Day::Tuesday:
    {
      return stream << "Tuesday";
    }
    case Day::Wednesday:
    {
      return stream << "Wednesday";
    }
    case Day::Thursday:
    {
      return stream << "Thursday";
    }
    case Day::Friday:
    {
      return stream << "Friday";
    }
    case Day::Saturday:
    {
      return stream << "Saturday";
    }
    case Day::Sunday:
    {
      return stream << "Sunday";
    }
  }
  return stream;
}

std::ostream & operator <<(std::ostream & stream, DayPattern const & dayPattern)
{
  return stream;
}

}