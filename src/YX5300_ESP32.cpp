/*

  YX5300_ESP32.cpp - Library for the YX5300 MP3 module.

  Created by bluejunimo, February 23, 2024.
  Updated by bluejunimo, April 22, 2024.

*/

#include "Arduino.h"
#include "YX5300_ESP32.h"

// Constructors
YX5300_ESP32::YX5300_ESP32() {
  
}

YX5300_ESP32::YX5300_ESP32(HardwareSerial& serial, int aRX, int aTX) {
    mp3Serial = &serial;
    mp3Serial->begin(9600, SERIAL_8N1, aRX, aTX);
    delay(500);
    sendCommand(CMD_SEL_DEV, 0, DTA_DEV_TF);
    delay(200);
}


// Begin playing tracks
void YX5300_ESP32::playTrack(byte track_index) {
  dbug(DBUG_INFO, "--- Playing track with index " + String(track_index) + ".");
  sendCommand(CMD_PLAY_INDEX, track_index);
}

void YX5300_ESP32::playTrackInLoop(byte track_index) {
  dbug(DBUG_INFO, "--- Playing track in loop with index " + String(track_index) + ".");
  sendCommand(CMD_SINGLE_CYCLE_PLAY, track_index);
} 

void YX5300_ESP32::playFolderInLoop(byte folder_index) {
  dbug(DBUG_INFO, "--- Playing folder in loop with index " + String(folder_index) + ".");
  sendCommand(CMD_PLAY_FOLDER, folder_index);
}

void YX5300_ESP32::playTrackInFolder(byte track_index, byte folder_index) {
  dbug(DBUG_INFO, "--- Playing track #" + String(track_index) + " in folder with index " + String(folder_index) + ".");
  sendCommand(CMD_PLAY_TRACK_IN_FOLDER, folder_index, track_index);
}


// Controlling the playing of the currrent tracks
void YX5300_ESP32::resume() {
  dbug(DBUG_INFO, "--- Resuming track.");
  sendCommand(CMD_RESUME);
}

void YX5300_ESP32::pause() {
  dbug(DBUG_INFO, "--- Pausing track.");
  sendCommand(CMD_PAUSE);
}

void YX5300_ESP32::stop() {
  dbug(DBUG_INFO, "--- Stopping track.");
  sendCommand(CMD_STOP);
}

void YX5300_ESP32::next() {
  dbug(DBUG_INFO, "--- Skipping to next track.");
  sendCommand(CMD_NEXT_TRACK);
}

void YX5300_ESP32::prev() {
  dbug(DBUG_INFO, "--- Retruning to previous track.");
  sendCommand(CMD_PREV_TRACK);
}

void YX5300_ESP32::beginLoopingTrack() {
  dbug(DBUG_INFO, "--- Looping track.");
  sendCommand(CMD_SET_LOOP, 0);
}

void YX5300_ESP32::endLoopingTrack() {
  dbug(DBUG_INFO, "--- Stopping track looping.");
  sendCommand(CMD_SET_LOOP, 1);
}


// Controlling the volume
void YX5300_ESP32::setVolume(byte volume) {
  if(volume > 30) {
    dbug(DBUG_WARNING, "--- Volume parameter (" + String(volume) + ") above limit. Setting device to maximum volume of 30.");
    sendCommand(CMD_SET_VOLUME, 30);
  }
  else if(volume < 0) {
    dbug(DBUG_WARNING, "--- Volume parameter (" + String(volume) + ") below threshold. Setting device to minimum volume of 0 (muted).");
    sendCommand(CMD_SET_VOLUME, 0);
  }
  else {
    dbug(DBUG_INFO, "--- Setting device volume to " + String(volume) + ".");
    sendCommand(CMD_SET_VOLUME, volume);
  }
}

void YX5300_ESP32::incrementVolume() {
  dbug(DBUG_INFO, "--- Increasing volume by 1.");
  sendCommand(CMD_VOLUME_UP);
}

void YX5300_ESP32::decrementVolume() {
  dbug(DBUG_INFO, "--- Decreasing volume by 1.");
  sendCommand(CMD_VOLUME_DOWN);
}


// Controlling Device States
void YX5300_ESP32::sleep() {
  dbug(DBUG_INFO, "--- Setting device to sleep.");
  sendCommand(CMD_SLEEP);
}

void YX5300_ESP32::wake() {
  dbug(DBUG_INFO, "--- Waking up device.");
  sendCommand(CMD_WAKE_UP);
}

void YX5300_ESP32::reset() {
  dbug(DBUG_INFO, "--- RESETTING DEVICE!");
  sendCommand(CMD_RESET);
}


