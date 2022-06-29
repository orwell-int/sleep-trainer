#line 2 "sleepTrainer.ino"

#include "global.hpp"

#include "Period.hpp"
//#include "Time.hpp"
#include "Clock.hpp"
#include "LedStrip.hpp"
#include "Pin.hpp"
#include "Interval.hpp"
#include "LedArray.hpp"
#include "LedDescriptor.hpp"

#include <TZ.h> // included in core
#include <sntp.h> // included in core
#include <NTPClient.h> // install: NTPClient
#include <WiFiUdp.h>
#include <WiFiManager.h> // install: WiFiManager tzapu ; https://github.com/tzapu/WiFiManager
#include <TimeLib.h> // install: Time
// #include <Adafruit_NeoPixel.h> // install: Adafruit Neopixel

#include <compare>
#include <sstream>

#define NTP_SERVER_A "pool.ntp.org"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER_A);

int const LED_PIN = 4;
int const BUTTON_PIN = 5;

#ifdef STREAM_DEBUG
std::ostringstream STREAM;
#endif // #ifdef STREAM_DEBUG
uint32_t LOOPS;
uint32_t const MAX_LOOPS = 10;

namespace sleep
{

LedStrip LED_STRIP(NUM_LEDS, Pin(LED_PIN), 255U);

#ifdef STREAM_DEBUG
static void PrintAndClearStream()
{
  Serial.println(STREAM.str().c_str());
  STREAM.str("");
  STREAM.clear();
}
#endif // #ifdef STREAM_DEBUG

#ifdef RUN_TESTS

static void Test()
{
  Hour::Test();
  Minute::Test();
  Period::Test();
  Time::Test();
  WallClock::Test();
}

#endif // #ifdef RUN_TESTS

#ifdef STREAM_DEBUG
static void SillyTest()
{
  sleep::WallClock c1(11, 0);
  sleep::WallClock c2(12, 0);
  sleep::WallClock c3(13, 0);
  sleep::Interval i1(c1, c2);
  sleep::Interval i2(c2, c1);
  Serial.print("durations: ");
  Serial.print(i1.duration().get());
  Serial.print(" ");
  Serial.println(i2.duration().get());
  if (sleep::Minute(60) == i1.duration())
  {
    STREAM << "1";
  }
  else
  {
    STREAM << "0";
  }
  if (sleep::Minute(60 * 23) == i2.duration())
  {
    STREAM << "1";
  }
  else
  {
    STREAM << "0";
  }
  sleep::PrintAndClearStream();
  if (i1.contains(c1))
  {
    STREAM << "1";
  }
  else
  {
    STREAM << "0";
  }
  if (i1.contains(c2))
  {
    STREAM << "1";
  }
  else
  {
    STREAM << "0";
  }
  if (i1.contains(c3))
  {
    STREAM << "1";
  }
  else
  {
    STREAM << "0";
  }
  STREAM << "\n";
  if (i2.contains(c1))
  {
    STREAM << "1";
  }
  else
  {
    STREAM << "0";
  }
  if (i2.contains(c2))
  {
    STREAM << "1";
  }
  else
  {
    STREAM << "0";
  }
  if (i2.contains(c3))
  {
    STREAM << "1";
  }
  else
  {
    STREAM << "0";
  }
  sleep::PrintAndClearStream();
}
#endif // #ifdef STREAM_DEBUG

enum class NTP
{
  kCore,
  kClient
};

unsigned long getEpoch(NTP const ntp)
{
  switch (ntp)
  {
    case NTP::kCore:
    {
      return sntp_get_current_timestamp();
    }
    case NTP::kClient:
    {
      return timeClient.getEpochTime();
    }
    default:
    {
      assert(false);
    }
  }
}

static void PrintClocks()
{
  Serial.print("core clock: ");
  Clock(getEpoch(NTP::kCore)).print();
  Serial.println("");
  Serial.print("client clock: ");
  Clock(getEpoch(NTP::kClient)).print();
  Serial.println("");
}

// Day: all dark
//      .....
// Before night: from all leds to one led
//      11111
//      1111.
//      111..
//      11...
//      1....
// Night: last led always on ; from 0 to all but last two leds
//      ....2
//      3...2
//      33..2
//      333.2
// After night: all leds on
//      44444
static void SetupLedStrip()
{
  Serial.println("SetupLedStrip");
  for (Day const day: Week())
  {
#ifdef STREAM_DEBUG
    STREAM << "Configure " << day;
    PrintAndClearStream();
#endif // #ifdef STREAM_DEBUG

    Clock const & dayBegin = Clock::Get(Period::Day, day);
#ifdef STREAM_DEBUG
    STREAM << "SetupLedStrip dayBegin = " << dayBegin;
    PrintAndClearStream();
#endif // #ifdef STREAM_DEBUG
    Clock const & beforeNightBegin = Clock::Get(Period::BeforeNight, day);
#ifdef STREAM_DEBUG
    STREAM << "SetupLedStrip beforeNightBegin = " << beforeNightBegin;
    PrintAndClearStream();
#endif // #ifdef STREAM_DEBUG
    Clock const & nightBegin = Clock::Get(Period::Night, day);
#ifdef STREAM_DEBUG
    STREAM << "SetupLedStrip nightBegin = " << nightBegin;
    PrintAndClearStream();
#endif // #ifdef STREAM_DEBUG
    Clock const & afterNightBegin = Clock::Get(Period::AfterNight, day);
#ifdef STREAM_DEBUG
    STREAM << "SetupLedStrip afterNightBegin = " << afterNightBegin;
    PrintAndClearStream();
#endif // #ifdef STREAM_DEBUG

    Interval const & beforeNight = Interval::Get(Period::BeforeNight, day);
    Interval const & night = Interval::Get(Period::Night, day);

    double const factorBeforeNight = (double)beforeNight.duration().get() / (double)(NUM_LEDS);
#ifdef STREAM_DEBUG
    STREAM << "factorBeforeNight = " << factorBeforeNight;
#endif // #ifdef STREAM_DEBUG
    double const factorNight = (double)night.duration().get() / (double)(NUM_LEDS - 1);
#ifdef STREAM_DEBUG
    STREAM << " ; factorNight = " << factorNight;
    PrintAndClearStream();
#endif // #ifdef STREAM_DEBUG
    WallClock lastClock = beforeNightBegin;
    LedArray leds;
    Interval interval(dayBegin, beforeNightBegin);
    LED_STRIP.addChange(LedDescriptor(interval, leds), day);
#ifdef STREAM_DEBUG
    STREAM << interval << " : " << 0 << " LED(s)" << leds;
    PrintAndClearStream();
#endif // #ifdef STREAM_DEBUG
    leds.fill(LedStrip::ColourBeforeNight);
    for (size_t i = 0 ; i < NUM_LEDS ; ++i)
    {
      WallClock const clock = beforeNightBegin + Minute((i + 1) * factorBeforeNight);
      interval = Interval(lastClock, clock);
#ifdef STREAM_DEBUG
      STREAM << interval << " : " << (NUM_LEDS - 1) << " LED(s)" << leds;
      PrintAndClearStream();
#endif // #ifdef STREAM_DEBUG
      LED_STRIP.addChange(LedDescriptor(interval, leds), day);
      lastClock = clock;
      leds.set(NUM_LEDS - (i + 1), LedStrip::Black);
    }
    leds.clear();
    leds.set(NUM_LEDS - 1, LedStrip::ColourTarget);
    lastClock = nightBegin;
    for (size_t i = 0 ; i < NUM_LEDS - 1 ; ++i)
    {
      WallClock const clock = nightBegin + Minute((i + 1) * factorNight);
      interval = Interval(lastClock, clock);
#ifdef STREAM_DEBUG
      STREAM << interval << " : " << i << " LED(s)" << leds;
      PrintAndClearStream();
#endif // #ifdef STREAM_DEBUG
      LED_STRIP.addChange(LedDescriptor(interval, leds), day);
      lastClock = clock;
      leds.set(i, LedStrip::ColourNight);
    }
    leds.fill(LedStrip::ColourAfterNight);
    interval = Interval(afterNightBegin, dayBegin);
    LED_STRIP.addChange(LedDescriptor(interval, leds), day);
#ifdef STREAM_DEBUG
    STREAM << interval << " : " << (int)NUM_LEDS << " LED(s)" << leds;
    PrintAndClearStream();
#endif // #ifdef STREAM_DEBUG
  }
#ifdef STREAM_DEBUG
  STREAM << "LED_STRIP:\n" << LED_STRIP;
  PrintAndClearStream();
#endif // #ifdef STREAM_DEBUG
}

static void Demo()
{
  Serial.println("Demo start");
  // 72s -> 72000ms
  timeClient.update();
  sleep::Clock const clock(timeClient.getEpochTime());
  // Run the demo for current day
  sleep::Day const day = clock.day();

  // Duration of the demo (real time)
  unsigned long demoDurationMillis = 20 * 1000;

  unsigned long nowMillis = 0;
  unsigned long deltaMillis = 0;
  int deltaFakeMinutes = 0;
  unsigned long const startMillis = millis();
  unsigned long const endMillis = startMillis + demoDurationMillis;
  // Simulated begin time
  Clock const beginDemo = Clock::Get(Period::BeforeNight, day) - Minute(60);
  // Simulated end time
  Clock const endDemo = Clock::Get(Period::Day, day) + Minute(60);
  int const beginDemoMinutes = beginDemo.totalMinutes().get();

  Serial.print("beginDemo: ");
  beginDemo.print();
  Serial.println("");
  Serial.print("endDemo: ");
  endDemo.print();
  Serial.println("");
  Serial.print("demo duration: ");
  int demoDuration = (endDemo - beginDemo).get();
  Serial.println(demoDuration);
  Serial.print("demo duration ms: ");
  Serial.println(demoDurationMillis);

  double const factor = (double)demoDuration / (double)demoDurationMillis;
  Serial.print("factor: ");
  Serial.println(factor);

  Serial.print("begin: ");
  Serial.print(startMillis);
  Serial.println(" ms");
  Serial.print("end: ");
  Serial.print(endMillis);
  Serial.println(" ms");
  Clock fakeClock(beginDemo);
  do
  {
    delay(100);
    nowMillis = millis();
    deltaMillis = nowMillis - startMillis;
    deltaFakeMinutes = deltaMillis * factor;
    fakeClock = Clock(Minute(beginDemoMinutes + deltaFakeMinutes), day);
    if (LED_STRIP.update(fakeClock))
    {
#ifdef STREAM_DEBUG
      STREAM << "At " << nowMillis << "ms " << LED_STRIP.getActiveLeds();
      PrintAndClearStream();
#endif // #ifdef STREAM_DEBUG
    }
  } while (nowMillis < endMillis);
}

} // namespace sleep

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // it is a good practice to make sure your code sets wifi mode how you want it.
  // Add a delay to make sure that the connection is open properly
  delay(1000);
  Serial.println("START");
