#include "mqtt.h"
#include "sensors.h"
#include <esp_sleep.h>


void setup() {
  Serial.begin(115200);

  pinMode(Red, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Blue, OUTPUT);
  connect_wifi();
}

void loop() {
  reconnect_mqtt();
  initialize_and_publish();
  Serial.println("Light sleep 4 sec...");

  esp_sleep_enable_timer_wakeup(4 * 1000000ULL); //INPUT THE LIGHT SLEEP DURATION
  esp_light_sleep_start();
  Serial.println("Woke up!");
}
