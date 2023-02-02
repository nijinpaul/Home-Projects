#include <SPI.h>
#include "nRF24L01.h"
#include <RF24.h>
RF24 radio (7, 8); // CE, CSN
const byte address[6] = "00001";
int c = 0;
void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  // radio.setPALevel(RF24_PA_MAX);
  //radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
}
void loop() {
  const char text[] = "Hello World";
  radio.write(&text, sizeof(text));
  radio.write(&c, sizeof(c));
  c++;
  //delay(2000);
}
