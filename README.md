# UsbKeyboard
Atmega328p VUSB Gameboy Zero or Arcade Controller

required hardware:

1. arduino nano, leonardo or at least something with an Atmega328p chip on it
2. two zener diodes 3v6, two 68r resistors, one 2.2k resistor (I actually used an 1.5k resistor)

Note you need to:

3. use version 1.0.5 of the arduino IDE which you can find at: https://www.arduino.cc/en/Main/OldSoftwareReleases#1.0.x
4. after you install the IDE go to the libraries folder and add the files from this repository (located under libraries)
5. pins 2,4 and 5 are used (D2, D4, D5 on a Arduino Nano V3) see schematic.jpg

The pin setup for the keys is as follows:

#define NUMBER_OF_KEYS 11
char pinCharacters[NUMBER_OF_KEYS] = {
  KEY_ENTER,KEY_B,KEY_Z,KEY_X,KEY_E,KEY_F,KEY_ARROW_UP,KEY_ARROW_DOWN,KEY_ARROW_LEFT,KEY_ARROW_RIGHT,KEY_K};
int pinButton[NUMBER_OF_KEYS] = {
  0,1,3,6,7,8,9,10,11,12,13};
  
If you use this software, make sure that:
6. you solder your keys / buttons to the proper input (as mentioned above)
7. change the above code to match your own soldering.....keeping in mind that by default pins D2,D4 and D5 are used for USB communication
8. if you want to change the used pins for USB check usbconfig.h (USB_CFG_DMINUS_BIT, USB_CFG_DPLUS_BIT and USB_CFG_PULLUP_BIT)
