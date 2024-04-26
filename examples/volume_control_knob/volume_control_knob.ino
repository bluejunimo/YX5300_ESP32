/*

  --- Volume control with a potentiometer ---
  
  Establishes connection with the YX5300 MP3 Module, plays the first 
  track on the device, and allows you to control the volume with a potentiometer.
  Reads the analog input pin, maps the value between 0 and 30 (min/max volume),
  and sends that value to the MP3 module
  
  The circuit:
  - MP3 module TX connected to pin 16 (RX1)
  - MP3 module RX connected to pin 17 (TX1)
  - Potentiometer connected to analog pin 0
    Center pin goes to the analog pin
    Side pinds go to +5V and ground
  
  Other notes:
  1. The SD card must have at least one song for playback. 
  Consider using the example music provided on GitHub:
  https://github.com/bluejunimo/YX5300_ESP32/music
  
  
  created 26 Apr 2024
  modified 26 Apr 2024
  by bluejunimo

*/

#include <YX5300_ESP32.h>

// *make sure the RX on the YX5300 goes to the TX on the ESP32, and vice-versa
#define RX 16
#define TX 17

#define PIN_POT A0  // analog input pin the potentiometer is attached to

#define MIN_VOL 0
#define MAX_VOL 30
int current_volume = 0;

YX5300_ESP32 mp3; // the mp3 object

void setup() {
  // initialize connection with the module
  mp3 = YX5300_ESP32(Serial2, RX, TX);
  
  // Shows whats being sent to and from the device, with helpful errors
  // Serial.begin(115200);
  // mp3.enableDebugging();
  
  // play the first track
  mp3.playTrack(1);
}

void loop() {  
  // read the potentiometer's value
  int raw_value = analogRead(PIN_POT);
  
  // map the value to the min/max of the mp3 device's volume values 
  int new_volume = map(raw_value, 0, 4095, MIN_VOL, MAX_VOL)
  
  // only change the volume if the position has changed
  if(new_volume != current_volume) {
    // update the mp3 device's volume
    mp3.setVolume(new_volume);
    
    // update the current volume with the new volume
    current_volume = new_volume;
  }
}
