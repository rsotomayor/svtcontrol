////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//   Software       : SVTCONTROL                                              //
//   Layer          : src                                                     //
//   Package        : TEMP                                                    //
//   Component      : tempcontrol                                             //
//                                                                            //
//   File           : mydef.h                                                 //
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

#ifndef __MYDEF_H__
#define __MYDEF_H__


#define NSENSOR_TEMP                16
#define TSAMPLE_MEDICION_DS18B20  5000  // milli segundos
#define NSENSOR_ANALOG               8
#define NCONTADORES                  6
#define DEBOUNCE_DELAY              50

#define NINPUT                      12
#define NOUTPUT_CONTROL             12
#define NOUTPUT_RELAY               12

#define PIN_DS18B20                  7
#define PIN_TX1                     18
#define PIN_RX1                     19
#define PIN_SDA                     20
#define PIN_SCL                     21

#define TIMER_1                  10000

#define VERSION_SOFTWARE           100

#define MAX_MODBUS                  64
#define SLAVE_ADDRESS               99

int getInputPinByIndex(int index_p);
int getOutputRelayPinByIndex(int index_p);
int getOutputControlPinByIndex(int index_p);
int getContadorByIndex(int index_p);


#endif


