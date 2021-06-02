////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//   Software       : SVTCONTROL                                              //
//   Layer          : src                                                     //
//   Package        : TEMP                                                    //
//   Component      : tempcontrol                                             //
//                                                                            //
//   File           : cdisplaylcd.cpp                                         //
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
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cdisplaylcd.h"

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

CDisplayLCD::CDisplayLCD() {
  lcd_a   = new LiquidCrystal_I2C(0x27, 20, 4);
} 

CDisplayLCD::~CDisplayLCD() {
  delete lcd_a ;
}

void CDisplayLCD::begin(){
  lcd_a->clear();
}  

void CDisplayLCD::init() {
  lcd_a->init();
  lcd_a->init();
  lcd_a->clear();  
  lcd_a->backlight();
}

void CDisplayLCD::setCursor(int col_p,int line_p) {
  lcd_a->setCursor(col_p, line_p);
}

void CDisplayLCD::write(const char *buf_p){
  lcd_a->print(buf_p);
} 

void CDisplayLCD::print(const char *buf_p){

  lcd_a->print(buf_p);
}  

void CDisplayLCD::print(String value_p){
  lcd_a->print(value_p);
}  

void CDisplayLCD::print(long value_p){
  lcd_a->print(value_p);
}  

void  CDisplayLCD::print(double &value_p,int fmt_p) {
  lcd_a->print(value_p,fmt_p);
}


void  CDisplayLCD::print(float &value_p,int fmt_p) {
  lcd_a->print(value_p,fmt_p);
}

