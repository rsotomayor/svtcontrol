////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//   Software       : SVTCONTROL                                              //
//   Layer          : src                                                     //
//   Package        : TEMP                                                    //
//   Component      : tempcontrol                                             //
//                                                                            //
//   File           : command.cpp                                             //
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
#include "command.h"

#include "cdisplaylcd.h"
#include "ccontrol.h"

extern CDisplayLCD   mydisplay_g;

#include <SoftwareSerial.h>
extern SoftwareSerial mySerial_g ;

int16_t      modbus_a[MAX_MODBUS];


uint16_t	crc_calc(char *request,char length_p){
  uint16_t	crc;
  const	uint16_t	Gen_Poly=0xA001;
  unsigned char	i,j;
  crc		=0xFFFF;
  bool	crc_bit;    
  
	for(j=0;j<length_p;j++){
		crc	=crc^(uint8_t)request[j];
		for(i=0;i<8;i++){
			crc_bit	=((crc & 0x0001)==0x0001);
			crc		=crc>>1;
			if(crc_bit)crc=crc^Gen_Poly;
		}
	}
	return	crc;
}


void rxDataDTU() {

  uint8_t rxbuffer[64];
  uint8_t index=0;

  while (mySerial_g.available() > 0) {
    uint8_t inByte = mySerial_g.read();
    Serial.print(inByte,HEX);
    Serial.print(" ");
    rxbuffer[index++] = inByte;
  }


  int rx_length = index-1 ;
  
  if ( rx_length > 4 ) {
    uint16_t crcCalc,crcRx;
    
    Serial.print("rx_length: ");Serial.println(rx_length);
    crcCalc = crc_calc(rxbuffer,rx_length-1);
    Serial.print("crcCalc: ");Serial.println(crcCalc,HEX);

    index   = rx_length - 1;
    crcRx   = (rxbuffer[index+1] << 8 ) | (rxbuffer[index] << 0) ;      

    Serial.print("crcRx: ");Serial.println(crcRx,HEX);

    if ( crcCalc == crcRx ) {
      uint8_t slv_address = rxbuffer[0];
      uint8_t slv_req     = rxbuffer[1];

      Serial.println("slv_address: "+String(slv_address));
      Serial.println("slv_req: "+ String(slv_req));

      if ( slv_address == SLAVE_ADDRESS && slv_req == 0x05 ) {
        uint16_t address,status_write; 
        index   = 2;
        address   = (rxbuffer[index] << 8 ) | (rxbuffer[index+1] << 0) ;      
        index   = 4;
        status_write = (rxbuffer[index] << 8 ) | (rxbuffer[index+1] << 0) ;      

        int pin,index;
        index = address - 16;

        pin = getOutputRelayPinByIndex(index);
        Serial.println("SETUPIO "+String(index) + " PIN "+String(pin) + " SW "+String(status_write));
  
        if ( pin != -1 ) {

          if ( (status_write >> 8)&0x00FF == 0xFF ) {
            digitalWrite(pin,LOW );
          } else if ( status_write == 0x00 ) {
            digitalWrite(pin,HIGH);
          } else {
            //~ Serial.println("0003");
          }

        }

        int       length_tx;
        uint16_t  crcTx;
        length_tx = sizeof(slv_address) + sizeof(slv_req) + sizeof(address) + sizeof(status_write) + sizeof(crcTx);
        
        uint8_t *pmodbus;
        pmodbus = new uint8_t[length_tx];
        index = 0 ;
        pmodbus[index++] = slv_address;
        pmodbus[index++] = slv_req;        
        pmodbus[index++] = (address >> 0 ) & 0xFF ;
        pmodbus[index++] = (address >> 8 ) & 0xFF ;        
        pmodbus[index++] = (status_write >> 0 ) & 0xFF ;
        pmodbus[index++] = (status_write >> 8 ) & 0xFF ;            
        crcTx = crc_calc(pmodbus,index);
        pmodbus[index++] = (crcTx >> 0 ) & 0xFF ;
        pmodbus[index++] = (crcTx >> 8 ) & 0xFF ;        
        Serial.print("coil crcTx: ");Serial.println(crcTx,HEX);
        Serial.print("coil tx Length: ");Serial.println(index);

        for ( int i=0;i<index;i++) {
          Serial.print(pmodbus[i],HEX);
        }
        mySerial_g.write(pmodbus,index);

        Serial.println("");
        
        delete pmodbus;    
        
      } else if ( slv_address == SLAVE_ADDRESS && slv_req== 0x03 ) {
        uint16_t address,count; 
        index   = 2;
        address = (rxbuffer[index] << 8 ) | (rxbuffer[index+1] << 0) ;      
        Serial.print("address: ");Serial.println(address);
        index   = 4;
        count   = (rxbuffer[index] << 8 ) | (rxbuffer[index+1] << 0) ;      
        Serial.print("*count: ");Serial.println(count);



        //~ Serial.println("**modbus_a[0] : " + String(modbus_a[0]));

        uint8_t   bytescount = count   ;
        uint16_t  crcTx;
        
        int length_tx;
        length_tx = sizeof(slv_address) + sizeof(slv_req) + sizeof(bytescount) + (bytescount*2) + sizeof(crcTx);
        
        uint8_t *pmodbus;
        pmodbus = new uint8_t[length_tx];
        
        index = 0 ;
        pmodbus[index++] = slv_address;
        pmodbus[index++] = slv_req;
        pmodbus[index++] = (bytescount*2);
        
        for ( int indexModbus = address ; indexModbus < ( address + bytescount ); indexModbus++ ) { 
          //~ Serial.println("indexModbus: " + String(indexModbus) + " bytescount: " + String(bytescount) + " MAX_MODBUS: " + String(MAX_MODBUS)+" Valor: "+String(modbus_a[indexModbus]));

          if ( indexModbus < MAX_MODBUS ) {
            pmodbus[index++] = (modbus_a[indexModbus] >> 8 ) & 0xFF ;
            pmodbus[index++] = (modbus_a[indexModbus] >> 0 ) & 0xFF ; 
          }
        }
        
        crcTx = crc_calc(pmodbus,index);
        pmodbus[index++] = (crcTx >> 0 ) & 0xFF ;
        pmodbus[index++] = (crcTx >> 8 ) & 0xFF ;        

        //~ Serial.print("coil crcTx: ");Serial.println(crcTx,HEX);
        //~ Serial.print("length_tx: ");Serial.println(length_tx);
        //~ Serial.print("coil tx Length: ");Serial.println(index);

        for ( int i=0;i<index;i++) {
          Serial.print(pmodbus[i],HEX);
        }
        mySerial_g.write(pmodbus,index);
        Serial.println("");
        delete pmodbus;        

      } 
    }
  }


}


