/*
 * Based on Obdev's AVRUSB code and under the same license.
 *
 * TODO: Make a proper file header. :-)
 */
#ifndef __UsbKeyboard_h__
#define __UsbKeyboard_h__

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <string.h>

#include "usbdrv.h"

// TODO: Work around Arduino 12 issues better.
//#include <WConstants.h>
//#undef int()

typedef uint8_t byte;


#define BUFFER_SIZE 7


static uchar    idleRate;           // in 4 ms units 


/* We use a simplifed keyboard report descriptor which does not support the
 * boot protocol. We don't allow setting status LEDs and but we do allow
 * simultaneous key presses. 
 * The report descriptor has been created with usb.org's "HID Descriptor Tool"
 * which can be downloaded from http://www.usb.org/developers/hidpage/.
 * Redundant entries (such as LOGICAL_MINIMUM and USAGE_PAGE) have been omitted
 * for the second INPUT item.
 */
PROGMEM char usbHidReportDescriptor[35] = { /* USB report descriptor */
  0x05, 0x01,                    // USAGE_PAGE (Generic Desktop) 
  0x09, 0x06,                    // USAGE (Keyboard) 
  0xa1, 0x01,                    // COLLECTION (Application) 
  0x05, 0x07,                    //   USAGE_PAGE (Keyboard) 
  0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl) 
  0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI) 
  0x15, 0x00,                    //   LOGICAL_MINIMUM (0) 
  0x25, 0x01,                    //   LOGICAL_MAXIMUM (1) 
  0x75, 0x01,                    //   REPORT_SIZE (1) 
  0x95, 0x08,                    //   REPORT_COUNT (8) 
  0x81, 0x02,                    //   INPUT (Data,Var,Abs) 
  0x95, BUFFER_SIZE-1,           //   REPORT_COUNT (simultaneous keystrokes) 
  0x75, 0x08,                    //   REPORT_SIZE (8) 
  0x25, 0x65,                    //   LOGICAL_MAXIMUM (101) 
  0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated)) 
  0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application) 
  0x81, 0x00,                    //   INPUT (Data,Ary,Abs) 
  0xc0                           // END_COLLECTION 
};



/* Keyboard usage values, see usb.org's HID-usage-tables document, chapter
 * 10 Keyboard/Keypad Page for more codes.
 */
#define MOD_CONTROL_LEFT    (1<<0)
#define MOD_SHIFT_LEFT      (1<<1)
#define MOD_ALT_LEFT        (1<<2)
#define MOD_GUI_LEFT        (1<<3)
#define MOD_CONTROL_RIGHT   (1<<4)
#define MOD_SHIFT_RIGHT     (1<<5)
#define MOD_ALT_RIGHT       (1<<6)
#define MOD_GUI_RIGHT       (1<<7)

#define KEY_LEFT_CTRL   0x80
#define KEY_LEFT_SHIFT    0x81
#define KEY_LEFT_ALT    0x82
#define KEY_LEFT_GUI    0x83
#define KEY_RIGHT_CTRL    0x84
#define KEY_RIGHT_SHIFT   0x85
#define KEY_RIGHT_ALT   0x86
#define KEY_RIGHT_GUI   0x87

#define KEY_A       4
#define KEY_B       5
#define KEY_C       6
#define KEY_D       7
#define KEY_E       8
#define KEY_F       9
#define KEY_G       10
#define KEY_H       11
#define KEY_I       12
#define KEY_J       13
#define KEY_K       14
#define KEY_L       15
#define KEY_M       16
#define KEY_N       17
#define KEY_O       18
#define KEY_P       19
#define KEY_Q       20
#define KEY_R       21
#define KEY_S       22
#define KEY_T       23
#define KEY_U       24
#define KEY_V       25
#define KEY_W       26
#define KEY_X       27
#define KEY_Y       28
#define KEY_Z       29
#define KEY_1       30
#define KEY_2       31
#define KEY_3       32
#define KEY_4       33
#define KEY_5       34
#define KEY_6       35
#define KEY_7       36
#define KEY_8       37
#define KEY_9       38
#define KEY_0       39

#define KEY_ENTER   40

#define KEY_SPACE   44

#define KEY_F1      58
#define KEY_F2      59
#define KEY_F3      60
#define KEY_F4      61
#define KEY_F5      62
#define KEY_F6      63
#define KEY_F7      64
#define KEY_F8      65
#define KEY_F9      66
#define KEY_F10     67
#define KEY_F11     68
#define KEY_F12     69

