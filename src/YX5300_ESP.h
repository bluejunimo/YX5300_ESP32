/*

  YX5300_ESP.h - Library for the YX5300 MP3 module.

  Created by Max Nielsen, February 23, 2024.

  Updated by Max Nielsen, February 23, 2024.

*/

#ifndef YX5300_ESP_h

#define YX5300_ESP_h

#include <Arduino.h>


// COMMANDS FOR MP3 PLAYER
#define CMD_NEXT_SONG 0X01
#define CMD_PREV_SONG 0X02
#define CMD_PLAY_W_INDEX 0X03
#define CMD_VOLUME_UP 0X04
#define CMD_VOLUME_DOWN 0X05
#define CMD_SET_VOLUME 0X06
#define CMD_SINGLE_CYCLE_PLAY 0X08
#define CMD_SEL_DEV 0X09
#define DEV_TF 0X02
#define CMD_SLEEP_MODE 0X0A
#define CMD_WAKE_UP 0X0B
#define CMD_RESET 0X0C
#define CMD_PLAY 0X0D
#define CMD_PAUSE 0X0E
#define CMD_PLAY_FOLDER_FILE 0X0F
#define CMD_STOP_PLAY 0X16
#define CMD_FOLDER_CYCLE 0X17
#define CMD_SET_SINGLE_CYCLE 0X19
#define SINGLE_CYCLE_ON 0X00
#define SINGLE_CYCLE_OFF 0X01
#define CMD_SET_DAC 0X1A
#define DAC_ON 0X00
#define DAC_OFF 0X01
#define CMD_PLAY_W_VOL 0X22
// ---- *** ---- //


class YX5300_ESP {
    public:
        YX5300_ESP();
        YX5300_ESP(HardwareSerial& serial, int aRX, int aTX);
        void play();
        void pause();
        void stop();
        void playFolder(int folder_number);
        void setVolume(int volume);
        void incrementVolume();
        void decrementVolume();
    private:
        void sendCommand(byte command, byte param1, byte param2);
        void sendCommand(byte command, byte param1, String param2);
        byte intToHex(int i);
        HardwareSerial *mp3Serial;

};


#endif