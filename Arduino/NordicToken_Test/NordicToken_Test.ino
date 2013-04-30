#include <Conversation.h>

#include <SPI.h>

#include <MirfSpiDriver.h>
#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>
#include <nRF24L01.h>

#include <Packet.h>

#include <Nordic.h>

//byte id = 0;
byte id = 1;

Nordic nord(id);

void setup() {
  Serial.begin(9600);
  Serial.println("Beginning Test");
}

void loop() {
  if (id){
    Packet pkt;
    while (pkt.getSourceId() == NULLDATA){
      pkt = nord.waitForToken(1000);
    }
    byte * data = pkt.getData();
  }
  else{
    nord.giveToken();
    while (!nord.passToken(1));
  }

  delay(5000);
}


