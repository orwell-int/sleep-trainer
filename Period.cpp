#include "Period.hpp"

#include <ostream>

#include <SoftwareSerial.h>

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

void Print(Period const period)
{
  switch (period)
  {
    case Period::Day:
      {
        Serial.print("Period::Day");
        break;
      }
    case Period::BeforeNight:
      {
        Serial.print("Period::BeforeNight");
        break;
      }
    case Period::Night:
      {
        Serial.print("Period::Night");
        break;
      }
    case Period::AfterNight:
      {
        Serial.print("Period::AfterNight");
        break;
      }
  }
}

}
