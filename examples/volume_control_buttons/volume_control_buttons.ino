/*

  --- Volume control with push buttons ---
  
  Establishes connection with the YX5300 MP3 Module, plays the first 
  track on the device, and allows you to control the volume with two buttons.
  Checks for a button press on both digital input pins, and increments or
  decrements the MP3 module's volume based on which button was pressed.
  Also allows for holding the buttons down for faster adjustment. 
  
  The circuit:
  - MP3 module TX connected to pin 16 (RX1)
  - MP3 module RX connected to pin 17 (TX1)
  - Two buttons connected to pins 18 and 19
    One pin goes to +5V
    The opposite pin goes to GND with a 10k resistor and the digital pins
  
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

#define PIN_INCR 18   // general purpose pin the + vol button is attached to
#define PIN_DECR 19   // general purpose pin the - vol button is attached to

int hold_delay = 500; // interval of volume changes when holding down the buttons; in milliseconds
long timer = 0;
bool button_down = false;

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
  // Increase the volume by one if the + vol button is being pressed or held
  if(buttonPressed(PIN_INCR)) {
    mp3.incrementVolume();
  }
  
  // Decrease the volume by one if the - vol button is being pressed or held
  if(buttonPressed(PIN_DECR)) {
    mp3.decrementVolume();
  }
  
  delay(20);
}

// buttonPressed(int pin) : bool
// 1. Check if the button is being pressed
// 2. Only return true the instant the user presses the button and 
//    at a set interval if the user is holding the button down.
bool buttonPressed(int pin) {
  // read the digital in value 
  int button_state = digitalRead(pin);
  
  // button is being pressed
  if(button_state == HIGH) {
    
    // button was just pressed 
    if(!button_down) {
      timer = millis();
      button_down = true;
      return true;
    }
    // button is being held down. only change volume at interval hold_delay
    else if(hold_delay + timer < millis()) {
      timer = millis();
      return true;
    }
  }
  // button is not being pressed
  else {
    button_down = false;
  }
  
  return false;
}