/*

  --- Basic playback of track ---
  
  Establishes connection with the YX5300 MP3 Module and plays the first 
  track on the device, looping through every song on the SD card.
  
  The circuit:
  - MP3 module TX connected to pin 16 (RX1)
  - MP3 module RX connected to pin 17 (TX1)
  
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

YX5300_ESP32 mp3; // the mp3 object

void setup() {
  // initialize connection with the module
  mp3 = YX5300_ESP32(Serial2, RX, TX);
  
  // Shows whats being sent to and from the device, with helpful errors
  // Serial.begin(115200);
  // mp3.enableDebugging();
  
  // Begins playing from 001.mp3/wav and continuously loops through all the songs 
  // on the SD card ONLY if there is no track already playing or paused.
  mp3.resume();
  
  // other ways to begin playing tracks (see documentation for functionality)
  // mp3.playTrack(1);
  // mp3.playTrackInLoop(1);
  // mp3.playFolderInLoop(1);
  // mp3.playTrackInFolder(1, 1);
}

void loop() {
  // put your main code here, to run repeatedly:

}
