#pragma once

#include "global.hpp"

#include <iosfwd>

namespace sleep
{

enum class Period
{
  Day,
  BeforeNight,
  Night,
  AfterNight
};

std::ostream & operator <<(std::ostream & stream, Period const& period);

}
