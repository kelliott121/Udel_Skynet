/*
Packet.cpp
Author: Keith Elliott
Date: 3/6/13
*/

//#include "Arduino.h"
#include "Packet.h"

Packet::Packet(){
  sequenceNumber = -1;
  targetId = -1;
  sourceId = -1;
  type = -1;
  data[0] = -1;
  data[1] = -1;
}

Packet::Packet(byte seqNum, byte tid, byte sid, byte t, byte d[2]){
  sequenceNumber = seqNum;
  targetId = tid;
  sourceId = sid;
  type = t;
  data[0] = d[0];
  data[1] = d[1];
}

byte * Packet::pack(){
  byte pack[6] = {sequenceNumber, targetId, sourceId, type, data[0], data[1]};
  return pack;
}

void Packet::setSequenceNumber(byte seqNum){
  sequenceNumber = seqNum;
}

void Packet::setTargetId(byte id){
  targetId = id;
}

void Packet::setSourceId(byte id){
  sourceId = id;
}

void Packet::setType(byte newType){
  type = newType;
}

void Packet::setData(byte newData[2]){
  data[0] = newData[0];
  data[1] = newData[1];
}

byte Packet::getSequenceNumber(){
  return sequenceNumber;
}

byte Packet::getTargetId(){
  return targetId;
}

byte Packet::getSourceId(){
  return sourceId;
}

byte Packet::getType(){
  return type;
}

byte * Packet::getData(){
  return data;
}
