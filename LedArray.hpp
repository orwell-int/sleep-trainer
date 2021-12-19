#pragma once

#include "global.hpp"

#include <iosfwd>

namespace sleep
{
struct LedArray
{
  uint32_t m_colours[NUM_LEDS];

  LedArray();

  void set(uint8_t const index, uint32_t const colour);

  void clear();

  void fill(uint32_t const colour);
};

std::ostream & operator <<(std::ostream & stream, LedArray const & leds);

}
