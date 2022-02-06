#include "Hour.hpp"

namespace sleep
{

#ifdef RUN_TESTS
void Test(TestResult & testResult)
{
  Hour five(5);
}
#endif // #ifdef RUN_TESTS

Hour::Hour(int const epoch)
  : m_value(epoch)
{
}

Hour & Hour::operator =(Hour const & other)
{
  if (this != &other)
  {
    m_value = other.m_value;
  }
  return *this;
}

int Hour::get() const
{
  return m_value;
}

Minute Hour::minutes() const
{
  return Minute(m_value * 60);
}

bool Hour::operator ==(Hour const & rhs) const
{
  return m_value == rhs.m_value;
}

bool Hour::operator !=(Hour const & rhs) const
{
  return m_value != rhs.m_value;
}

bool Hour::operator <(Hour const & rhs) const
{
  return m_value < rhs.m_value;
}

bool Hour::operator >(Hour const & rhs) const
{
  return m_value > rhs.m_value;
}

bool Hour::operator <=(Hour const & rhs) const
{
  return m_value <= rhs.m_value;
}

bool Hour::operator >=(Hour const & rhs) const
{
  return m_value >= rhs.m_value;
}

Hour Hour::operator +(Hour const & rhs) const
{
  return Hour(m_value + rhs.m_value);
}

Hour Hour::operator -(Hour const & rhs) const
{
  return Hour(m_value - rhs.m_value);
}

}