#ifdef STREAM_DEBUG
  sleep::SillyTest();
#endif // #ifdef STREAM_DEBUG
  sleep::SetupLedStrip();

  sleep::LedArray leds;
  leds.fill(sleep::LedStrip::ColourNotConnected);
  sleep::LED_STRIP.lightLeds(leds);

  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;

  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
  //wm.resetSettings();

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result


  wm.setConfigPortalTimeout(60);
  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  res = wm.autoConnect("AutoConnectAP", "password"); // password protected ap

  timeClient.begin();

  if (!res)
  {
    Serial.println("Failed to connect");
    // ESP.restart();
  }
  else
  {
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");

    leds.fill(sleep::LedStrip::ColourConnected);
    sleep::LED_STRIP.lightLeds(leds);
    delay(750);
    leds.fill(sleep::LedStrip::Black);
    sleep::LED_STRIP.lightLeds(leds);
  }

  //This could even take two additional pool servers
  configTime(TZ_Europe_Paris, NTP_SERVER_A);

  timeClient.update();
  sleep::Clock const clock(timeClient.getEpochTime());
  bool const isDay = clock.getPeriod() == sleep::Period::Day;
#ifdef STREAM_DEBUG
  STREAM << "Period in setup: " << clock.getPeriod();
  sleep::PrintAndClearStream();
