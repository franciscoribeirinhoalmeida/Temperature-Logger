#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <WiFi.h>
#include "rtctime.h"
#include "temp.h"
#include "globalvars.h"
#include "globalfunctions.h"
#include "memory.h"
#include "webserver.h"

void setup()
{
    WiFiClass WiFi;

    DateTime CompilationDate = DateTime(F(__DATE__), F(__TIME__));

    uint32_t compilationUnix = CompilationDate.unixtime();

    Serial.begin(115200);
    delay(1000);

    setup_rtc(compilationUnix);

    setup_memory();

    setup_server();

    // wipe_memory();

    // display_menu();

    /* for (int i = 1000; i < 1500; i++)
    {
        read_reg(i, &globalTemp, &globalUnixTime);
    } */
}

void loop()
{
    // read temp
    globalTemp = get_temp();
    Serial.printf("temp: %f\n", globalTemp);

    // get timestamp
    globalUnixTime = get_rtc_time();
    Serial.printf("rtc date: %d\n", globalUnixTime);

    // write reg to mem
    write_reg(globalRegCount, &globalTemp, &globalUnixTime);

    // debug
    read_reg(globalRegCount, &globalTemp, &globalUnixTime);
    Serial.printf("memory temp: %f\n", globalTemp);
    Serial.printf("memory date: %d\n", globalUnixTime);

    // regcnt++
    globalRegCount++;

    // write regcnt to mem
    write_reg_count(globalRegCount);
    globalRegCount = read_reg_count();
    Serial.printf("regcount: %d\n", globalRegCount);

    send_events_from_server();

    // sleep 10sec
    Serial.printf("----------------------------\n");
    delay(10000);
}

/*
void setup()
{
    Serial.begin(115200);
    // Set ESP32 wifi to station mode
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    Serial.println("Setup done");
}
void loop()
{
    Serial.println("scan start");
    // WiFi.scanNetworks will give total found wifi networks
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0)
    {
        Serial.println("no networks found");
    }
    else
    {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i)
        {
            // Print SSID and signal strength
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
            delay(10);
        }
    }
    Serial.println("");
    // delay of 5 sec
    delay(5000);
}





void setup()
{
    Serial.begin(115200);

    // Set WiFi to station mode and disconnect from an AP if it was previously connected.
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    Serial.println("Setup done");
}

void loop()
{
    Serial.println("Scan start");

    // WiFi.scanNetworks will return the number of networks found.
    int n = WiFi.scanNetworks();
    Serial.println("Scan done");
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        Serial.print(n);
        Serial.println(" networks found");
        Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            Serial.printf("%2d",i + 1);
            Serial.print(" | ");
            Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
            Serial.print(" | ");
            Serial.printf("%4d", WiFi.RSSI(i));
            Serial.print(" | ");
            Serial.printf("%2d", WiFi.channel(i));
            Serial.print(" | ");
            switch (WiFi.encryptionType(i))
            {
            case WIFI_AUTH_OPEN:
                Serial.print("open");
                break;
            case WIFI_AUTH_WEP:
                Serial.print("WEP");
                break;
            case WIFI_AUTH_WPA_PSK:
                Serial.print("WPA");
                break;
            case WIFI_AUTH_WPA2_PSK:
                Serial.print("WPA2");
                break;
            case WIFI_AUTH_WPA_WPA2_PSK:
                Serial.print("WPA+WPA2");
                break;
            case WIFI_AUTH_WPA2_ENTERPRISE:
                Serial.print("WPA2-EAP");
                break;
            case WIFI_AUTH_WPA3_PSK:
                Serial.print("WPA3");
                break;
            case WIFI_AUTH_WPA2_WPA3_PSK:
                Serial.print("WPA2+WPA3");
                break;
            case WIFI_AUTH_WAPI_PSK:
                Serial.print("WAPI");
                break;
            default:
                Serial.print("unknown");
            }
            Serial.println();
            delay(10);
        }
    }
    Serial.println("");

    // Delete the scan result to free memory for code below.
    WiFi.scanDelete();

    // Wait a bit before scanning again.
    delay(5000);
}
*/
