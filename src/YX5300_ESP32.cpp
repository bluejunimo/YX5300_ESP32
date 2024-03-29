/*

  YX5300_ESP32.cpp - Library for the YX5300 MP3 module.

  Created by Max Nielsen, February 23, 2024.
  Updated by Max Nielsen, March 29, 2024.

*/

#include "Arduino.h"
#include "YX5300_ESP32.h"


#include <sstream> // for std::stringstream
#include <ios>     // for std::hex
#include <cstdlib> // for std::strtol


// Constructors
YX5300_ESP32::YX5300_ESP32() {
  
}

YX5300_ESP32::YX5300_ESP32(HardwareSerial& serial, int aRX, int aTX) {
    mp3Serial = &serial;
    mp3Serial->begin(9600, SERIAL_8N1, aRX, aTX);
    delay(500);
    sendCommand(CMD_SEL_DEV, 0, DEV_TF);
    delay(200);
}


// Begin playing tracks
void YX5300_ESP32::playTrack(int track_index) {
    sendCommand(CMD_PLAY_W_INDEX, track_index);
}

void YX5300_ESP32::playTrackInLoop(int track_index) {
  sendCommand(CMD_SINGLE_CYCLE_PLAY, track_index);
} 

void YX5300_ESP32::playFolderInLoop(int folder_index) {
    sendCommand(CMD_FOLDER_CYCLE, folder_index);
}


// Controlling the playing of the currrent tracks
void YX5300_ESP32::resume() {
    sendCommand(CMD_RESUME);
}

void YX5300_ESP32::pause() {
    sendCommand(CMD_PAUSE);
}

void YX5300_ESP32::stop() {
    sendCommand(CMD_STOP_PLAY);
}

void YX5300_ESP32::next() {
  sendCommand(CMD_NEXT_TRACK);
}

void YX5300_ESP32::prev() {
  sendCommand(CMD_PREV_TRACK);
}

void YX5300_ESP32::beginLoopingTrack() {
  sendCommand(CMD_SET_SINGLE_CYCLE, 0);
}

void YX5300_ESP32::endLoopingTrack() {
  sendCommand(CMD_SET_SINGLE_CYCLE, 1);
}

// Controlling the volume
void YX5300_ESP32::setVolume(int volume) {
  if(volume > 30) {
    sendCommand(CMD_SET_VOLUME, 30);
  }
  else if(volume < 0) {
    sendCommand(CMD_SET_VOLUME, 0);
  }
  else {
    sendCommand(CMD_SET_VOLUME, volume);
  }
}

void YX5300_ESP32::incrementVolume() {
  sendCommand(CMD_VOLUME_UP);
}

void YX5300_ESP32::decrementVolume() {
  sendCommand(CMD_VOLUME_DOWN);
}


// sends commands to the mp3 module through the serial connection
void YX5300_ESP32::sendCommand(byte command, int param1, int param2) {
  static int8_t send_buff[8] = {0};

  delay(20);
  
  send_buff[0] = 0x7e;
  send_buff[1] = 0xff;
  send_buff[2] = 0x06;
  send_buff[3] = command;
  send_buff[4] = 0x00;
  send_buff[5] = intToHex(param1);
  send_buff[6] = intToHex(param2);
  send_buff[7] = 0xef;
  
  for(uint8_t i = 0; i < 8; i++) {
    mp3Serial->write(send_buff[i]);
  }
}

void YX5300_ESP32::sendCommand(byte command, int param1) {
  if(param1 < 255) {
    sendCommand(command, 0, param1);
  }
}

void YX5300_ESP32::sendCommand(byte command) {
  sendCommand(command, 0, 0);
}

// convert int to hex: https://stackoverflow.com/a/69326849
byte YX5300_ESP32::intToHex(int i) {
    if(i <= 255) {
        std::stringstream ss;
        ss << std::hex << i;
        std::string intString = ss.str();

        // std::string intString = std::format("{:x}", i);
        byte intByte = strtol(intString.c_str(), NULL, 16);
        return intByte;
    }
    else {
        return 0;
    }
}