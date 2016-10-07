#include "UsbKeyboard.h"

#define NUMBER_OF_KEYS 11

int lastState[NUMBER_OF_KEYS]; // LOW is equivalent to 0
char pinCharacters[NUMBER_OF_KEYS] = {
  KEY_ENTER,KEY_B,KEY_Z,KEY_X,KEY_A,KEY_S,KEY_ARROW_UP,KEY_ARROW_DOWN,KEY_ARROW_LEFT,KEY_ARROW_RIGHT,KEY_K};
int pinButton[NUMBER_OF_KEYS] = {
  0,1,3,6,7,8,9,10,11,12,13};

void setup() {

  //  button setup
  for(int i = 0; i < NUMBER_OF_KEYS; i++){
    pinMode(pinButton[i], INPUT_PULLUP);
    lastState[i] = HIGH;
  }

  // USB setup -----------------
  // Disable timer0 since it can mess with the USB timing. Note that
  // this means some functions such as delay() will no longer work.
  TIMSK0 &= !(1<TOIE0);
  // Clear interrupts while performing time-critical operations
  cli();

  // Force re-enumeration so the host will detect us
  usbDeviceDisconnect();
  delayMs(250);
  usbDeviceConnect();

  // Set interrupts again
  sei();

  UsbKeyboard.resetAll();
}

void loop() {
  // update USB device state
  UsbKeyboard.update();
  for(int i = 0; i < NUMBER_OF_KEYS; i++){
    // check, if transition to button-pressed has occured
    if( (digitalRead(pinButton[i]) == LOW) && (lastState[i]==HIGH) ) {
      UsbKeyboard.press(pinCharacters[i]); // if so, send a letter via USB keyboard
      lastState[i] = LOW; // update the state variable
    } 
    else if( (digitalRead(pinButton[i]) == HIGH) && (lastState[i]==LOW) ) {
       
      UsbKeyboard.release(pinCharacters[i]);
      lastState[i] = HIGH;
    } 
    
  }

}

// helper method for V-USB library
void delayMs(unsigned int ms) {
  for( int i=0; i<ms; i++ ) {
    delayMicroseconds(1000);
  }
}

