/*

  YX5300_ESP32.h - Library for the YX5300 MP3 module.

  Created by bluejunimo, February 23, 2024.
  Updated by bluejunimo, April 22, 2024.

*/

#ifndef YX5300_ESP32_h

#define YX5300_ESP32_h

#include <Arduino.h>


// COMMANDS FOR MP3 PLAYER

// Volume control
#define CMD_VOLUME_UP 0X04
#define CMD_VOLUME_DOWN 0X05
#define CMD_SET_VOLUME 0X06

// Playing tracks
#define CMD_PLAY_INDEX 0X03
#define CMD_SINGLE_CYCLE_PLAY 0X08
#define CMD_PLAY_INDEX_AT_VOL 0X22
#define CMD_PLAY_FOLDER 0X17
#define CMD_PLAY_TRACK_IN_FOLDER 0X0F

// Controlling current tracks
#define CMD_RESUME 0X0D
#define CMD_PAUSE 0X0E
#define CMD_STOP 0X16
#define CMD_NEXT_TRACK 0X01
#define CMD_PREV_TRACK 0X02
#define CMD_SET_LOOP 0X19 // looping individual tracks

// Initializing values
#define CMD_SEL_DEV 0X09
#define DTA_DEV_TF 0X02

// Device state
#define CMD_SLEEP 0X0A
#define CMD_WAKE_UP 0X0B
#define CMD_RESET 0X0C

// DAC
#define CMD_SET_DAC 0X1A
#define DTA_DAC_ON 0X00
#define DTA_DAC_OFF 0X01

// Device Communication
#define MSG_BEGIN 0x7e
#define MSG_VERSION 0xff
#define MSG_LENGTH 0x06
#define MSG_NO_FEEDBACK 0x00
#define MSG_FEEDBACK 0x01
#define MSG_END 0xef

// Device Queries
#define QRY_CURRENT_TRACK_INDEX 0X4C
#define QRY_DEVICE_STATE 0x42
#define QRY_CURRENT_VOLUME 0x43
#define QRY_TRACK_COUNT 0x48
#define QRY_PLAY_MODE 0x45


// ---- *** ---- //


class YX5300_ESP32 {
    public:
        // Empty constructor.
        // Please use the full constructor when intializing the device.  
        YX5300_ESP32();

        // Full constructor. Initializes the device with a specific Serial channel.
        // HardwareSerial serial: Usually Serial2 if you are using the RX0/TX0 or RX1/TX1 on your board.
        // int aRX: The RX pin the device is connected to (usually 3 or 16).
        // int aTX: The TX pin the device is connected to (usually 1 or 17).
        YX5300_ESP32(HardwareSerial& serial, int aRX, int aTX);

        // Plays a specific track with a specific index. Only plays that one track once.
        // int index: the number of the track. (max. track index of 255).
        void playTrack(byte track_index);

        // Plays a specific track with a specific index. Plays the one track continously in a loop.
        void playTrackInLoop(byte track_index);

        // Plays all tracks from the folder, starting with the lowest track index. 
        // Continously loops the folder unless endLoopingTrack() is called.
        // int folder_index: the folder number (i.e. playing folder "03" would be 3).
        void playFolderInLoop(byte folder_index);

        // Begins playing a track from a specific folder.
        // int track_index: the track's standard index.
        // int folder_index: the folder number.
        // Example: You have 10 songs in folder 1. playTrackInFolder(13, 2) would play the third track in folder "02").
        void playTrackInFolder(byte track_index, byte folder_index);

        // Resumes the paused track.
        // If used when no track is playing or paused, begins playing 001.mp3 and continuously loops through all the tracks on the SD card. 
        void resume();

        // Pauses a currently playing track.
        // A track must be playing for this to function.
        void pause();

        // Stops the currently playing track and clears the queue. Cannot call resume() afterwards.
        // A track must be playing for this to function.
        void stop();

        // Skips the rest of the current track and plays the track of track_index + 1, regardless of if the current track is in a loop.
        // Using next() with playFolder() loops around the folder from the last track to the first one.
        // Otherwise, calling next() on the highest indexed track on the SD card plays the track 001.mp3.  
        void next();

        // Goes back and plays the track of track_index - 1, regardless of if the current track is in a loop.
        // Using prev() with playFolder() loops around the folder from the first track to the last one.
        // Otherwise, calling prev() on track 001.mp3 plays the last track on the SD card. 
        void prev();

        // Starts looping the current track.
        // Ignored by playFolder() unless endLoopingTrack is called beforehand.
        void beginLoopingTrack();

        // Ends the looping of the current track and clears the queue of tracks. 
        void endLoopingTrack();

        // Sets the volume to a specific amount.
        // int volume: between 0 (muted) and 30 (max volume).
        void setVolume(byte volume);

        // Increase the volume by 1.
        void incrementVolume();

        // Decrease the volume by 1.
        void decrementVolume();

        // Sleep the device to decrease power consumption when not in use.
        // Automatically stops tracks playing.
        // WARNING: waking the de
        void sleep();

        // Wake the device up from its sleep state.
        // Does nothing if device is not in sleep mode.
        void wake();

        // Resets the device.
        void reset();

        // Physically mutes the device by turning off the DAC output.
        void mute();

        // Switches the DAC output back on. 
        // Does nothing if DAC output is already on.
        void unmute();

        // Returns the index of the current track playing
        int queryCurrentTrackIndex();

        // Returns the device's current volume.
        // Range: 0-30.
        int queryCurrentVolume();

        // Returns the total number of tracks hosted on the TF card.
        int queryTrackCount();

        // Returns the state of the device
        // 0: stopped
        // 1: playing
        // 2: paused
        int queryDeviceState();

        // Enable debugging messages on Serial.
        void enableDebugging();

        // Disable debugging messages on Serial.
        void disableDebugging();

    private:
        // Serial connection
        HardwareSerial *mp3Serial;

        // Communication helper functions
        void sendCommand(byte command, byte param1, byte param2);
        void sendCommand(byte command, byte param1);
        void sendCommand(byte command);
        int16_t sendCommandWithResponse(byte command);

        // Debugging message prefixes
        String prefix = "[YX5300_ESP32] ";
        String DBUG_INFO = "info: ";
        String DBUG_WARNING = "Warning: ";
        String DBUG_ERROR = "ERROR: ";

        // Is debugging turned on?
        bool debugging = false;

        // Debugging helper functions
        void dbug(String status, String msg);
        void dbugStart1Line(String status, String msg);
        void dbug1Line(String msg);
        void dbug1LineHex(String msg);
        void dbugEnd1Line();

        // Misc helper functions
        int hexToInt(int16_t hexValue);
        

};


#endif