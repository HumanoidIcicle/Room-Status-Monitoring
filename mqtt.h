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
#define SSID  "SSID_NAME"
#define PASSWORD  "PASSWORD"
#define MQTT_SERVER "SERVER_NAME"

extern PubSubClient client;

void connect_wifi();
void reconnect_mqtt();
void initialize_and_publish();
void publish_sensor_values();
void callback(char* topic, byte* payload, unsigned int length);
void RGB_setup(char res);

#endif
