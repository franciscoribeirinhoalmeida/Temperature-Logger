#include <RTClib.h>
#include "globalvars.h"
#include "rtctime.h"

RTC_DS3231 Rtc;

void init_rtc()
{
    if (!Rtc.begin())
    {
        Serial.println("Couldn't find RTC");

        Serial.flush();

        while (1)
            delay(10);
    }
}

uint32_t get_rtc_time()
{
    DateTime now = Rtc.now();
    uint32_t nowUnix = now.unixtime();

    return nowUnix;
}

void set_rtc_time(uint32_t newTime)
{
    Rtc.adjust(newTime);
}