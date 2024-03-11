#define RX 3
#define TX 1

#include <YX5300_ESP.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("SERIAL ONE INITIALIZED!");
  
  YX5300_ESP mp3 = YX5300_ESP(Serial2, RX, TX);
  
  mp3.playFolder(1);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
