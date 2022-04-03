#pragma once

#include <memory>
#include <iosfwd>

#include "global.hpp"

#include "Day.hpp"

namespace sleep
{

class Pin;
class LedDescriptor;
class LedArray;
class Clock;

class LedStrip
{
public:
  // Start by sending commands to have all leds off
  LedStrip(uint8_t const nbLeds, Pin const & pin, uint8_t const brightness = 5U);
  // Make sure all leds are off
  ~LedStrip();
  void addChange(LedDescriptor const & ledDescriptor, Day const day);

  // Turn all leds off
  void shutdown();

  void lightLeds(LedArray const & leds);

  bool update(Clock const & clock);

  bool hasActiveLeds() const;

  // Used for debugging so far
  LedDescriptor const & getActiveLeds() const;

  static uint32_t const Black;
  static uint32_t const ColourDay;
  static uint32_t const ColourBeforeNight;
  static uint32_t const ColourNight;
  static uint32_t const ColourAfterNight;
  static uint32_t const ColourTarget;
  static uint32_t const ColourNotConnected;
  static uint32_t const ColourConnected;
  static uint32_t const ColourError;

  friend std::ostream& operator<< (std::ostream & stream, LedStrip const & ledStrip);
private:
  class Internal;

  std::unique_ptr< Internal > m_pimpl;
};

std::ostream & operator <<(std::ostream & stream, LedStrip const & ledStrip);
}