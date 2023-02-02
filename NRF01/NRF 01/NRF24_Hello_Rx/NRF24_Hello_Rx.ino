#include <SPI.h>
#include "nRF24L01.h"
#include <RF24.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
RF24 radio (7, 8); // CE, CSN
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
const byte address[6] = "00001";
void setup() {
  lcd.begin(16,2);  // Initialinze LCD.
  lcd.backlight(); // makes Backligh ON. 
  lcd.clear();     // Clears LCD
  lcd.home();
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1, address);
 //  radio.setPALevel(RF24_PA_MAX);
  // radio.setDataRate(RF24_250KBPS);
  radio.startListening();
}
void loop() {
  if (radio.available()) {
        char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
     lcd.print(text);
    delay(1000);  
     lcd.clear();
  }
   else if (!radio.available())  
   {
    Serial.println(" No Data ");
   
     lcd.print(" No Data ");
    delay(1000);
     lcd.clear();
   }
}
