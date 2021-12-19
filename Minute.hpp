#pragma once

#include "global.hpp"

#include <iosfwd>

namespace sleep
{

#ifdef RUN_TESTS
class TestResult;
#endif // #ifdef RUN_TESTS

class Hour;

class Minute
{
  public:
#ifdef RUN_TESTS
    static void Test(TestResult & testResult);
#endif // #ifdef RUN_TESTS

    Minute(int const value);

    Minute & operator =(Minute const & other);

    int get() const;

    bool operator ==(Minute const & rhs) const;
    bool operator !=(Minute const & rhs) const;
    bool operator <(Minute const & rhs) const;
    bool operator >(Minute const & rhs) const;
    bool operator <=(Minute const & rhs) const;
    bool operator >=(Minute const & rhs) const;

    Minute operator +(Minute const & rhs) const;

    Minute operator -(Minute const & rhs) const;

    Minute operator +(Hour const & rhs) const;

    Minute operator -(Hour const & rhs) const;
  private:
    int m_value;
};

std::ostream & operator <<(std::ostream & stream, Minute const & minute);

}
