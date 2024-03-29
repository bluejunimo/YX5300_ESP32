/*

  YX5300_ESP32.h - Library for the YX5300 MP3 module.

  Created by Max Nielsen, February 23, 2024.
  Updated by Max Nielsen, March 29, 2024.

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
#define CMD_PLAY_W_INDEX 0X03
#define CMD_SINGLE_CYCLE_PLAY 0X08
#define CMD_PLAY_W_VOL 0X22
#define CMD_FOLDER_CYCLE 0X17
#define CMD_PLAY_FOLDER_FILE 0X0F

// Controlling current tracks
#define CMD_RESUME 0X0D
#define CMD_PAUSE 0X0E
#define CMD_STOP_PLAY 0X16
#define CMD_NEXT_TRACK 0X01
#define CMD_PREV_TRACK 0X02
#define CMD_SET_SINGLE_CYCLE 0X19 // looping songs

// Initializing values
#define CMD_SEL_DEV 0X09
#define DEV_TF 0X02

// Device state
#define CMD_SLEEP_MODE 0X0A
#define CMD_WAKE_UP 0X0B
#define CMD_RESET 0X0C

// DAC
#define CMD_SET_DAC 0X1A
#define DAC_ON 0X00
#define DAC_OFF 0X01

// ---- *** ---- //


class YX5300_ESP32 {
    public:
        // Empty constructor.
        // Please use the full constructor when intializing the device.  
        YX5300_ESP32();

        // Full constructor. Initializes the device with a specific Serial channel
        // HardwareSerial serial: Usually Serial2 if you are using the RX0 and TX0 on your board
        // int aRX: The RX pin the device is connected to (usually 3)
        // int aTX: The TX pin the device is connected to (usually 1)
        YX5300_ESP32(HardwareSerial& serial, int aRX, int aTX);

        // Plays a specific track with a specific index. Only plays that one track once.
        // int index: the number of the track. (max. track index of 255)
        void playTrack(int track_index);

        // Plays a specific track with a specific index. Plays the one track continously in a loop.
        void playTrackInLoop(int track_index);

        // Plays all tracks from the folder, starting with the lowest song index. 
        // Continously loops the folder unless endLoopingTrack() is called.
        // int folder_index: the folder number (i.e. playing folder "03" would be 3)
        void playFolderInLoop(int folder_index);

        // Resumes the paused track.
        // If used when no track is playing or paused, begins playing 001.mp3 and continuously loops through all the songs on the SD card. 
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
        // Ignored by playFolder() unless endLoopingTrack is called beforehand
        void beginLoopingTrack();

        // Ends the looping of the current track and clears the queue of songs. 
        void endLoopingTrack();

        // Sets the volume to a specific amount
        // int volume: between 0 (muted) and 30 (max volume)
        void setVolume(int volume);

        // Increase the volume by 1
        void incrementVolume();

        // Decrease the volume by 1
        void decrementVolume();

    private:
        void sendCommand(byte command, int param1, int param2);
        void sendCommand(byte command, int param1);
        void sendCommand(byte command);
        byte intToHex(int i);
        HardwareSerial *mp3Serial;

};


#endif