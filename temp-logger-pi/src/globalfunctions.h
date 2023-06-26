#ifndef _GLOBALFUNCTIONS_H_
#define _GLOBALFUNCTIONS_H_

void setup_rtc(uint32_t compilationUnix);

void display_menu();

int output(int ascii);

void serial_flush();

void init_wifi();

int create_array_between_dates(int initDateUnix, int lastDateUnix);

#endif