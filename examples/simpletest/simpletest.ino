/* 

simpletest.ino - testing basic functionality of the YX5300 MP3 player.

Created by Max Nielsen, February 23, 2024.
Updated by Max Nielsen, March 29, 2024.

*/

#include "YX5300_ESP32.h"

// typical pins for ESP32 RX and TX
// *make sure the RX on the YX5300 goes to the TX on the ESP, and vice-versa
#define RX 16
#define TX 17

YX5300_ESP32 mp3;

void setup() {
  Serial.begin(115200);
 
  // Full constructor. Initializes the device with a specific Serial channel
  // HardwareSerial serial: Usually Serial2 if you are using the RX0 and TX0 on your board
  // int aRX: The RX pin the device is connected to
  // int aTX: The TX pin the device is connected to
  mp3 = YX5300_ESP32(Serial2, RX, TX);

  // Shows whats being sent to and from the device, with helpful errors
  mp3.enableDebugging();
  
  // Begins playing from 001.mp3/wav and continuously loops through all the songs on the SD card ONLY if there is no track already playing or paused.
  mp3.resume();
  
  // other ways to begin playing tracks:
  // mp3.playTrack(1);
  // mp3.playTrackInLoop(1);
  // mp3.playFolderInLoop(1);
  // mp3.playTrackInFolder(1, 1);
  
  // pauses the track after 3 seconds for 5 seconds
  delay(3000);
  mp3.pause();
  delay(5000);
  
  // Resumes playing the current track.
  mp3.resume();

  Serial.println("Testing standard serial communication");
}

void loop() {
  // put your main code here, to run repeatedly:
  

  // Set the volume to a random value every 10 seconds
  delay(10000);
  mp3.setVolume(random(0, 31));
}
