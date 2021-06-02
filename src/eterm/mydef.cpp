////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//   Software       : SVTCONTROL                                              //
//   Layer          : src                                                     //
//   Package        : TEMP                                                    //
//   Component      : tempcontrol                                             //
//                                                                            //
//   File           : mydef.cpp                                               //
//   Author         : RSO ( Rafael Sotomayor Brûlé )                          //
//   EMail          : rsotomayor@savtec.cl                                    //
//   Type           : C++ Source                                              //
//   Usage          :                                                         //
//   Purpose        : Definición de defines                                   //
//                                                                            //
//                                                                            //
//   who when        what                                                     //
//   ~~~ ~~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//   RSO 2021-06-01  Creation                                                 //
////////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>

#include "mydef.h"

int  getInputPinByIndex(int index_p) {
  int retval = -1 ;
  switch ( index_p ) {
    case  0: retval = 22 ; break;
    case  1: retval = 23 ; break;
    case  2: retval = 24 ; break;
    case  3: retval = 25 ; break;
    case  4: retval = 26 ; break;
    case  5: retval = 27 ; break;
    case  6: retval = 28 ; break;
    case  7: retval = 29 ; break;
    case  8: retval = 30 ; break;
    case  9: retval = 31 ; break;
    case 10: retval = 32 ; break;
    case 11: retval = 33 ; break;
    default: retval = -1 ; break;
  }
  
  return retval;
  
}

int  getOutputRelayPinByIndex(int index_p) {
  int retval = -1 ;
  switch ( index_p ) {
    case  0: retval = 34 ; break;
    case  1: retval = 35 ; break;
    case  2: retval = 36 ; break;
    case  3: retval = 37 ; break;
    case  4: retval = 38 ; break;
    case  5: retval = 39 ; break;
    case  6: retval = 40 ; break;
    case  7: retval = 41 ; break;
    case  8: retval = 42 ; break;
    case  9: retval = 43 ; break;
    case 10: retval = 44 ; break;
    case 11: retval = 45 ; break;
    default: retval = -1 ; break;
  }
  
  return retval;
  
}


int  getOutputControlPinByIndex(int index_p) {
  int retval = -1 ;
  

  switch ( index_p ) {
    case  0: retval = 46 ; break;
    case  1: retval = 47 ; break;
    case  2: retval = 48 ; break;
    case  3: retval = 49 ; break;
    case  4: retval = A15 ; break;
    case  5: retval = A14 ; break;
    case  6: retval = A13; break;
    case  7: retval = A12 ; break;
    case  8: retval = A11 ; break;
    case  9: retval = A10 ; break;
    case 10: retval = A9 ; break;
    case 11: retval = A8 ; break;
    default: retval = -1 ; break;
  }
  
  return retval;
  
}


int  getContadorByIndex(int index_p) {
  int retval = -1 ;

  switch ( index_p ) {
    case  0: retval = 4 ; break;
    case  1: retval = 5 ; break;
    case  2: retval = 6 ; break;
    case  3: retval = 7 ; break;
    case  4: retval = 8 ; break;
    case  5: retval = 9 ; break;
    default: retval = -1 ; break;
  }
  
  return retval;
}




