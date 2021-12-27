#include "LedStrip.hpp"

#include <vector>
#include <ostream>

#include <Adafruit_NeoPixel.h> // install: Adafruit Neopixel

#include "WallClock.hpp"
#include "LedDescriptor.hpp"
#include "Pin.hpp"

namespace
{
void Fill(Adafruit_NeoPixel & strip, uint32_t const color, uint32_t const numLeds)
{
  for (int i = 0 ; i < numLeds ; ++i)
  {
    strip.setPixelColor(i, color);
  }
  strip.show();
}
}

namespace sleep
{

class LedStrip::Internal
{
public:
  Internal(uint8_t const nbLeds, Pin const & pin, uint8_t const brightness);
  std::vector< LedDescriptor > m_ledDescriptors;
  std::vector< LedDescriptor >::const_iterator m_currentStep;
  Adafruit_NeoPixel m_ledStrip;
};

uint32_t const LedStrip::Black = Adafruit_NeoPixel::Color(0, 0, 0);
uint32_t const LedStrip::ColourDay = Adafruit_NeoPixel::Color(0, 0, 0);
uint32_t const LedStrip::ColourBeforeNight = Adafruit_NeoPixel::Color(50, 30, 2);
uint32_t const LedStrip::ColourNight = Adafruit_NeoPixel::Color(15, 2, 40);
uint32_t const LedStrip::ColourAfterNight = Adafruit_NeoPixel::Color(2, 25, 50);
uint32_t const LedStrip::ColourTarget = Adafruit_NeoPixel::Color(20, 20, 1);

LedStrip::Internal::Internal(uint8_t const nbLeds, Pin const & pin, uint8_t const brightness)
  : m_ledStrip(nbLeds, pin.get(), NEO_GRB + NEO_KHZ800)
{
}

LedStrip::LedStrip(uint8_t const nbLeds, Pin const & pin, uint8_t const brightness)
  : m_pimpl(new LedStrip::Internal{nbLeds, pin, brightness})
{
  m_pimpl->m_ledStrip.begin();
  m_pimpl->m_ledStrip.show();
  m_pimpl->m_ledStrip.setBrightness(brightness);
  shutdown();
}

LedStrip::~LedStrip()
{
  shutdown();
}

void LedStrip::addChange(LedDescriptor const & ledDescriptor)
{
  m_pimpl->m_ledDescriptors.push_back(ledDescriptor);
  m_pimpl->m_currentStep = m_pimpl->m_ledDescriptors.cbegin();
}

void LedStrip::shutdown()
{
  Fill(m_pimpl->m_ledStrip, LedStrip::Black, m_pimpl->m_ledStrip.numPixels());
}

void LedStrip::lightLeds(LedArray const & leds)
{
  for (uint8_t i = 0 ; i < m_pimpl->m_ledStrip.numPixels() ; ++i)
  {
    m_pimpl->m_ledStrip.setPixelColor(i, leds.m_colours[i]);
  }
  m_pimpl->m_ledStrip.show();
}

bool LedStrip::update(WallClock const & clock)
{
  if (m_pimpl->m_ledDescriptors.empty())
  {
    return false;
  }
  size_t steps = 0;
  size_t const maxSteps = m_pimpl->m_ledDescriptors.size();
  bool updated = false;
  while (not m_pimpl->m_currentStep->contains(clock))
  {
    if (not updated)
    {
      updated = true;
    }
    if (maxSteps == steps)
    {
      shutdown();
      return true;
    }
    ++steps;
    ++m_pimpl->m_currentStep;
    if (m_pimpl->m_currentStep == m_pimpl->m_ledDescriptors.end())
    {
      m_pimpl->m_currentStep = m_pimpl->m_ledDescriptors.begin();
    }
  }
  if (updated)
  {
    lightLeds(m_pimpl->m_currentStep->m_leds);
  }
  return updated;
}

bool LedStrip::hasActiveLeds() const
{
  return (not m_pimpl->m_ledDescriptors.empty());
}

LedDescriptor const & LedStrip::getActiveLeds() const
{
  assert(not m_pimpl->m_ledDescriptors.empty());
  return *m_pimpl->m_currentStep;
}

std::ostream & operator <<(std::ostream & stream, LedStrip const & ledStrip)
{
  stream << "led descriptor with " << ledStrip.m_pimpl->m_ledDescriptors.size() << " steps\n";
  for (auto const& ledDescriptor: ledStrip.m_pimpl->m_ledDescriptors)
  {
    stream << ledDescriptor << "\n";
  }
  return stream;
}

}
