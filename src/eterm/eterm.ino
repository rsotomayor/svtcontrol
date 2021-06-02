#include "mydef.h"

#include <avr/wdt.h>
#include <TimerOne.h>
#include <SoftwareSerial.h>

#include <cglobal.h>
#include "cdisplaylcd.h"
#include "ckeypad.h"
#include "ccontrol.h"
#include "command.h"


CDisplayLCD    mydisplay_g;
SoftwareSerial mySerial_g(PIN_RX1, PIN_TX1); // RX, TX


void callbackTimer() {
  incrementaContadores();
  readContadores();
  
}


void setup()
{
  wdt_disable(); 
  mySerial_g.begin(9600);

 	Serial.begin(9600);
  delay(500);
  
  Serial.print(F("Starting ..... Version: "));
  Serial.println(VERSION_SOFTWARE);

  keypad_g.begin( );          // GDY120705
  keypad_g.addEventListener(keypadEvent); //add an event listener for this keypad

  Serial.println(F("Timer Config"));
  Timer1.attachInterrupt(callbackTimer);
  Timer1.initialize(TIMER_1);
  initControl();


  wdt_enable(WDTO_4S);
  Serial.println(F("SETUP OK"));
  
  setuplcd();
  
}

int nReset_g=0 ;


void loop() {

  rxKeyPad();                 // Lee teclado y ejecuta acción
  readSensorDS18B20();        // Lee Sensores de T°
  readDigitalInput();         // Lee entradas digitales
  ejecutaControl();           // Ejecuta control
  readDigitalOutput();        // Lee salidas digitales
  readRelayOutput();          // Lee salidas relay
  displayLCDControl();        // Display Variables
  
  writeModBusMap();           // Escribe variables en arreglo ModBus
  rxDataDTU();                // Lee ModBus desde la DTU 
  
  Serial.println("Ejecutando");
  wdt_reset();                // Wach Dog Timer, tiene que llegar acá al cabo de 4 seg
  delay(1000);

}
