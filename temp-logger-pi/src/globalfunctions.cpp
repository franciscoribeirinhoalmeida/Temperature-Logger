#include "rtctime.h"
#include "memory.h"
#include "globalvars.h"
#include <Arduino.h>

void serial_flush()
{
    while (Serial.available() > 0)
    {
        char t = Serial.read();
    }
}

void setup_rtc(uint32_t compilationUnix)
{
    uint32_t nowUnix;

    init_rtc();
    nowUnix = get_rtc_time();

    if (nowUnix < compilationUnix)
    {
        Serial.printf("rtc time(%d) older than compilation time(%d)\n", nowUnix, compilationUnix);
        set_rtc_time(compilationUnix);
    }
    else
    {
        Serial.printf("rtc time(%d) newer than compilation time(%d)\n", nowUnix, compilationUnix);
    }
}

int output(int mode)
{
    uint16_t regToRead = 0;
    int serialAscii;
    float temp;
    uint32_t unixTime;

    switch (mode)
    {
    case '1':
        return 1;

    case '2':
        wipe_memory();
        return 1;

    case '3':
        Serial.printf("value to read:\n");

        while (1)
        {
            if (Serial.available() > 0)
            {
                serialAscii = Serial.read();
                // Serial.printf("v: %d\n", serialAscii);

                if (serialAscii >= 48 && serialAscii <= 57)
                {
                    regToRead = (regToRead * 10) + (serialAscii - '0'); // convert ascii to int decimal
                    Serial.printf("%d", serialAscii - '0');
                }
                else if (serialAscii == '\n' || serialAscii == '\r')
                {
                    Serial.printf("\n");
                    if (regToRead > globalRegCount)
                    {
                        Serial.printf("value too large, please enter a lower address number\n");
                    }
                    else
                    {
                        read_reg(regToRead - 1, &temp, &unixTime);
                        Serial.printf("Reg: %d\n", regToRead);
                        Serial.printf("temp: %f\n", temp);
                        Serial.printf("rtc date: %d\n", unixTime);
                    }

                    // Serial.printf("%d\n", v);
                    serial_flush();
                    break;
                }
            }
        }
        return 0;

    default:
        return -1;
    }
}

void display_menu()
{
    int serialAscii;
    int mode;

    while (1)
    {
        Serial.printf("\n");
        Serial.printf(" ---------------------------\n");
        Serial.printf("| temp logger               |\n");
        Serial.printf("|                           |\n");
        Serial.printf("| 1 - append                |\n");
        Serial.printf("| 2 - wipe memory and write |\n");
        Serial.printf("| 3 - read value            |\n");
        Serial.printf(" --------------------------- \n");

        while (1)
        {
            if (Serial.available() > 0)
            {
                serialAscii = Serial.read();

                if (serialAscii >= 48 && serialAscii <= 57)
                {
                    Serial.printf("%d", serialAscii - '0');
                    mode = serialAscii;
                    // Serial.printf("char: %c\n", mode);
                    // Serial.printf("ascii: %d\n", mode);
                }
                else if (serialAscii == '\n' || serialAscii == '\r')
                {
                    serial_flush();
                    Serial.printf("\n");

                    if (output(mode) == 1)
                        return;
                    break;
                }
            }
        }
    }
}

int create_array_between_dates(int initDateUnix, int lastDateUnix)
{
    uint32_t unixTime = 0;
    float temp = 0;
    uint16_t initDateReg = 0;
    uint16_t lastDateReg = 0;
    int n = 0, cnt = 0;
    int numReg = 0;

    if (initDateUnix > lastDateUnix)
    {
        Serial.printf("initDateUnix > lastDateUnix\n");
        // return NULL;
    }

    while (unixTime < initDateUnix)
    {
        read_reg(initDateReg, &temp, &unixTime);
        if (initDateReg >= 65535)
            break;
        Serial.printf("time of Reg: %u, unix: %d +30\n", unixTime, initDateUnix);
        initDateReg += 30;
    }
    initDateReg -= 30;

    while (unixTime > initDateUnix)
    {
        initDateReg--;
        if (initDateReg <= 0)
            break;
        read_reg(initDateReg, &temp, &unixTime);
        Serial.printf("time of Reg: %u, unix: %d -1\n", unixTime, initDateUnix);
    }
    initDateReg++;

    lastDateReg = initDateReg;

    cnt = 0;
    while (unixTime < lastDateUnix)
    {
        if (cnt++ > 1000)
            break;

        lastDateReg++;

        if (lastDateReg >= 65535)
            break;

        read_reg(lastDateReg, &temp, &unixTime);
        if (unixTime == 4294967295)
            n++;

        if (n > 10)
        {
            n = 0;
            break;
        }
        Serial.printf("time of Reg: %u, unix: %d +1\n", unixTime, lastDateUnix);
    }
    lastDateReg--;

    numReg = lastDateReg - initDateReg + 1;

    Serial.printf("initDateReg: %d\n", initDateReg);
    Serial.printf("lastDateReg: %d\n", lastDateReg);

    tempArray = (float *)malloc(numReg * sizeof(float));
    timeArray = (uint32_t *)malloc(numReg * sizeof(uint32_t));

    for (int i = 0; i < numReg; i++)
    {
        read_reg(initDateReg + i, &temp, &unixTime);
        tempArray[i] = temp;
        timeArray[i] = unixTime;
    }

    return numReg;
}