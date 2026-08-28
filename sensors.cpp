#include <ArduinoJson.h>
#include "sensors.h"
#include <DHT.h>
#include <Wire.h>     //initializes the Wire library and joins the I2C bus as either a master (controller) or slave (peripheral) device
#include <SensirionI2cScd4x.h>  

#define DHTPIN 4
#define SDA_PIN 21
#define SCL_PIN 22
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
StaticJsonDocument<200> data; //create json object of size 200 bytes
SensirionI2cScd4x scd4x;  //creating an object

const char* name = "Room 1"; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! CHANGE FOR EACH NODE

void DHT11_values(){ //Declare and initialize key:value pairs of sensor data into the json object
  data["nodeName"] = name; 
  data["dht_hum"] = dht.readHumidity() - 7.5;
  data["dht_temp"] = dht.readTemperature() + 5.0;
}

void init_SCD40() {
  Wire.begin(SDA_PIN, SCL_PIN);  // SDA, SCL (wiring)

  //every I2C device has an address
  scd4x.begin(Wire, 0x62);  //SCD40 address (connected in the pins in Wire.begin)

  uint16_t error;
  error = scd4x.stopPeriodicMeasurement(); // Stops the continuous measuring of SCD40
  delay(100); // Small delay to allow proper measurement shut down
  error = scd4x.startPeriodicMeasurement(); // Restart measuring

  Serial.println("SCD40 Started...");
}

void SCD40_values(){
  uint16_t co2;
  float temperature;
  float humidity;

  uint16_t error;
  delay(5000);   // SCD40 updates every ~5 sec
  
  error = scd4x.readMeasurement(co2, temperature, humidity);  //Read and store the measurements into the variables, returns the reading status 
  if (error) {
      Serial.println("Error reading sensor");
      return;
  }

  // Add the sensor data to the Data json object
  data["scd_temp"] = temperature;
  data["scd_hum"] = humidity;
  data["scd_air"] = co2;
  
}

void data_to_send(char* output, size_t size){
  dht.begin(); 
  init_SCD40();
  DHT11_values();
  SCD40_values();
  serializeJson(data, output, size); // Flattens the json data object into text able to send over mqtt
}
