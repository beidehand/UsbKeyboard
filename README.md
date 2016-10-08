# UsbKeyboard
Atmega328p VUSB Gameboy Zero or Arcade Controller

Using this USB device was only tested on a raspberry pi B with Retropie

required hardware:

1. arduino nano, leonardo or at least something with an Atmega328p chip on it
2. two zener diodes 3v6, two 68r resistors, one 2.2k resistor (I actually used an 1.5k resistor)

Note you need to:

3. use version 1.0.5 of the arduino IDE which you can find at: https://www.arduino.cc/en/Main/OldSoftwareReleases#1.0.x
4. after you install the IDE go to the libraries folder and add the files from this repository (located under libraries)
5. pins 2,4 and 5 are used for USB communications (D2, D4, D5 on a Arduino Nano V3) see schematic.jpg
6. solder the diodes and resitors to D2, D4 and D5 as shown in the schematic
7. pins 14,15,16,17 are actually analog pins(A0,A1,A2,A3), but the can be used as digital inputs as well
8. this software is pull-up, so your buttons should go to ground (not 5V or else). Use a common ground.

The pin setup for the keys is as follows:

define NUMBER_OF_KEYS 15
char pinCharacters[NUMBER_OF_KEYS] = {
  KEY_ENTER,KEY_B,KEY_Z,KEY_X,KEY_A,KEY_S,KEY_ARROW_UP,KEY_ARROW_DOWN,KEY_ARROW_LEFT,KEY_ARROW_RIGHT,KEY_K,KEY_Q,KEY_W,KEY_E,KEY_R};
int pinButton[NUMBER_OF_KEYS] = {
  0,1,3,6,7,8,9,10,11,12,13,14,15,16,17};
  
If you use this software, make sure that:

9. you solder your keys / buttons to the proper input (as mentioned above)
10. change the above code to match your own soldering.....keeping in mind that by default pins D2,D4 and D5 are used for USB communication
11. if you want to change the used pins for USB check usbconfig.h (USB_CFG_DMINUS_BIT, USB_CFG_DPLUS_BIT and USB_CFG_PULLUP_BIT)
