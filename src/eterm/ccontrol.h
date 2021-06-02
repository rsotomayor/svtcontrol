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

#ifndef __CCONTROL_H__
#define __CCONTROL_H__

#include <OneWire.h>
#include <DallasTemperature.h>

void    initControl();
void    initVariablesControl();
void    initDS18B20();
int     readDS18B20() ;
void    readSensorDS18B20();

void    displayLCDControl();
void    readDigitalInput();
void    ejecutaControl();
void    readDigitalOutput();
void    readRelayOutput();
void    initContadores();
void    readContadores();
void    incrementaContadores();

extern unsigned  long      estadoinput_a;
extern unsigned  long      estadooutput_a;
extern unsigned  long      estadorelay_a;
extern unsigned  long      counterTsUp_a;
extern unsigned  long      tsTimer_a;

extern uint32_t            contador_g[];
extern float               temperaturaagua_a[]; 


#endif

