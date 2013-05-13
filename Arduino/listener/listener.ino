#include <WatchTower.h>

#include <Conversation.h>
#include <SPI.h>
#include <MirfSpiDriver.h>
#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>
#include <nRF24L01.h>
#include <Packet.h>
#include <Nordic.h>

byte id = 4;
Nordic nord(id);

WatchTower wt = WatchTower();

void setup() {
  Serial.begin(9600);
  Serial.println("Beginning Test");
}

void loop() { 
  Packet pkt;
  while(pkt.getSourceId() == NULLDATA){
    pkt = nord.waitForPacket(1000); 
  }
  switch (pkt.getType()) {
    case POS_X:
      wt.update_pos_x(pkt.getSourceId(), wt.get_packet_data(pkt));
      break;
    case POS_Y:
      wt.update_pos_y(pkt.getSourceId(), wt.get_packet_data(pkt));
      break;       
  }  
}

