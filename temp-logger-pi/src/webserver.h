#ifndef _WEBSERVER_H_
#define _WEBSERVER_H_

void init_wifi();

String get_sensor_readings();

void setup_server();

void send_events_from_server();

String ArrayToJson(int numReg);

#endif