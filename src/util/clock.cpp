#include "clock.hpp"
#include "Arduino.h"
#include <cstdint>

/* constants */
static constexpr uint8_t CLOCK_RESET_PIN = 4U;
static constexpr uint8_t CLOCK_DATA_PIN = 5U;
static constexpr uint8_t CLOCK_CLK_PIN = 6U;

ThreeWire myWire(CLOCK_DATA_PIN, CLOCK_CLK_PIN, CLOCK_RESET_PIN);

RtcDS1302<ThreeWire> Rtc(myWire);

static RtcDateTime now;

namespace util::clock {

CustomClock::CustomClock() {}

void CustomClock::setup() {
    Serial.printf("%s\n", __PRETTY_FUNCTION__);
    Rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    printDateTime(compiled);
    Serial.println();

    if (!Rtc.IsDateTimeValid()) {
        // Common Causes:
        //    1) first time you ran and the device wasn't running yet
        //    2) the battery on the device is low or even missing

        Serial.println("RTC lost confidence in the DateTime!");
        Rtc.SetDateTime(compiled);
    }
    if (Rtc.GetIsWriteProtected()) {
        Serial.println("RTC was write protected, enabling writing now");
        Rtc.SetIsWriteProtected(false);
    }

    if (!Rtc.GetIsRunning()) {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled) {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    } else if (now > compiled) {
        Serial.println("RTC is newer than compile time. (this is expected)");
    } else if (now == compiled) {
        Serial.println(
            "RTC is the same as compile time! (not expected but all is fine)");
    }
}

bool CustomClock::getTime(RtcDateTime &now) {
    bool valid = Rtc.IsDateTimeValid();

    if (valid) {
        now = Rtc.GetDateTime();
    }

    return valid;
}

bool CustomClock::setTime(RtcDateTime &dt) {
    Rtc.SetDateTime(dt);
    
    return Rtc.IsDateTimeValid();
}

void CustomClock::printDateTime(const RtcDateTime &dt) {
    char datestring[26];

    snprintf_P(datestring, countof(datestring),
               PSTR("%02u/%02u/%04u %02u:%02u:%02u\n"), dt.Month(), dt.Day(),
               dt.Year(), dt.Hour(), dt.Minute(), dt.Second());
    Serial.print(datestring);
}

void CustomClock::setInternalDateTime(RtcDateTime &now) {
    tm now_tm;
    now_tm.tm_year = now.Year() - 1900;
    now_tm.tm_mon = now.Month() - 1;
    now_tm.tm_mday = now.Day();
    now_tm.tm_hour = now.Hour();
    now_tm.tm_min = now.Minute();
    now_tm.tm_sec = now.Second();
    time_t t = mktime(&now_tm);
    struct timeval now_timeval = {.tv_sec = t};
    Serial.printf("set time OK %d\n",
                  settimeofday((const timeval *)&now_timeval, NULL));
}

void CustomClock::printInternalDateTime() {
    time_t now = time(0);

    // Convert now to tm struct for local timezone
    tm *localtm = localtime(&now);

    printf("The local date and time is: %s", asctime(localtm));
}

time_t CustomClock::getTimeFromProgramDate(riego::data::ProgramDate &progDate) {
    tm now_tm;

    now_tm.tm_year = progDate.year - 1900;
    now_tm.tm_mon = progDate.month - 1;
    now_tm.tm_mday = progDate.day;
    now_tm.tm_hour = 0;
    now_tm.tm_min = 0;
    now_tm.tm_sec = 0;
    return mktime(&now_tm);
}

time_t CustomClock::getTimeNowToValveTime(riego::data::ValveTime &valveTime) {
    time_t intNow;
    struct tm *now_tm;

    intNow = time(NULL);
    now_tm = localtime(&intNow);

    now_tm->tm_hour = valveTime.hour;
    now_tm->tm_min = valveTime.minute;
    now_tm->tm_sec = 0;

    return mktime(now_tm);
}

} // namespace util::clock
