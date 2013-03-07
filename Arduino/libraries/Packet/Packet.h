/*
Packet.h - Datagram library for organizing wireless module data without
guaranteed delivery
Author: Keith Elliott
Date: 3/6/13
*/

#ifndef Packet_h
#define Packet_h

//Define the Packet Types
#define NULLDATA 255
#define COMMAND 0
#define TOKEN 1
#define ACK 2
#define INIT_SONIC 3
#define REQUEST_DATA 4

#include "Arduino.h"

class Packet{
 public:
  Packet();
  Packet(byte seqNum, byte tid, byte sid, byte t, byte d[2]);
  Packet(byte pkt[6]);

  byte * pack();

  void setSequenceNumber(byte seqNum);
  void setTargetId(byte id);
  void setSourceId(byte id);
  void setType(byte newType);
  void setData(byte newData[2]);

  byte getSequenceNumber();
  byte getTargetId();
  byte getSourceId();
  byte getType();
  byte * getData();

 private:
  byte sequenceNumber;
  byte targetId;
  byte sourceId;
  byte type;
  byte data[2];
};

#endif