void writeModBusMap() {
//~ 30001	VFW   
//~ 30002	Numero de Input 
//~ 30003	Numero de Output 
//~ 30004	Numero de Relay
//~ 30005	Numero de TLI
//~ 30006	Numero de Analog 
//~ 30007	contadorTsUp 1 MSB 
//~ 30008	contadorTsUp 1 LSB 
//~ 30009	contador 1 MSB 
//~ 30010	contador 1 LSB 
//~ 30011	contador 2 MSB 
//~ 30012	contador 2 LSB 
//~ 30013	contador 3 MSB 
//~ 30014	contador 3 LSB 
//~ 30015	contador 4 MSB 
//~ 30016	contador 4 LSB 
//~ 30017	Contador 5 MSB 
//~ 30018	Contador 5 LSB 
//~ 30019	Contador 6 MSB 
//~ 30020	Contador 6 LSB 
//~ 30021	A0
//~ 30022	A1
//~ 30023	A2
//~ 30024	A3
//~ 30025	A4
//~ 30026	A5
//~ 30027	A6
//~ 30028	A7
//~ 30029	estadoInput  MSB
//~ 30030	estadoInput  LSB
//~ 30031	estadoOutput MSB
//~ 30032	estadoOutput LSB
//~ 30033	estadoRelay  MSB
//~ 30034	estadoRelay  LSB
//~ 30035	TL1
//~ 30036	TL2
//~ 30037	TL3
//~ 30038	TL4
//~ 30039	TL5
//~ 30040	TL6
//~ 30041	TL7
//~ 30042	TL8
//~ 30043	TL9
//~ 30044	TL10
//~ 30045	TL11
//~ 30046	TL12
//~ 30047	TL13
//~ 30048	TL14
//~ 30049	TL15
//~ 30050	TL16

  int index=0;

  modbus_a[index++] = atoi(VERSION_SOFTWARE);
  modbus_a[index++] = NINPUT;
  modbus_a[index++] = NOUTPUT_CONTROL;   
  modbus_a[index++] = NOUTPUT_RELAY;   
  modbus_a[index++] = NSENSOR_TEMP;  
  modbus_a[index++] = NSENSOR_ANALOG;  

  modbus_a[index++] = (counterTsUp_a >> 0  ) & 0xFFFF ;
  modbus_a[index++] = (counterTsUp_a >> 16 ) & 0xFFFF ;

  modbus_a[index++] = (contador_g[0] >> 0  ) & 0xFFFF ;
  modbus_a[index++] = (contador_g[0] >> 16 ) & 0xFFFF ;
  modbus_a[index++] = (contador_g[1] >> 0  ) & 0xFFFF ;
  modbus_a[index++] = (contador_g[1] >> 16 ) & 0xFFFF ;
  modbus_a[index++] = (contador_g[2] >> 0  ) & 0xFFFF ;
  modbus_a[index++] = (contador_g[2] >> 16 ) & 0xFFFF ;
  modbus_a[index++] = (contador_g[3] >> 0  ) & 0xFFFF ;
  modbus_a[index++] = (contador_g[3] >> 16 ) & 0xFFFF ;
  modbus_a[index++] = (contador_g[4] >> 0  ) & 0xFFFF ;
  modbus_a[index++] = (contador_g[4] >> 16 ) & 0xFFFF ;
  modbus_a[index++] = (contador_g[5] >> 0  ) & 0xFFFF ;
  modbus_a[index++] = (contador_g[5] >> 16 ) & 0xFFFF ;


  modbus_a[index++] = analogRead(A0);
  modbus_a[index++] = analogRead(A1);
  modbus_a[index++] = analogRead(A2);
  modbus_a[index++] = analogRead(A3);
  modbus_a[index++] = analogRead(A4);
  modbus_a[index++] = analogRead(A5);
  modbus_a[index++] = analogRead(A6);
  modbus_a[index++] = analogRead(A7);


  modbus_a[index++] = (estadoinput_a >> 0  ) & 0xFFFF ;
  modbus_a[index++] = (estadoinput_a >> 16 ) & 0xFFFF ;

  modbus_a[index++] = (estadooutput_a >> 0  ) & 0xFFFF ;
  modbus_a[index++] = (estadooutput_a >> 16 ) & 0xFFFF ;

  modbus_a[index++] = (estadorelay_a >> 0  ) & 0xFFFF ;
  modbus_a[index++] = (estadorelay_a >> 16 ) & 0xFFFF ;


  modbus_a[index++] = (int16_t) (temperaturaagua_a[0]*100.0) ;
  modbus_a[index++] = (int16_t) (temperaturaagua_a[1]*100.0) ;
  modbus_a[index++] = (int16_t) (temperaturaagua_a[2]*100.0) ;
  modbus_a[index++] = (int16_t) (temperaturaagua_a[3]*100.0) ;
  modbus_a[index++] = (int16_t) (temperaturaagua_a[4]*100.0) ;
  modbus_a[index++] = (int16_t) (temperaturaagua_a[5]*100.0) ;

  modbus_a[index++] = (int16_t) (temperaturaagua_a[6]*100.0) ;
  modbus_a[index++] = (int16_t) (temperaturaagua_a[7]*100.0) ;
  modbus_a[index++] = (int16_t) (temperaturaagua_a[8]*100.0) ;
  modbus_a[index++] = (int16_t) (temperaturaagua_a[9]*100.0) ;
  modbus_a[index++] = (int16_t) (temperaturaagua_a[10]*100.0) ;
  modbus_a[index++] = (int16_t) (temperaturaagua_a[11]*100.0) ;
  modbus_a[index++] = (int16_t) (temperaturaagua_a[12]*100.0) ;
  modbus_a[index++] = (int16_t) (temperaturaagua_a[13]*100.0) ;
  modbus_a[index++] = (int16_t) (temperaturaagua_a[14]*100.0) ;
  modbus_a[index++] = (int16_t) (temperaturaagua_a[15]*100.0) ;

  
}


