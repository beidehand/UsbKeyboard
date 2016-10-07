# UsbKeyboard
Atmega328p VUSB Gameboy Zero or Arcade Controller

required hardware:

1. arduino nano, leonardo or at least something with an Atmega328p chip on it
2. two zener diodes 3v6, two 68r resistors, one 2.2k resistor (I actually used an 1.5k resistor)

Note you need to:

a. use version 1.0.5 of the arduino IDE which you can find at: https://www.arduino.cc/en/Main/OldSoftwareReleases#1.0.x
b. after you install the IDE go to the libraries folder and add the files from this repository (located under libraries)
c. pins 2,4 and 5 are used (D2, D4, D5 on a Arduino Nano V3) see schematic.jpg
