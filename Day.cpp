#include "Day.hpp"

#include <ostream>

#include <TimeLib.h>
#include <SoftwareSerial.h>

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


void PrintDay(Day const day)
{
  switch (day)
  {
    case Day::Monday:
    {
      Serial.print("Monday");
      break;
    }
    case Day::Tuesday:
    {
      Serial.print("Tuesday");
      break;
    }
    case Day::Wednesday:
    {
      Serial.print("Wednesday");
      break;
    }
    case Day::Thursday:
    {
      Serial.print("Thursday");
      break;
    }
    case Day::Friday:
    {
      Serial.print("Friday");
      break;
    }
    case Day::Saturday:
    {
      Serial.print("Saturday");
      break;
    }
    case Day::Sunday:
    {
      Serial.print("Sunday");
      break;
    }
  }
}

Day EpochToDay(unsigned long const epoch)
{
  return Day((weekday(epoch) + 5) % 7);
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
