#pragma once

#include "global.hpp"

#include <iosfwd>
#include <initializer_list>

namespace sleep
{

enum class Period
{
  Day,
  BeforeNight,
  Night,
  AfterNight
};

constexpr std::initializer_list< Period > Period_All =
{
  Period::Day, Period::BeforeNight, Period::Night, Period::AfterNight
};

std::ostream & operator <<(std::ostream & stream, Period const & period);

}
