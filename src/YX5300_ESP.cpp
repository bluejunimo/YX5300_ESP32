/*

  YX5300_ESP.cpp - Library for the YX5300 MP3 module.

  Created by Max Nielsen, February 23, 2024.

  Updated by Max Nielsen, February 23, 2024.

*/



#include "Arduino.h"
#include "YX5300_ESP.h"


#include <sstream> // for std::stringstream
#include <ios>     // for std::hex
#include <cstdlib> // for std::strtol

YX5300_ESP::YX5300_ESP() {
  
}

YX5300_ESP::YX5300_ESP(HardwareSerial& serial, int aRX, int aTX) {
    mp3Serial = &serial;
    mp3Serial->begin(9600, SERIAL_8N1, aRX, aTX);
    delay(500);
    sendCommand(CMD_SEL_DEV, 0, DEV_TF);
    delay(200);
}

void YX5300_ESP::play() {
    sendCommand(CMD_PLAY, 0, 0);
}

void YX5300_ESP::pause() {
    sendCommand(CMD_PAUSE, 0, 0);
}

void YX5300_ESP::stop() {
    sendCommand(CMD_STOP_PLAY, 0, 0);
}

void YX5300_ESP::playFolder(int folder_number) {
    sendCommand(CMD_FOLDER_CYCLE, 0, folder_number);
}

void YX5300_ESP::setVolume(int volume) {
  sendCommand(CMD_SET_VOLUME, 0, intToHex(volume));
}

void YX5300_ESP::incrementVolume() {
  sendCommand(CMD_VOLUME_UP, 0, 0);
}

void YX5300_ESP::decrementVolume() {
  sendCommand(CMD_VOLUME_DOWN, 0, 0);
}

// sends commands to the mp3 module through the serial connection
void YX5300_ESP::sendCommand(byte command, byte param1, byte param2) {
  static int8_t send_buff[8] = {0};

  delay(20);
  
  send_buff[0] = 0x7e;
  send_buff[1] = 0xff;
  send_buff[2] = 0x06;
  send_buff[3] = command;
  send_buff[4] = 0x00;
  send_buff[5] = param1;
  send_buff[6] = param2;
  send_buff[7] = 0xef;
  
  for(uint8_t i = 0; i < 8; i++) {
    mp3Serial->write(send_buff[i]);
  }
}

// convert int to hex: https://stackoverflow.com/a/69326849
byte YX5300_ESP::intToHex(int vol) {
    if(vol <= 255) {
        std::stringstream ss;
        ss << std::hex << vol;
        std::string volStr = ss.str();

        // std::string volStr = std::format("{:x}", vol);
        byte volByte = strtol(volStr.c_str(), NULL, 16);
        return volByte;
    }
    else {
        return 0;
    }
}