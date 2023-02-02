#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#define DOOR_PIN 0 // GPIO 0 pin for control
#define WLAN_SSID       "D-Link_DIR-600M" //
#define WLAN_PASS       "12345678" //
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883 //use 8883 if not working
#define AIO_USERNAME  "lukedynasty"
#define AIO_KEY       "aio_JXan63prsWSjvXJ9Bu4NqO9tlZhW"


WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe openDoorFeed = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/bulb");

void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(DOOR_PIN, OUTPUT);
  digitalWrite(DOOR_PIN, LOW);
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);

  Serial.println(); Serial.println();
  Serial.print("Connecting to Wi-FI ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("*");
  }
  Serial.println();

  Serial.println("Wi-FI Connected!");
  Serial.println("Use the ip : https://"); 
  Serial.println(WiFi.localIP());
 for(int i=0; i<3; i++)
  {
       delay(10);
       digitalWrite(2, 1);
       delay(50);
       digitalWrite(2, 0);
       delay(10);
       digitalWrite(0, 1); 
       delay(50);
       digitalWrite(0, 0);
       delay(500);
  }

  mqtt.subscribe(&openDoorFeed);
}

uint32_t x=0;

void loop() {

  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  
  while ((subscription = mqtt.readSubscription(5000))) {
    
    if (subscription == &openDoorFeed) {
      
      Serial.print(F("Data riceived : "));
      Serial.println((char *)openDoorFeed.lastread);
      digitalWrite(DOOR_PIN, (int)openDoorFeed.lastread);

      if(!strcmp((char *)openDoorFeed.lastread, "on")) on_bulb();
      if(!strcmp((char *)openDoorFeed.lastread, "off")) off_bulb();
      
    }
    
  }

  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
}

void on_bulb() {
  digitalWrite(DOOR_PIN, HIGH);
}

void off_bulb() {
  digitalWrite(DOOR_PIN, LOW);
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connected to server MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { 
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Attempt to connect in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  
       retries--;
       if (retries == 0) {
         while (1);
       }
  }
  Serial.println("MQTT connected!");
}
