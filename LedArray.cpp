#include "LedArray.hpp"

#include <assert.h>
#include <ostream>
#include <iomanip>

namespace sleep
{

LedArray::LedArray()
{
  clear();
}

void LedArray::set(uint8_t const index, uint32_t const colour)
{
  assert(index < NUM_LEDS);
  m_colours[index] = colour;
}

void LedArray::clear()
{
  for (auto & colour: m_colours)
  {
    colour = 0;
  }
}

void LedArray::fill(uint32_t const colourValue)
{
  for (auto & colour: m_colours)
  {
    colour = colourValue;
  }
}

std::ostream & operator <<(std::ostream & stream, LedArray const & leds)
{
  for (auto const & colour: leds.m_colours)
  {
    int const r = colour >> 16;
    int const g = (colour & 0xFF00) >> 8;
    int const b = (colour & 0xFF);
    stream << " (" << std::setw(3) << r << ", "
      << std::setw(3) << g << ", "
      << std::setw(3) << b << ")";
  }
  return stream;
}

}
