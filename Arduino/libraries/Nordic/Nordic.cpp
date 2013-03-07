/*
Nordic.cpp - Wrapper library to customize Mirf for our swarm purposes
Author: Keith Elliott
Date: 3/6/13
*/

#include "Nordic.h"

Nordic::Nordic(byte newID){
  ID = newID;
  seq_num = 0;

  Mirf.csnPin = CSN;
  Mirf.cePin = CE;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();

  Mirf.payload = sizeof(Packet);
  Mirf.channel = 0;
  Mirf.config();

  //Set 1Mbps data rate to slightly increase range
  Mirf.configRegister(RF_SETUP, 0x06);

  Mirf.setTADDR((byte*)BROADCAST);
  Mirf.setRADDR((byte*)BROADCAST);
}

bool Nordic::sendCommand(byte target, byte direction, byte time){
  byte data[2] = {direction, time};
  Packet packet(seq_num, target, ID, COMMAND, data);
  
  byte taddr[5] = {0, 0, 0, 0, target};
  //Mirf.setTADDR(taddr);

  bool gotACK = false;

  for (int t=TIMEOUT; t>=0 && !gotACK; t--){
    Mirf.send(packet.pack());
    while (Mirf.isSending());
    Serial.println("Sent packet");
    Serial.print("Of type: ");
    Serial.println(packet.getType());
    byte ack[sizeof(Packet)];
    for (int i = 5; i >= 0; i--){
      if (Mirf.dataReady()){
	break;
      }
      delay(10);
    }
    
    if (Mirf.dataReady()){
      Mirf.getData(ack);
      Packet ackPacket(ack);
      
      if (ackPacket.getSequenceNumber() == packet.getSequenceNumber() &&
	  ackPacket.getSourceId() == packet.getTargetId() &&
	  ackPacket.getTargetId() == packet.getSourceId() &&
	  ackPacket.getType() == ACK){
	gotACK = true;
      }
    }
  }
  
  return gotACK;
}

Packet Nordic::waitForCommand(long timeout){
  Serial.println("Waiting for command");
  while (1){
    if (Mirf.dataReady()){
      Serial.println("Packet is here");
      byte ack[sizeof(Packet)];
      Mirf.getData(ack);
      Packet packet(ack);
      Serial.print("Packet type: ");
      Serial.println(packet.getType());
      //if (packet.getType() == COMMAND){
	Serial.println("Received a Command Packet");
	sendACK(packet);
	return packet;
	//}
    }
    
    delay(100);
    timeout-=100;
    
    if (timeout == 0){
      byte nullpkt[6] = {NULLDATA, NULLDATA, NULLDATA, NULLDATA, NULLDATA, NULLDATA};
      Packet pkt(nullpkt);
      Serial.println("Timeout reached");
      return pkt;
    }
  }
}

void Nordic::sendACK(Packet packet){
  byte data[2] = {0,0};
  Packet ack(packet.getSequenceNumber(), packet.getSourceId(), ID, ACK, data);

  byte taddr[5] = {0,0,0,0,packet.getSourceId()};
  //Mirf.setTADDR(taddr);

  Mirf.send(ack.pack());
  while (Mirf.isSending());
}