// Control the onboard DAC
void YX5300_ESP32::mute() {
  dbug(DBUG_INFO, "--- Turning DAC off.");
  sendCommand(CMD_SET_DAC, DTA_DAC_OFF);
} 

void YX5300_ESP32::unmute() {
  dbug(DBUG_INFO, "--- Turning DAC on.");
  sendCommand(CMD_SET_DAC, DTA_DAC_ON);
} 
 

// Querying the device
int YX5300_ESP32::queryCurrentTrackIndex() {
  dbug(DBUG_INFO, "--- Querying current track index...");
  return hexToInt(sendCommandWithResponse(QRY_CURRENT_TRACK_INDEX));
}

int YX5300_ESP32::queryCurrentVolume() {
  dbug(DBUG_INFO, "--- Querying current volume...");
  return hexToInt(sendCommandWithResponse(QRY_CURRENT_VOLUME));
  
}

int YX5300_ESP32::queryTrackCount() {
  dbug(DBUG_INFO, "--- Querying number of tracks on TF card...");
  return hexToInt(sendCommandWithResponse(QRY_TRACK_COUNT));
}

int YX5300_ESP32::queryDeviceState() {
  dbugStart1Line(DBUG_INFO, "--- Querying device play/pause/stop state...");
  return hexToInt(sendCommandWithResponse(QRY_DEVICE_STATE));
}


// sends commands to the mp3 module through the serial connection
void YX5300_ESP32::sendCommand(byte command, byte param1, byte param2) {
  static int8_t send_buff[8] = {0};
  
  // create message
  send_buff[0] = MSG_BEGIN;
  send_buff[1] = MSG_VERSION;
  send_buff[2] = MSG_LENGTH;
  send_buff[3] = command;
  send_buff[4] = MSG_NO_FEEDBACK;
  send_buff[5] = param1;
  send_buff[6] = param2;
  send_buff[7] = MSG_END;

  dbugStart1Line(DBUG_INFO, "Sending hex message: ");
  
  // Iterate through each item and send it to the device.
  for(int i = 0; i < 8; i++) {
    dbug1Line(String(send_buff[i], HEX));
    dbug1Line(" ");
    mp3Serial->write(send_buff[i]);
  }
  dbugEnd1Line();
}

void YX5300_ESP32::sendCommand(byte command, byte param1) {
  sendCommand(command, 0, param1);
}

void YX5300_ESP32::sendCommand(byte command) {
  sendCommand(command, 0, 0);
}

// await response from sending a command. process it and return its value.
int16_t YX5300_ESP32::sendCommandWithResponse(byte command) {
  sendCommand(command, 0, 0);

  int time_command_sent = millis();
  int time_until_timeout = 3000;

  char raw_response[16];
  int index = 0;
  int16_t parsed_response;

  dbugStart1Line(DBUG_INFO, "Receiving hex message: ");

  while(true) {
    int output = '\0';

    // Check if the response time has exceeded the timeout time 
    if(millis() - time_command_sent > time_until_timeout) {
      dbugEnd1Line();
      dbug(DBUG_ERROR, "Query timed out, please retry.");
      return -0x0A;
    }
    
    // Get response data
    if(mp3Serial->available()) {

      output = mp3Serial->read();

      raw_response[index++] = output;

      dbug1Line(String(output, HEX));
      dbug1Line(" ");
      
      // Messge ended: parse response data.
      if(output == MSG_END) {
        dbugEnd1Line();
        if(raw_response[3] != command) {
          dbug(DBUG_WARNING, "Unexpected response " + String(raw_response[3], HEX) + ": still listnening...");
          dbugStart1Line(DBUG_INFO, "Receiving hex message: ");
          index = 0;
        }
        else {
          parsed_response = (int16_t)raw_response[5] << 8 | raw_response[6];
          dbug(DBUG_INFO, "Response to command " + String(command, HEX) + ": " + String(parsed_response, HEX));
          return parsed_response;
        }
      }
    }
  }
}

// Misc helper function
int YX5300_ESP32::hexToInt(int16_t hexValue) {
  return static_cast<int>(hexValue);
}


// Debugging functions
void YX5300_ESP32::enableDebugging() {
  debugging = true;
}

void YX5300_ESP32::disableDebugging() {
  debugging = false;
}

void YX5300_ESP32::dbug(String status, String msg) {
  if(debugging) {
    Serial.println(prefix + status + msg);
  }
}

void YX5300_ESP32::dbugStart1Line(String status, String msg) {
  if(debugging) {
    Serial.print(prefix + status + msg);
  }
}

void YX5300_ESP32::dbug1Line(String msg) {
  if(debugging) {
    Serial.print(msg);
  }
}

void YX5300_ESP32::dbugEnd1Line() {
  if(debugging) {
    Serial.println("");
  }
}