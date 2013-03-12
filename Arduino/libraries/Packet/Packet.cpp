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

Packet::Packet(byte pkt[6]){
  sequenceNumber = pkt[0];
  targetId = pkt[1];
  sourceId = pkt[2];
  type = pkt[3];
  data[0] = pkt[4];
  data[1] = pkt[5];
}

byte * Packet::pack(){
  //byte pack[7] = {sequenceNumber, targetId, sourceId, type, data[0], data[1], '\0'};

  byte * pack = (byte *) malloc(6);
  pack[0] = sequenceNumber;
  pack[1] = targetId;
  pack[2] = sourceId;
  pack[3] = type;
  pack[4] = data[0];
  pack[5] = data[1];

  return pack;
}

void Packet::print(int format){
  Serial.print("SeqNum:\t");
  Serial.println(sequenceNumber, format);
  
  Serial.print("Target:\t");
  Serial.println(targetId, format);

  Serial.print("Source:\t");
  Serial.println(sourceId, format);

  Serial.print("Type:\t");
  Serial.println(type, format);

  Serial.print("Data[0]:\t");
  Serial.println(data[0], format);
  
  Serial.print("Data[1]:\t");
  Serial.println(data[1], format);
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
