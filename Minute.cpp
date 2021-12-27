#include "WallClock.hpp"

#include <ostream>

#include "Hour.hpp"

namespace sleep
{

Minute::Minute(int const value)
  : m_value(value)
{
}

Minute & Minute::operator =(Minute const & other)
{
  if (this != &other)
  {
    m_value = other.m_value;
  }
  return *this;
}

int Minute::get() const
{
  return m_value;
}

bool Minute::operator ==(Minute const & rhs) const
{
  return m_value == rhs.m_value;
}

bool Minute::operator !=(Minute const & rhs) const
{
  return m_value != rhs.m_value;
}

bool Minute::operator <(Minute const & rhs) const
{
  return m_value < rhs.m_value;
}

bool Minute::operator >(Minute const & rhs) const
{
  return m_value > rhs.m_value;
}

bool Minute::operator <=(Minute const & rhs) const
{
  return m_value <= rhs.m_value;
}

bool Minute::operator >=(Minute const & rhs) const
{
  return m_value >= rhs.m_value;
}

Minute Minute::operator +(Minute const & rhs) const
{
  return Minute(m_value + rhs.m_value);
}

Minute Minute::operator -(Minute const & rhs) const
{
  return Minute(m_value - rhs.m_value);
}

Minute Minute::operator +(Hour const & rhs) const
{
  return *this + rhs.minutes();
}

Minute Minute::operator -(Hour const & rhs) const
{
  return *this - rhs.minutes();
}

std::ostream & operator <<(std::ostream & stream, Minute const & minute)
{
  stream << minute.get() << "m";
  return stream;
}

}
