#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

void init_scd40();
void DHT11_values();
void SCD40_values();
void data_to_send(char* output, size_t size);

#endif
