#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "mqtt.h"
#include "sensors.h" //
#include <Wire.h>     //initializes the Wire library and joins the I2C bus as either a master (controller) or slave (peripheral) device
#include <SensirionI2cScd4x.h> 


const char* topic = "sensor/nodes"; // NAME OF THE TOPIC
const char* results = "results/room1";  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! CHANGE NAME FOR EACH NODE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

WiFiClient sensorNode;
PubSubClient client(sensorNode);

void connect_wifi(){
  Serial.println("Connecting WiFi...");
  WiFi.begin(SSID, PASSWORD);

  while(WiFi.status() != WL_CONNECTED){ //keep trying to connect to wifi
    Serial.print(".");
    delay(100);
  }
  Serial.println("Connected to WiFi.");
}

void reconnect_mqtt(){
  client.setServer(MQTT_SERVER, PORT);
  client.setCallback(callback);
  Serial.println("Connecting MQTT...");
  
  while(!client.connected()){ //keep trying until connected to mqtt

    if(client.connect("room1")){ //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! CHANGE FOR EACH NODE
      Serial.println("MQTT connected.");
      client.subscribe(results);
    } 
    else { //Print status code for reason for failed connection
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again");
      delay(500);
    }
  }
}

void RGB_setup(char res){
  if(res == '1'){
    analogWrite(Red, 0);
    analogWrite(Green, 255);
    analogWrite(Blue, 0);
    noTone(Buzzer);

  }
  else if(res == '2'){
    analogWrite(Red, 0);
    analogWrite(Green, 0);
    analogWrite(Blue, 255);
    noTone(Buzzer);
  }
  else {
    analogWrite(Red, 255);
    analogWrite(Green, 0);
    analogWrite(Blue, 0);
    tone(Buzzer, 1000, 3000); //pin, frequency, duration
  }
}

void callback(char* topic, byte* payload, unsigned int length){ 
  if (length==0) return;

  char got = (char)payload[0];

  Serial.print("\nReceived data: ");
  Serial.println(got);

  RGB_setup(got);
}

void publish_sensor_values(){
  if(!client.connected()) reconnect_mqtt(); //if no mqtt connection exists, try reconnecting

  // Format data into string and publish it
  char buffer[200];
  data_to_send(buffer, sizeof(buffer));
  client.publish(topic, buffer, false);

  Serial.print("Data published: ");
  Serial.println(buffer);
}

void initialize_and_publish(){ 

  publish_sensor_values();

  unsigned long start = millis(); //saves the current time in ms since boot
  while (millis() - start < 5000) { //creates a 5 sec window
    client.loop(); //checks MQTT, incoming messages, triggers callback
    delay(10);
  }
}
