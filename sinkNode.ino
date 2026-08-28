#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define PORT 1883
#define SSID  "Jeremy Consolidated"
#define PASSWORD  "HUAJOVP30"
#define MQTT_SERVER "10.50.172.213"

WiFiClient sinkNode;
PubSubClient client(sinkNode); 

char rating;
String level;
const char* topic = "sensor/nodes";

void air_analysis(float air){
  if (air < 600) {
    rating =  '1'; // green
    level = "Optimal. (Enjoy the session).";
  }
  else if (air > 600 && air < 1250) {
    rating = '2'; // yellow
    level = "Good. (Less desirable levels).";
  }
  else{
    rating = '3'; //red
    level = "Poor. (Open a window).";
  }
}
void callback(char* topic, byte* payload, unsigned int length){ 
  StaticJsonDocument<200> data;

  DeserializationError err = deserializeJson(data, payload, length);
  if(err){
    Serial.println("JSON parse failed");
    return;
  }
  const char* nodeName = data["nodeName"];
    float dht_hum = data["dht_hum"];
    float dht_temp = data["dht_temp"];
    float scd_temp = data["scd_temp"];
    float scd_hum = data["scd_hum"];
    float scd_air = data["scd_air"];

  //=========================================================================
  air_analysis(scd_air);
  if (strcmp(nodeName, "Room 1") == 0){
    client.publish("results/room1", &rating, 1);
    Serial.print("Sent to Room 1: ");
    Serial.println(rating);
  }
  else if (strcmp(nodeName, "Room 2") == 0) {
    client.publish("results/room2", &rating, 1);

    Serial.print("Sent to Room 2: ");
    Serial.println(rating);
  }

  Serial.println("\n\t============ ROOM STATUS ============");
    Serial.print("\t Name: ");
    Serial.println(nodeName);

    Serial.print("\t CO2: ");
    Serial.print(scd_air);
    Serial.println(" ppm");
    Serial.print("\t Rating: ");
    Serial.println(level);

    Serial.print("\t DHT-Temp: ");
    Serial.print(dht_temp);
    Serial.print(" C");

    Serial.print(" | SCD-Temp: ");
    Serial.print(scd_temp);
    Serial.println(" C");

    Serial.print("\t DHT-Hum: ");
    Serial.print(dht_hum);
    Serial.print(" %");

    Serial.print(" | SCD-Hum: ");
    Serial.print(scd_hum);
    Serial.println(" %");
  Serial.println("\t=====================================");
}
void connect_wifi(){
  Serial.println("Connecting WiFi...");
  WiFi.begin(SSID, PASSWORD);

  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }
  Serial.println("Connected to WiFi.");
}

void reconnect_mqtt(){
  Serial.println("Connecting MQTT...");
  while(!client.connected()){
    // if(client.connect("Room1")){ //====================Change room before upload
    if(client.connect("Room2")){
      Serial.println("MQTT connected.");
      client.subscribe(topic);
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again");
      delay(10);
    }
  }
}

void setup() {
  Serial.begin(115200); 
  WiFi.begin();
  connect_wifi();
  client.setServer(MQTT_SERVER, PORT);
  client.setCallback(callback);
}

void loop() {
  if(!client.connected()) reconnect_mqtt();
  client.loop(); 
}
