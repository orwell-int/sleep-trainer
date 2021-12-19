#pragma once

#include "global.hpp"

#include <memory>
#include <iosfwd>

namespace sleep
{

class Pin;
class LedDescriptor;
class LedArray;
class WallClock;

class LedStrip
{
public:
  // Start by sending commands to have all leds off
  LedStrip(uint8_t const nbLeds, Pin const & pin, uint8_t const brightness = 5U);
  // Make sure all leds are off
  ~LedStrip();
  void addChange(LedDescriptor const & ledDescriptor);

  // Turn all leds off
  void shutdown();

  void lightLeds(LedArray const & leds);

  bool update(WallClock const & clock);

  bool hasActiveLeds() const;

  LedDescriptor const & getActiveLeds() const;

  static uint32_t const Black;
  static uint32_t const ColourDay;
  static uint32_t const ColourBeforeNight;
  static uint32_t const ColourNight;
  static uint32_t const ColourAfterNight;
  static uint32_t const ColourTarget;

  friend std::ostream& operator<< (std::ostream & stream, LedStrip const & ledStrip);
private:
  class Internal;

  std::unique_ptr< Internal > m_pimpl;
};

std::ostream & operator <<(std::ostream & stream, LedStrip const & ledStrip);
}
