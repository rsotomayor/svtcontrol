////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//   Software       : SVTCONTROL                                              //
//   Layer          : src                                                     //
//   Package        : TEMP                                                    //
//   Component      : tempcontrol                                             //
//                                                                            //
//   File           : ckeypad.cpp                                             //
//   Author         : RSO ( Rafael Sotomayor Brûlé )                          //
//   EMail          : rsotomayor@savtec.cl                                    //
//   Type           : C++ Source                                              //
//   Usage          :                                                         //
//   Purpose        : Clase para keypad display                               //
//                                                                            //
//                                                                            //
//   who when        what                                                     //
//   ~~~ ~~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//   RSO 2021-06-01  Creation                                                 //
////////////////////////////////////////////////////////////////////////////////

#include "mydef.h"
#include "ckeypad.h"

#define I2CADDR 0x38

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
	{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C',},
	{'*','0','#','D'}
	};
byte rowPins[ROWS] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6, 7}; //connect to the column pinouts of the keypad
	
Keypad_I2C keypad_g = Keypad_I2C( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR );
//~ byte ledPin_g = 13; 

String strKeyCommand_g;

//take care of some special events
void keypadEvent(KeypadEvent key){
  //~ Serial.print("KEYPAD");
  switch (keypad_g.getState()){
    case PRESSED:
      //~ Serial.println(key);SLOGGER
      if ( strKeyCommand_g.length() > 3 ) {
        strKeyCommand_g="";
      } else  {
        strKeyCommand_g += key;
      }
      switch (key){
        case '#': 
          break;
        case '*': 
          strKeyCommand_g="";
          break;
        }
    break;
    case RELEASED:
      switch (key){
        case '*': 
          //~ digitalWrite(ledPin,!digitalRead(ledPin));
          //~ blink = false;
        break;
      }
    break;
    case HOLD:
      switch (key){
        case '*': 
          //~ blink = true; 
          break;
      }
    break;
  }
}



void procesaComando() {
  
  
}


void rxKeyPad() {

  char key = keypad_g.getKey();

  if ( key ) {
    if (  key == '#'  ) {
      procesaComando();
      strKeyCommand_g="";
    }
  }

}

