// This coding is for ESP 8266 01 model
// Compatable with NODE MCU as well
// Follow my channel https://www.youtube.com/channel/UCKWKPFYLsz2gl0EhMo7rRqQ/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h> //
#include <ArduinoJson.h>  
#include <StreamString.h>

ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;
WiFiClient client;

#define MyApiKey "xxxxxxxxxxxxxxxxxxxxxxxx" //  Change to your sinric API Key.
#define MySSID "xxxxxxxxxxxxx" // Change to your Wifi  SSID
#define MyWifiPassword "xxxxxxxxxx" // Change to your Wifi  password

#define HEARTBEAT_INTERVAL 300000 // 5 Minutes 

uint64_t heartbeatTimestamp = 0;
bool isConnected = false;



void turnOn(String deviceId) {
  if (deviceId == "xxxxxxxxxxxxxxxxxxx") // Device ID in your dash ID
  {  
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(0, HIGH);
  } 
  
}

void turnOff(String deviceId) {
   if (deviceId == "xxxxxxxxxxxxxxxxxx") // Device ID in your dash ID
   {  
     Serial.print("Turn off Device ID: ");
     Serial.println(deviceId);
     digitalWrite(0, LOW);
   }

}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      isConnected = false;    
      Serial.printf("[WSc] Webservice disconnected from sinric.com!\n");
      break;
    case WStype_CONNECTED: {
      isConnected = true;
      Serial.printf("[WSc] Service connected to sinric.com at url: %s\n", payload);
      Serial.printf("Waiting for commands from sinric.com ...\n");        
      }
      break;
    case WStype_TEXT: {
        Serial.printf("[WSc] get text: %s\n", payload);
 
#if ARDUINOJSON_VERSION_MAJOR == 5
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject((char*)payload);
#endif
#if ARDUINOJSON_VERSION_MAJOR == 6        
        DynamicJsonDocument json(1024);
        deserializeJson(json, (char*) payload);      
#endif        
        String deviceId = json ["deviceId"];     
        String action = json ["action"];
        
        if(action == "setPowerState") { // Switch or Light
            String value = json ["value"];
            if(value == "ON") {
                turnOn(deviceId);
            } else {
                turnOff(deviceId);
            }
        }
        else if (action == "SetTargetTemperature") {
            String deviceId = json ["deviceId"];     
            String action = json ["action"];
            String value = json ["value"];
        }
        else if (action == "test") {
            Serial.println("[WSc] received test command from sinric.com");
        }
      }
      break;
    case WStype_BIN:
      Serial.printf("[WSc] get binary length: %u\n", length);
      break;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  
  WiFiMulti.addAP(MySSID, MyWifiPassword);
  Serial.println();
  Serial.print("Connecting to Wifi: ");
  Serial.println(MySSID);  

  // Waiting for Wifi connect
  while(WiFiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  if(WiFiMulti.run() == WL_CONNECTED) {
    Serial.println("");
    Serial.print("WiFi connected. ");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }

  // server address, port and URL
  webSocket.begin("iot.sinric.com", 80, "/");

  // event handler
  webSocket.onEvent(webSocketEvent);
  webSocket.setAuthorization("apikey", MyApiKey);
  
  // try again every 5000ms if connection has failed
  webSocket.setReconnectInterval(5000);   // If you see 'class WebSocketsClient' has no member named 'setReconnectInterval' error update arduinoWebSockets
   for(int i=0; i<3; i++) // blink the bulb everytime when it gets connected to the home wifi.
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
}

void loop() {
  webSocket.loop();
  
  if(isConnected) {
      uint64_t now = millis();
      
            if((now - heartbeatTimestamp) > HEARTBEAT_INTERVAL) {
          heartbeatTimestamp = now;
          webSocket.sendTXT("H");          
      }
  }   
}
