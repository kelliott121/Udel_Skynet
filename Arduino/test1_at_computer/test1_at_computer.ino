#include <Wire.h>
#include <MirfSpiDriver.h>
#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>
#include <nRF24L01.h>
#include <SPI.h>

String str_from_comp;
char char_from_comp;

byte[5] outgoing_msg;
byte[5] incoming_msg;
int msg_id;

void setup() {       
  str_from_comp = "";
  char_from_comp = 0;  
  Serial.begin(9600);  
  Mirf.csnPin = 3;
  Mirf.cePin = 9;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.payload = 5;
  Mirf.channel = 0;
  Mirf.config();
  Mirf.setTADDR((byte *)"TX_01");
  Mirf.setRADDR((byte *)"TX_01");  
  msg_id = 0;
}

void loop() {

  while(Serial.available()) {
    char_from_comp = Serial.read();
    str_from_comp.concat(char_from_comp);
    if(char_from_comp == 0x04){
      SendCommand();
      str_from_comp = "";
      char_from_comp = 0;        
    }
  }
  
}

void SendCommand(){  
  byte c[4];
  boolean got_ack = false;
  

  while (!got_ack){
    Mirf.send((byte*)carray);
    while( Mirf.isSending() );
    delay(100);
    if (Mirf.dataReady()){
      Mirf.getData(c);
      if(strcmp((char*)c, "ACK"))
        got_ack = true;
        msg_id++;
    }
  }
}

void ConvertStringToByteArray(){
  char carray[str_from_comp.length()+1];
  str_from_comp.toCharArray(carray, sizeof(carray));
  
  outgoing_msg
}
