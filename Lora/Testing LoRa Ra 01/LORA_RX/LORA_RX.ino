#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 9
Adafruit_SSD1306 display(OLED_RESET);
String LoRaData;
void setup() {
    Serial.begin(9600);
  while (!Serial);
  Serial.println("LoRa Receiver");
    
    if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false);
      display.clearDisplay();
}

void loop() {
  display.clearDisplay();
    display.setTextSize(1);
  display.setTextColor(WHITE);  
  display.setCursor(0,0);
 
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
       LoRaData = LoRa.readString();
    Serial.print((char)LoRa.read());

    }
         Serial.print(LoRaData); 
      display.setCursor(0,0);
display.println(LoRaData);
     Serial.println("");



    // print RSSI of packet
 //   Serial.print("' with RSSI ");
 //   Serial.println(LoRa.packetRssi());
    display.display();
  }
}
