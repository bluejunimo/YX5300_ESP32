# YX5300 for ESP32
A library to support the YX5300/YX6300 MP3 Module on the ESP32 platform. I created it because the other libaries use SoftwareSerial, which is unavailable on ESP32. Instead, this libary uses HardwareSerial to interface with the device to control the basic functions of the module. We simplify commands and queries and responses to make it easy for the end-user to use this as an audio player and controller.

## The MP3 Module
<img src="https://github.com/bluejunimo/YX5300_ESP32/blob/main/docs/yx5300.png" width="400"/>
Pictured above is how most YX5300 and YX6300 chips are implemented. They have a simple UART control interface with TX, RX, VCC, and GND pins.




### Specifications
Pulled from the [manual](https://github.com/bluejunimo/YX5300_ESP32/blob/main/docs/Manual.pdf).

**Note:** VCC is stated as 3.2V-5.2V in the specifications. However, from testing multiple modules, it is most reliable when connected to 5V. It works intermittently on 3.3V.  
| Power  | Value |
| ------------- |:-------------:|
| Power Supply (VCC)      | 3.2-5.2 VDC     |
| Max. Current (@ 5V)      | 200mA     |
| Logic Level Interface      | 3.3V / 5V TTL     |

| Card & Tracks | Value |
| ------------- |:-------------:|
| Supported Cards      | microSD/SDHC (<= 32GB)     |
| Card Format      | FAT16/32     |
| Track Formats       |  MP3/WAV     |
| Frequencies      | 8-48 kHz     |

| Misc.  | Value |
| ------------- |:-------------:|
| Baud Rate      | 9600 bps     |
| Volume Range      | 0 - 30     |
| Dimensions      | 49 x 24 x 8.5mm     |
| Weight      | 5 grams     |


## Getting Started
Please refer to the example simpletest.ino and the YX5300_ESP32.h file for information about functions.

### Folder/File Structure
If you just need to test the device, consider using the [example music folder](https://github.com/bluejunimo/YX5300_ESP32/blob/main/music) in this repository. It is already setup in this structure with public domain music.


Otherwise, let's start by organizing your audio tracks. Once you have gathered all your tracks, we need to rename them and place them in folders that match the structure expected by the MP3 module.

1. First, format your microSD card as FAT16 or FAT32.
2. Then, create however many folders you want starting with the label "01" and incrementing with each new folder. Think of folders as different playlists.
3. Your tracks need to have a 3-digit prefix at the start. They each must be unique, but naming them sequentially is the easiest way. Anything after the prefix is ignored by the module.

Here is an example of the folder and file structure of the microSD card:

```
01/
    001xxx.mp3
    002xxx.mp3
    003xxx.mp3

02/
    004xxx.mp3
    005xxx.mp3
    006xxx.mp3
    007xxx.mp3

03/
    008xxx.mp3
    009xxx.mp3
    010xxx.mp3
```

There is a maximum of 99 folders with 255 songs in each folder.



### Connecting the Module to an ESP32
The UART interface pins on the module make it easy to connect to your ESP using simple jumper wires. 

Connect these to the following:
| MP3 Module  | ESP32 |
| ------------- |:-------------:|
| TX      | any RX pin      |
| RX      | any TX pin     |
| VCC      | 5V     |
| GND      | GND     |

From testing, using TX0/RX0 (pins 1/3)  jumbles the Serial communication between the ESP32 and your computer. Using TX1/RX1 (pins 17, 16) has been the most reliable for [my board](https://www.tinytronics.nl/en/development-boards/microcontroller-boards/with-wi-fi/makepico-esp32-d0wdq6-development-board-with-wi-fi-and-bluetooth). It might vary board-to-board.


### Coding
Familiarize yourself with the [examples](https://github.com/bluejunimo/YX5300_ESP32/tree/main/examples/) and explore the [header file](https://github.com/bluejunimo/YX5300_ESP32/blob/main/src/YX5300_ESP32.h) to learn more about specific functions and abilities this device has. I plan to create full documentation in the future.
