#ifndef _RTCTIME_H_
#define _RTCTIME_H_

#include <Arduino.h>

void init_rtc();

void set_rtc_time(uint32_t newTime);

uint32_t get_rtc_time();

#endif