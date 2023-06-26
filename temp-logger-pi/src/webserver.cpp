#include <Arduino.h>
#include <WiFi.h>
#include <string.h>
#include <SPIFFS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "globalvars.h"
#include "globalfunctions.h"
#include "memory.h"

// python3 -m http.server

const char *ssid = "";
const char *password = "";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

// Json Variable to Hold Sensor Readings
StaticJsonDocument<200> realTimeReadings;
StaticJsonDocument<200> startAndEndDates;
DynamicJsonDocument TemperatureArray(5000);
// JSONVar TemperatureArray;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

String ArrayToJson(int numReg)
{
    String json;

    for (int i = 0; i < numReg; i++)
    {
        TemperatureArray["temperature_array"][i] = tempArray[i];
        TemperatureArray["timestamp_array"][i] = timeArray[i];
    }

    serializeJson(TemperatureArray, json);

    return json;
}

// Get Sensor Readings and return JSON object
String curr_temp_to_json()
{
    String json;

    realTimeReadings["current_temperature"] = String(globalTemp);

    serializeJson(realTimeReadings, json);

    return json;
}

void init_wifi()
{
    WiFi.mode(WIFI_AP);
    WiFi.begin(ssid, password);
    // WiFi.softAP(ssid, password);
    Serial.println("Connecting to WiFi..");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }
    Serial.println(WiFi.localIP());
    // Serial.println(WiFi.softAPIP());
}

void setup_server()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    init_wifi();

    // Web Server Root URL
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/index.html", "text/html"); });
    server.serveStatic("/", SPIFFS, "/");

    // Request for the latest sensor readings
    server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    String json = curr_temp_to_json();
    request->send(200, "application/json", json);
    json = String(); });

    // Wipe memory upon request
    server.on("/wipe", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    wipe_memory();
    request->send(200, "text/plain", "memory wiped"); });

    server.on(
        "/dates", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL,
        [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
        {
            Serial.printf("data: %s\n", (char *)data);
            DeserializationError error = deserializeJson(startAndEndDates, data);

            if (!error)
            {
                uint32_t startDate = startAndEndDates["start_date_unix"];
                uint32_t endDate = startAndEndDates["end_date_unix"];
                Serial.printf("start date: %u\n", startDate);
                Serial.printf("end date: %u\n", endDate);

                String jsonArray;
                jsonArray = ArrayToJson(create_array_between_dates(startDate, endDate));

                request->send(200, "application/json", jsonArray);
            }
            else
            {
                Serial.println("JSON parsing failed!");
                request->send(400, "text/plain", "Invalid JSON data");
            } });

    events.onConnect([](AsyncEventSourceClient *client)
                     {
            if (client->lastId())
                {
                    Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
                }
                // send event with message "hello!", id current millis
                // and set reconnect delay to 1 second
            client->send("", NULL, millis(), 1000); });
    server.addHandler(&events);

    // Start server
    server.begin();
}

void send_events_from_server()
{
    if ((millis() - lastTime) > timerDelay)
    {
        // Send Events to the client with the Sensor Readings Every 10 seconds
        events.send("ping", NULL, millis());
        events.send(curr_temp_to_json().c_str(), "new_readings", millis());
        lastTime = millis();
    }
}
