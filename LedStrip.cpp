#include "LedStrip.hpp"

#include <vector>
#include <map>
#include <ostream>

#include <Adafruit_NeoPixel.h> // install: Adafruit Neopixel

#include "Clock.hpp"
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
  std::map< Day, std::vector< LedDescriptor > > m_ledDescriptors;
  std::map< Day, std::vector< LedDescriptor >::const_iterator > m_currentStep;
  Day m_lastDay;
  bool m_started;
  Adafruit_NeoPixel m_ledStrip;
};

uint32_t const MaxLight = 6;
double const IntensityFactor = 0.5 * 255 / MaxLight;

uint32_t MakeColor(uint8_t r, uint8_t g, uint8_t b)
{
  r *= IntensityFactor;
  g *= IntensityFactor;
  b *= IntensityFactor;
  return Adafruit_NeoPixel::Color(0, 0, 0);
}

uint32_t const LedStrip::Black = MakeColor(0, 0, 0);
uint32_t const LedStrip::ColourDay = MakeColor(0, 0, 0);
uint32_t const LedStrip::ColourBeforeNight = MakeColor(4, 3, 0);
uint32_t const LedStrip::ColourNight = MakeColor(1, 0, 2);
uint32_t const LedStrip::ColourAfterNight = MakeColor(0, 6, 1);
uint32_t const LedStrip::ColourTarget = MakeColor(2, 2, 0);
uint32_t const LedStrip::ColourNotConnected = MakeColor(5, 1, 0);
uint32_t const LedStrip::ColourConnected = MakeColor(1, 5, 0);
uint32_t const LedStrip::ColourError = MakeColor(4, 0, 0);

//uint32_t const LedStrip::ColourNight = Adafruit_NeoPixel::Color(100, 0, 200);
//uint32_t const LedStrip::ColourAfterNight = Adafruit_NeoPixel::Color(0, 250, 40);
//uint32_t const LedStrip::ColourTarget = Adafruit_NeoPixel::Color(200, 200, 0);
//uint32_t const LedStrip::ColourNotConnected = Adafruit_NeoPixel::Color(255, 120, 120);
//uint32_t const LedStrip::ColourConnected = Adafruit_NeoPixel::Color(120, 255, 120);
//uint32_t const LedStrip::ColourError = Adafruit_NeoPixel::Color(255, 0, 0);


LedStrip::Internal::Internal(uint8_t const nbLeds, Pin const & pin, uint8_t const brightness)
  : m_ledStrip(nbLeds, pin.get(), NEO_GRB + NEO_KHZ800)
  , m_started(false)
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

void LedStrip::addChange(LedDescriptor const & ledDescriptor, Day const day)
{
  m_pimpl->m_ledDescriptors[day].push_back(ledDescriptor);
  m_pimpl->m_currentStep[day] = m_pimpl->m_ledDescriptors[day].cbegin();
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

bool LedStrip::update(Clock const & clock)
{
  Day const day = clock.day();
  if (m_pimpl->m_ledDescriptors[day].empty())
  {
    return false;
  }
  size_t steps = 0;
  if ((m_pimpl->m_started) and (day != m_pimpl->m_lastDay))
  {
    m_pimpl->m_currentStep[m_pimpl->m_lastDay] =
      m_pimpl->m_ledDescriptors[m_pimpl->m_lastDay].cbegin();
    m_pimpl->m_lastDay = day;
  }
  size_t const maxSteps = m_pimpl->m_ledDescriptors[day].size();
  bool updated = false;
  while (not m_pimpl->m_currentStep[day]->contains(clock))
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
    ++m_pimpl->m_currentStep[day];
    if (m_pimpl->m_currentStep[day] == m_pimpl->m_ledDescriptors[day].end())
    {
      m_pimpl->m_currentStep[day] = m_pimpl->m_ledDescriptors[day].begin();
    }
  }
  if (updated)
  {
    lightLeds(m_pimpl->m_currentStep[day]->m_leds);
  }
  return updated;
}

bool LedStrip::hasActiveLeds() const
{
  return (not m_pimpl->m_ledDescriptors.empty());
}

LedDescriptor const & LedStrip::getActiveLeds() const
{
  assert(not m_pimpl->m_ledDescriptors.at(m_pimpl->m_lastDay).empty());
  return *m_pimpl->m_currentStep.at(m_pimpl->m_lastDay);
}

std::ostream & operator <<(std::ostream & stream, LedStrip const & ledStrip)
{
  stream << "led descriptor with " << ledStrip.m_pimpl->m_ledDescriptors.size() << " days\n";
  for (auto const & mapItem: ledStrip.m_pimpl->m_ledDescriptors)
  {
    Day const & day = mapItem.first;
    stream << "  " << day << " with " << mapItem.second.size() << " days\n";
    for (auto const & ledDescriptor: mapItem.second)
    {
      stream << "    " << ledDescriptor << "\n";
    }
  }
  return stream;
}

}