void  setuplcd() {
  mydisplay_g.init();

  mydisplay_g.setCursor(0,0);
  mydisplay_g.print("                     ");

  mydisplay_g.setCursor(0,0);
  mydisplay_g.print(F("V:"));
  mydisplay_g.print(VERSION_SOFTWARE);
}


void displayLCDControl() {

  displayLCDPage(pageDisplay_g);

  
}


void displayLCDPage(int page_p) {
  
 
  if ( page_p == 0 ) {
    
    String strDisplay;
    strDisplay = counterTsUp_a;

    mydisplay_g.setCursor(0,0);
    mydisplay_g.print("                    ");
    mydisplay_g.setCursor(0,0);
    mydisplay_g.print(F("V:"));
    mydisplay_g.print(VERSION_SOFTWARE);
    mydisplay_g.print(" PAGE: "+String(page_p));
    
    mydisplay_g.setCursor(0,1);
    mydisplay_g.print("                    ");
    mydisplay_g.setCursor(0,2);
    mydisplay_g.print("                    ");
    mydisplay_g.setCursor(0,3);
    mydisplay_g.print("                    ");
    mydisplay_g.setCursor(0,3);
    mydisplay_g.print(strDisplay.c_str());


    
  } else if ( page_p == 1 ) {
    

    String strDisplay;
    strDisplay = counterTsUp_a;

    mydisplay_g.setCursor(0,0);
    mydisplay_g.print("                    ");
    mydisplay_g.setCursor(0,0);
    mydisplay_g.print(F("V:"));
    mydisplay_g.print(VERSION_SOFTWARE);
    mydisplay_g.print(" PAGE: "+String(page_p));

    mydisplay_g.setCursor(0,1);
    mydisplay_g.print("                    ");
    mydisplay_g.setCursor(0,2);
    mydisplay_g.print("                    ");
    mydisplay_g.setCursor(0,3);
    mydisplay_g.print("                    ");
    mydisplay_g.setCursor(0,3);
    mydisplay_g.print(strDisplay.c_str());

    
  } else if ( page_p == 2 ) {
  
    String strDisplay;
    strDisplay = counterTsUp_a;

    mydisplay_g.setCursor(0,0);
    mydisplay_g.print("                    ");
    mydisplay_g.setCursor(0,0);
    mydisplay_g.print(F("V:"));
    mydisplay_g.print(VERSION_SOFTWARE);
    mydisplay_g.print(" PAGE: "+String(page_p));

    mydisplay_g.setCursor(0,1);
    mydisplay_g.print("                    ");
    mydisplay_g.setCursor(0,2);
    mydisplay_g.print("                    ");
    mydisplay_g.setCursor(0,3);
    mydisplay_g.print("                    ");
    mydisplay_g.setCursor(0,3);
    mydisplay_g.print(strDisplay.c_str());
  

  } else if ( page_p == 3 ) {
    
    String strDisplay;
    strDisplay = counterTsUp_a;

    mydisplay_g.setCursor(0,0);
    mydisplay_g.print("                    ");
    mydisplay_g.setCursor(0,0);
    mydisplay_g.print(F("V:"));
    mydisplay_g.print(VERSION_SOFTWARE);
    mydisplay_g.print(" PAGE: "+String(page_p));

    mydisplay_g.setCursor(0,1);
    mydisplay_g.print("                    ");
    mydisplay_g.setCursor(0,2);
    mydisplay_g.print("                    ");
    mydisplay_g.setCursor(0,3);
    mydisplay_g.print("                    ");
    mydisplay_g.setCursor(0,3);
    mydisplay_g.print(strDisplay.c_str());
    
    
  } else {
    
    String strDisplay;
    strDisplay = counterTsUp_a;

    mydisplay_g.setCursor(0,0);
    mydisplay_g.print("                    ");
    mydisplay_g.setCursor(0,0);
    mydisplay_g.print(F("V:"));
    mydisplay_g.print(VERSION_SOFTWARE);
    mydisplay_g.print(" **PAGE: "+String(page_p));

    mydisplay_g.setCursor(0,1);
    mydisplay_g.print("                    ");
    mydisplay_g.setCursor(0,2);
    mydisplay_g.print("                    ");
    mydisplay_g.setCursor(0,3);
    mydisplay_g.print("                    ");
    mydisplay_g.setCursor(0,3);
    mydisplay_g.print(strDisplay.c_str());
    
    
  }
  
  
}


