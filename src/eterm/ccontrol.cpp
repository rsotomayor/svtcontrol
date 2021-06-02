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
#include "ccontrol.h"

#include "cdisplaylcd.h"
extern CDisplayLCD   mydisplay_g;


float               temperaturaagua_a[NSENSOR_TEMP]; 
unsigned long       previousMillisTsampleDS18B20_a;

OneWire             *oneWire_a;
DallasTemperature   *sensors_a;  

unsigned  long      estadoinput_a;
unsigned  long      estadooutput_a;
unsigned  long      estadorelay_a;
unsigned long       counterTsUp_a;
unsigned long       tsTimer_a;


unsigned long lastDebounceTime_a[NCONTADORES];
uint8_t       lastButtonState_a[NCONTADORES];
uint8_t       buttonState_a[NCONTADORES] ;
uint8_t       contador_a[NCONTADORES];  
bool          flagContador_a;


uint32_t      contador_g[NCONTADORES];


void  initControl() {
  
  analogRead(A0);
    
  initVariablesControl();
  initDS18B20();
}

void  initVariablesControl() {
  previousMillisTsampleDS18B20_a = 0 ; 
  

  estadooutput_a      = 0x00000000 ;
  estadoinput_a       = 0x00000000 ;
  estadorelay_a       = 0x00000000 ;  
 
  tsTimer_a           = 0 ;
  counterTsUp_a       = 0 ;
  flagContador_a      = false ;
 
  initContadores();
 
}


void initContadores() {
  for ( int i=0;i<NCONTADORES;i++) {
    int pin;
    pin   = getContadorByIndex(i);
    pinMode (pin, INPUT);
    digitalWrite(pin, HIGH);       // turn on pullup resistors 
  }



  for ( int i=0;i<NCONTADORES;i++) {
    contador_a[i]           = 0 ;
    lastButtonState_a[i]    = 0 ;
    lastDebounceTime_a[i]   = 0 ;    
  }


}

void readContadores() {

  uint8_t dummy;

  for ( int i=0 ;i < NCONTADORES ;i++) {
    int pin;
    pin   = getContadorByIndex(i);
    dummy = digitalRead(pin);
    if ( dummy != lastButtonState_a[i]  ) {
      lastDebounceTime_a[i]   = millis();
    }
    
    if ((millis() - lastDebounceTime_a[i]) > DEBOUNCE_DELAY) {
      // cualquiera que sea la lectura que ha estado ahí por mas tiempo
      // que el retardo de entrada se toma como el estado actual real:
      // si el estado del pulsador cambia:
      if (dummy != buttonState_a[i] ) {
        
        flagContador_a       = true ;
        buttonState_a[i]     = dummy;
        if ( contador_g[i] > 99999 ) {
          contador_g[i] = 0 ;  
        } else {
          contador_g[i] = contador_g[i] + (contador_a[i]++)%2;
        }
        
      }
    }
    
    lastButtonState_a[i] = dummy;    
    
    
  }
}


void readSensorDS18B20() {
  unsigned long deltaT;
  deltaT = millis() - previousMillisTsampleDS18B20_a;

  if (  deltaT > TSAMPLE_MEDICION_DS18B20 ) {
    previousMillisTsampleDS18B20_a = millis();
    readDS18B20();
  }
}

void initDS18B20() {
  oneWire_a = new OneWire(PIN_DS18B20);
  sensors_a = new DallasTemperature(oneWire_a);  
}

void endDS18B20() {
  delete oneWire_a;
  delete sensors_a;
}

int readDS18B20() {
  int     retval = -1 ;
  float   dummy;

  sensors_a->requestTemperatures(); 
  
  for ( int i=0;i<NSENSOR_TEMP;i++) {
    temperaturaagua_a[i] = sensors_a->getTempCByIndex(i);
    //~ temperaturaagua_a[i] = (float) 1.0*random(10, 30);
 
    if ( !(temperaturaagua_a[i] > -400 && temperaturaagua_a[i] < 1000) ) {
      retval = -1;
      temperaturaagua_a[i] = -400 ;
    } else {
      retval = 0 ;
    }
  }


}

void  readDigitalInput() {
  int val;
  int pin;
  estadoinput_a       = 0x00000000;

  for ( int i=0;i<NINPUT;i++) {
    pin   = getInputPinByIndex(i);
    val   = !digitalRead(pin);
    estadoinput_a |= ( val << i ) ;     
  }
  
}

void  readRelayOutput() {
  
  int val;
  int pin;

  estadorelay_a       = 0x00000000;

  for ( int i=0;i<NOUTPUT_RELAY;i++) {
    pin   = getOutputRelayPinByIndex(i);
    val   = !digitalRead(pin);
    estadorelay_a |= ( val << i ) ;     
  }
  
}

void  readDigitalOutput() {
  
  int val;
  int pin;

  estadooutput_a       = 0x00000000;

  for ( int i=0;i<NOUTPUT_CONTROL;i++) {
    pin   = getOutputControlPinByIndex(i);
    val   = !digitalRead(pin);
    estadooutput_a |= ( val << i ) ;     
  }
    
  
}


void incrementaContadores() {
  if ( (++tsTimer_a)%100 == 0 ) {
    counterTsUp_a++;
  }
  
};




/*
REQUERIMIENTO ALTO NIVEL:
======================
Controlar Temperatura de 12 sectores de la casa mediante lectura de temperatura en tubería y lectura de contacto seco termostato de la pieza.

ESPECIFICACIONES TÉCNICAS DEL EQUIPO
=====================================
Funcional
* MODO DE OPERACIÓN BOMBA DE CALOR : Este modo corresponde cuando la casa opera con bomba de calor, temperatura IDA primario menor o igual a 50 °C . Si la señal del termostato es ON el sistema abre válvula de termostáticas de 3 vía a 100% y encienden bomba monofásica del sector. Si la señal del termostato es OFF el sistema cierra válvula de termostáticas de 3 vía a 0% y apaga bomba monofásica del sector.
* MODO DE OPERACIÓN BOMBA DE CALDERA : Este modo corresponde cuando la casa opera con bomba de calor, temperatura IDA del primario mayor a 50 °C . Si la señal del termostato es ON el sistema modula con control PID válvula de termostáticas de 3 vías para mantener temperatura de IDA entre 35 y 45 °C, y encienden bomba monofásica del sector. Si la señal del termostato es OFF el sistema cierra válvula de termostáticas de 3 vía al 100% y apaga bomba monofásica del sector. 


ENTRADA:
16 Sensores de T° 1-Wire DS18B20 
4 Sensores Temperatura para monitoreo ( Bomba de Calro, Retorno Primario, Bioler 1 y Boiler 2 )
12 Sensores de Temperatura para Control 
12 Lecturas de Contacto Seco
1  Detección de Modo operación ( Por confirmar como se detecta el modo )

SALIDAS
12 Actuaciones de válvula de 3 vías ( 0 - 100 % )
12 Actuaciones Relay para valvulas monofásicas de de recirculación

INTERFACE:
DISPLAY LCD 20x4 + TECLADO I2C
RS485 PARA ACCEDER A MAPA DE MEMORIA DE VARIABLES MODBUS

*/



void  ejecutaControl() {
    
    
}




