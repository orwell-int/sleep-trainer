#include "Time.hpp"

#include <TimeLib.h>


namespace sleep
{

int const START_HOURS = 19;
int const START_MINUTES = 0;
int const STOP_HOURS = 6;
int const STOP_MINUTES = 0;

Time::Time(int const epoch)
  : m_epoch(epoch)
  , m_minutesInDay(epoch % 60)
{
}

int Time::getHours() const
{
  return hour(m_epoch);
}

int Time::getMinutes() const
{
  return minute(m_epoch);
}

Period Time::getPeriod() const
{
  int const hours = getHours();
  int const minutes = getMinutes();
  if (((hours == START_HOURS and minutes >= START_MINUTES) or (hours > START_HOURS))
    or ((hours == STOP_HOURS and minutes < STOP_MINUTES) or (hours < STOP_HOURS)))
  {
    return Period::Night;
  }
  if (((hours == START_HOURS and minutes >= START_MINUTES) or (hours > START_HOURS))
    or ((hours == STOP_HOURS and minutes < STOP_MINUTES) or (hours < STOP_HOURS)))
  {
    return Period::Night;
  }
  return Period::Day;
}

bool Time::operator ==(Time const & rhs) const
{
  return m_epoch == rhs.m_epoch;
}

bool Time::operator !=(Time const & rhs) const
{
  return m_epoch != rhs.m_epoch;
}

bool Time::operator <(Time const & rhs) const
{
  return m_epoch < rhs.m_epoch;
}

bool Time::operator >(Time const & rhs) const
{
  return m_epoch > rhs.m_epoch;
}

bool Time::operator <=(Time const & rhs) const
{
  return m_epoch <= rhs.m_epoch;
}

bool Time::operator >=(Time const & rhs) const
{
  return m_epoch >= rhs.m_epoch;
}


}
