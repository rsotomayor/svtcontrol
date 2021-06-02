////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//   Software       : SVTCONTROL                                              //
//   Layer          : src                                                     //
//   Package        : TEMP                                                    //
//   Component      : tempcontrol                                             //
//                                                                            //
//   File           : cdisplaylcd.h                                           //
//   Author         : RSO ( Rafael Sotomayor Brûlé )                          //
//   EMail          : rsotomayor@savtec.cl                                    //
//   Type           : C++ Source                                              //
//   Usage          :                                                         //
//   Purpose        : Clase para keypad display                               //
//                                                                            //
//                                                                            //
//   who when        what                                                     //
//   ~~~ ~~~~~~~~ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//   RSO 31-05-2016  Creation                                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef __CDISPLAYLCD_H__
#define __CDISPLAYLCD_H__

#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

class CDisplayLCD {

public:
  CDisplayLCD();
  ~CDisplayLCD();
  void            init();  
  void            begin();
  void            print(const char *buf_p);
  void            print(long value_p);  
  void            print(double &value_p,int fmt_p); 
  void            print(float &value_p,int fmt_p);  
  void            print(String value_p);
  void            setCursor(int line_p,int col_p);
  void            write(const char *buf_p);

protected:

private:
  void    mapKey(int &key_p);
  void    procesaKey(int key_p);
  int     getKey(int value_p);
  LiquidCrystal_I2C *lcd_a;  
} ;

#endif
