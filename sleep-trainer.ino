#line 2 "sleepTrainer.ino"

#include "global.hpp"

#include "Period.hpp"
#include "Time.hpp"
#include "WallClock.hpp"

#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <TimeLib.h>

#include <compare>
#include <sstream>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
int const START_HOURS = 19;
int const START_MINUTES = 0;
int const STOP_HOURS = 6;
int const STOP_MINUTES = 0;

int const NUM_LEDS = 15;


std::ostringstream STREAM;
uint32_t LOOPS;
uint32_t const MAX_LOOPS = 10;


namespace sleep
{
WallClock const NIGHT_START(19, 0);
WallClock const NIGHT_STOP(6, 0);

Minute const OFFSET_BEFORE(30);
Minute const OFFSET_AFTER(60);

WallClock const BEFORE_NIGHT(NIGHT_START - OFFSET_BEFORE);
WallClock const AFTER_NIGHT(NIGHT_STOP + OFFSET_AFTER);

static void PrintAndClearStream()
{
  Serial.println(STREAM.str().c_str());
  STREAM.str("");
  STREAM.clear();
}

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

bool isNight()
{
  int const hours = timeClient.getHours();
  int const minutes = timeClient.getMinutes();

  return ((hours == START_HOURS and minutes >= START_MINUTES) or (hours > START_HOURS))
         or ((hours == STOP_HOURS and minutes < STOP_MINUTES) or (hours < STOP_HOURS));
}

static void PrintDebug()
{
  timeClient.update();

  Time const now(timeClient.getEpochTime());

  Serial.println(timeClient.getFormattedTime());
  Serial.print("is night? ");
  Serial.println(isNight());
  STREAM << now.getPeriod();
  PrintAndClearStream();

  Minute const dayDuration = (BEFORE_NIGHT - AFTER_NIGHT);
  STREAM << "Day          : [ " << AFTER_NIGHT << " - " << BEFORE_NIGHT << " ] => " << dayDuration;
  PrintAndClearStream();
  Minute const beforeNightDuration = (NIGHT_START - BEFORE_NIGHT);
  STREAM << "Before night : [ " << BEFORE_NIGHT << " - " << NIGHT_START << " ] => " << beforeNightDuration;
  PrintAndClearStream();
  Minute const nightDuration = (NIGHT_STOP - NIGHT_START);
  STREAM << "Night        : [ " << NIGHT_START << " - " << NIGHT_STOP << " ] => " << nightDuration;
  PrintAndClearStream();
  Minute const afterNightDuration = (AFTER_NIGHT - NIGHT_STOP);
  STREAM << "After night  : [ ";
  STREAM << NIGHT_STOP;
  STREAM << " - ";
  STREAM << AFTER_NIGHT;
  STREAM << " ] => ";
  STREAM << afterNightDuration;
  PrintAndClearStream();
  double const factor = (double)nightDuration.get() / (double)(NUM_LEDS - 1);
  STREAM << "factor = " << factor;
  PrintAndClearStream();
  for (size_t i = 0 ; i < NUM_LEDS ; ++i)
  {
    STREAM << (NIGHT_START + Minute(i * factor)) << " : " << i << " LED(s)";
    PrintAndClearStream();
  }


  if (sleep::Period::Day == now.getPeriod())
  {

  }

  Serial.println(timeClient.getHours());
}

static sleep::Period GetPeriod(int const hours, int const minutes)
{
  if (((hours == START_HOURS and minutes >= START_MINUTES) or (hours > START_HOURS))
      or ((hours == STOP_HOURS and minutes < STOP_MINUTES) or (hours < STOP_HOURS)))
  {
    return sleep::Period::Night;
  }
  return sleep::Period::Day;
}

} // namespace sleep

void setup()
{
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // it is a good practice to make sure your code sets wifi mode how you want it.

  // put your setup code here, to run once:
  Serial.begin(115200);

  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;

  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
  //wm.resetSettings();

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result

  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  res = wm.autoConnect("AutoConnectAP", "password"); // password protected ap

  if (!res) {
    Serial.println("Failed to connect");
    // ESP.restart();
  }
  else {
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
    timeClient.begin();
  }

  sleep::PrintDebug();
}

void loop()
{
#ifdef RUN_TESTS
  sleep::Test();
#else // #ifdef RUN_TESTS
  timeClient.update();

  delay(1000);
#endif // #ifdef RUN_TESTS
  if (++LOOPS > MAX_LOOPS)
  {
    sleep::PrintDebug();
    LOOPS = 0;
  }
}
