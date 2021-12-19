#pragma once

#include "global.hpp"

#include <iosfwd>

namespace sleep
{

#ifdef RUN_TESTS
class TestResult;
#endif // #ifdef RUN_TESTS

class Pin
{
  public:
#ifdef RUN_TESTS
    static void Test(TestResult & testResult);
#endif // #ifdef RUN_TESTS

    explicit Pin(int const value);

    Pin & operator =(Pin const & other);

    int get() const;

    bool operator ==(Pin const & rhs) const;
    bool operator !=(Pin const & rhs) const;

  private:
    int m_value;
};

std::ostream & operator <<(std::ostream & stream, Pin const & pin);

}
