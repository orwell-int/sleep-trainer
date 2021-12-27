#include "Pin.hpp"

#include <ostream>

#include "Hour.hpp"

namespace sleep
{

Pin::Pin(int const epoch)
  : m_value(epoch)
{
}

Pin & Pin::operator =(Pin const & other)
{
  if (this != &other)
  {
    m_value = other.m_value;
  }
}

int Pin::get() const
{
  return m_value;
}

bool Pin::operator ==(Pin const & rhs) const
{
  return m_value == rhs.m_value;
}

bool Pin::operator !=(Pin const & rhs) const
{
  return m_value != rhs.m_value;
}

std::ostream & operator <<(std::ostream & stream, Pin const & pin)
{
  stream << "Pin " << pin.get();
  return stream;
}

}