#endif // #ifdef STREAM_DEBUG

  Serial.print("day: ");
  PrintDay(clock.day());
  Serial.println("");

  sleep::PrintClocks();

  if (isDay)
  {
    // show a demo
    sleep::Demo();
  }
  // Button setup
  pinMode(BUTTON_PIN, INPUT);
}

unsigned long PREVIOUS_TIME = 0;
unsigned int ERRORS = 0;
unsigned int const MAX_ERRORS = pow(2, NUM_LEDS + 1) - 1;
bool SHOW_DEMO = false;
unsigned long DEMO_END(0);
unsigned long DEMO_TIME(0);
unsigned long DEMO_STEP(5 * sleep::StrictMinute::Seconds + 1);
unsigned int DEMO_BUTTON_PRESSES(0);

void loop()
{
#ifdef RUN_TESTS
  sleep::Test();
#else // #ifndef RUN_TESTS

  sleep::NTP const ntp = sleep::NTP::kCore;
  if (SHOW_DEMO)
  {
    DEMO_BUTTON_PRESSES -= 1;
    DEMO_TIME += DEMO_STEP;
    if (DEMO_TIME > DEMO_END)
    {
      SHOW_DEMO = false;
      Serial.println("Week demo finish");
    }
    else
    {
      delay(100);
    }
  }
  if (not SHOW_DEMO)
  {
    delay(1000);
    timeClient.update();
  }
  unsigned long time = SHOW_DEMO ? DEMO_TIME : sleep::getEpoch(ntp);
  sleep::Clock const clock(time);
  if (not SHOW_DEMO)
  {
    if (time < PREVIOUS_TIME)
    {
      ++ERRORS;
      if (ERRORS > MAX_ERRORS)
      {
        ERRORS = 1;
      }
      if (sleep::Period::Day == clock.getPeriod())
      {
        sleep::LedArray leds;
        int errors = ERRORS;
        int index = 0;
        while (errors > 0)
        {
          if (errors % 2 == 1)
          {
            leds.set(index, sleep::LedStrip::ColourError);
          }
          errors /= 2;
          ++index;
        }
        sleep::LED_STRIP.lightLeds(leds);
      }
    }
    int reading = digitalRead(BUTTON_PIN);
    if (reading == HIGH /*and clock.getPeriod() == sleep::Period::Day*/)
    {
      Serial.println("Week demo start");
      DEMO_BUTTON_PRESSES = 0;
      SHOW_DEMO = true;
      DEMO_TIME = time;
      DEMO_END = DEMO_TIME +
        (sleep::StrictMinute::WeekDuration
          * sleep::StrictMinute::Seconds).get();
      Serial.print("DEMO_TIME = ");
      Serial.println(DEMO_TIME);
      Serial.print("DEMO_END  = ");
      Serial.println(DEMO_END);
    }
    if (++LOOPS > MAX_LOOPS)
    {
      Serial.println("bip");
      LOOPS = 0;
    }
  }
  else
  {
    int reading = digitalRead(BUTTON_PIN);
    if (reading == HIGH)
    {
      DEMO_BUTTON_PRESSES += 100;
      Serial.print("DEMO_BUTTON_PRESSES: ");
      Serial.println(DEMO_BUTTON_PRESSES);
      if (DEMO_BUTTON_PRESSES > 300)
      {
        ESP.restart();
      }
    }
  }
  if (sleep::LED_STRIP.update(clock))
  {
    Serial.print("Change leds at ");
    clock.print();
    Serial.print(" (");
    sleep::Print(clock.getPeriod());
    Serial.println(")");
    if (not SHOW_DEMO)
    {
      sleep::PrintClocks();
    }
  }
#endif // #ifndef RUN_TESTS
}
