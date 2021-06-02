////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//   Software       : SVTCONTROL                                              //
//   Layer          : src                                                     //
//   Package        : TEMP                                                    //
//   Component      : tempcontrol                                             //
//                                                                            //
//   File           : ckeypad.h                                               //
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


#ifndef __CKEYPAD_H__
#define __CKEYPAD_H__

#include "mydef.h"

#include <Keypad_I2C.h>
#include <Keypad.h>          // GDY120705
#include <Wire.h>

extern void rxKeyPad();
extern void keypadEvent(KeypadEvent key);
extern void procesaComando() ;
extern Keypad_I2C keypad_g;
extern String strKeyCommand_g;

#endif


