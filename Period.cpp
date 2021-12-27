#include "Period.hpp"

#include <ostream>

namespace sleep
{

std::ostream & operator <<(std::ostream & stream, Period const & period)
{
  switch (period)
  {
    case Period::Day:
      {
        stream << "Period::Day";
        break;
      }
    case Period::BeforeNight:
      {
        stream << "Period::BeforeNight";
        break;
      }
    case Period::Night:
      {
        stream << "Period::Night";
        break;
      }
    case Period::AfterNight:
      {
        stream << "Period::AfterNight";
        break;
      }
  }
  return stream;
}

}
