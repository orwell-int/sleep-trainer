#pragma once

#include "global.hpp"

#include "Minute.hpp"

namespace sleep
{

#ifdef RUN_TESTS
class TestResult;
#endif // #ifdef RUN_TESTS

class Hour
{
  public:
#ifdef RUN_TESTS
    static void Test(TestResult & testResult);
#endif // #ifdef RUN_TESTS

    Hour(int const value);

    int get() const;

    Minute minutes() const;

    bool operator ==(Hour const& rhs) const;
    bool operator !=(Hour const& rhs) const;
    bool operator <(Hour const& rhs) const;
    bool operator >(Hour const& rhs) const;
    bool operator <=(Hour const& rhs) const;
    bool operator >=(Hour const& rhs) const;

    Hour operator +(Hour const& rhs) const;

    Hour operator -(Hour const& rhs) const;

  private:
    int const m_value;
};

}
