#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>
#include "sensors.h"
#include <PubSubClient.h>

#define Red 25
#define Green 26
#define Blue 27
#define Buzzer 13

#define PORT 1883
#define SSID  "Jeremy Consolidated"
#define PASSWORD  "HUAJOVP30"
#define MQTT_SERVER "10.50.172.213"

extern PubSubClient client;

void connect_wifi();
void reconnect_mqtt();
void initialize_and_publish();
void publish_sensor_values();
void callback(char* topic, byte* payload, unsigned int length);
void RGB_setup(char res);

#endif
