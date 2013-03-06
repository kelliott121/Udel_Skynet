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
  Mirf.setRADDR((byte*)ID);
}

bool Nordic::sendCommand(byte target, byte direction, byte time){
  byte data[2] = {direction, time};
  Packet packet(seq_num, target, ID, COMMAND, data);
  
  byte taddr[5] = {0, 0, 0, 0, target};
  Mirf.setTADDR(taddr);

  Mirf.send(packet.pack());
  while (Mirf.isSending());
}

void Nordic::sendACK(Packet packet){
  byte data[2] = {0,0};
  Packet ack(packet.getSequenceNumber(), packet.getSourceId(), ID, ACK, data);

  byte taddr[5] = {0,0,0,0,packet.getSourceId()};
  Mirf.setTADDR(taddr);

  Mirf.send(ack.pack());
  while (Mirf.isSending());
}
