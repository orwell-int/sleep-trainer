#include "Day.hpp"

#include <ostream>

#include <TimeLib.h>

namespace sleep
{

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

Day EpochToDay(unsigned long const epoch)
{
  return Day((weekday(epoch) - 1) % 7);
}

std::set< Day > const & Week()
{
  static std::set< Day > const week {
    Day::Monday,
    Day::Tuesday,
    Day::Wednesday,
    Day::Thursday,
    Day::Friday,
    Day::Saturday,
    Day::Sunday
  };
  return week;
}

bool IsWeekDay(Day const day)
{
  return (not (Day::Saturday == day) or (Day::Sunday == day));
}

Day PreviousDay(Day const day)
{
  switch (day)
  {
    case Day::Monday:
    {
      return Day::Sunday;
    }
    default:
    {
      return static_cast< Day >(static_cast< int >(day) - 1);
    }
  }
}

Day NextDay(Day const day)
{
  switch (day)
  {
    case Day::Sunday:
    {
      return Day::Monday;
    }
    default:
    {
      return static_cast< Day >(static_cast< int >(day) + 1);
    }
  }
}

}