#define KEY_ARROW_LEFT 0x50
#define KEY_ARROW_RIGHT 0x4F
#define KEY_ARROW_UP 0x52
#define KEY_ARROW_DOWN 0x51

//  Low level key report: up to 6 keys and shift, ctrl etc at once
#define START_MODIFIERS	0
#define START_KEYS		1



class UsbKeyboardDevice {
 public:
  
  uint8_t usbReport[BUFFER_SIZE];
 
  UsbKeyboardDevice () {
    PORTD = 0; // TODO: Only for USB pins?
    DDRD |= ~USBMASK;

    cli();
    usbDeviceDisconnect();
    usbDeviceConnect();


    usbInit();
      
    sei();

    //reset all
	for (int i=START_MODIFIERS; i<BUFFER_SIZE; i++) {
		usbReport[i] = 0x00;
	}  
	
    usbSetInterrupt(usbReport, sizeof(usbReport));
  }
    
  void update() {
    usbPoll();
  }
  
  void resetAll(){
	//reset all
	for (int i=START_MODIFIERS; i<BUFFER_SIZE; i++) {
		usbReport[i] = 0x00;
	}  
	
    usbSetInterrupt(usbReport, sizeof(usbReport));
  }
    
  void release(byte keyStroke){
	
	while (!usbInterruptIsReady()) {
		// Note: We wait until we can send keystroke
		//       so we know the previous keystroke was
		//       sent.
	}
	
	if(keyStroke==KEY_RIGHT_SHIFT){
		usbReport[START_MODIFIERS] = 0x00;
	} else {
		
		for (int i=START_KEYS; i<BUFFER_SIZE; i++) {
			if (usbReport[i] == keyStroke) {
				usbReport[i] = 0x00;
			}
		}
	}
	usbSetInterrupt(usbReport, sizeof(usbReport));
		
	while (!usbInterruptIsReady()) {
	  // Note: We wait until we can send keystroke
	  //       so we know the previous keystroke was
	  //       sent.
	}
	
  }
  
  void press(byte keyStroke) {
	
	while (!usbInterruptIsReady()) {
		// Note: We wait until we can send keystroke
		//       so we know the previous keystroke was
		//       sent.
	}
    
	if(keyStroke==KEY_RIGHT_SHIFT){
		usbReport[START_MODIFIERS] = MOD_SHIFT_RIGHT;
	} else {
		//not use modifiers
		usbReport[START_MODIFIERS] = 0x00;
		
		int alreadyPressed = 0;
		
		for (int i=START_KEYS; i<BUFFER_SIZE; i++) {
			if (usbReport[i] == keyStroke) {
				alreadyPressed = 1;
				break;
			}
		}
		
		if (alreadyPressed==0) {
			
			for (int i=START_KEYS; i<BUFFER_SIZE; i++) {
				if (usbReport[i] == 0x00) {
					usbReport[i] = keyStroke;
					break;
				}
			}
			
		}
	
    }
	
	usbSetInterrupt(usbReport, sizeof(usbReport));
		
	while (!usbInterruptIsReady()) {
	  // Note: We wait until we can send keystroke
	  //       so we know the previous keystroke was
	  //       sent.
	}

    
  }

};

UsbKeyboardDevice UsbKeyboard = UsbKeyboardDevice();

#ifdef __cplusplus
extern "C"{
#endif 
  // USB_PUBLIC uchar usbFunctionSetup
uchar usbFunctionSetup(uchar data[8]) 
  {
    usbRequest_t    *rq = (usbRequest_t *)((void *)data);

    usbMsgPtr = UsbKeyboard.usbReport; //
    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){
      /* class request type */

      if(rq->bRequest == USBRQ_HID_GET_REPORT){
	/* wValue: ReportType (highbyte), ReportID (lowbyte) */

	/* we only have one report type, so don't look at wValue */
        // TODO: Ensure it's okay not to return anything here?    
	return 0;

      }else if(rq->bRequest == USBRQ_HID_GET_IDLE){
	//            usbMsgPtr = &idleRate;
	//            return 1;
	return 0;
      }else if(rq->bRequest == USBRQ_HID_SET_IDLE){
	idleRate = rq->wValue.bytes[1];
      }
    }else{
      /* no vendor specific requests implemented */
    }
    return 0;
  }
#ifdef __cplusplus
} // extern "C"
#endif


#endif // __UsbKeyboard_h__
