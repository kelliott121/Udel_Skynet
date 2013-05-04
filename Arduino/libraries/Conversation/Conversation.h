/*
Conversation.h - Library to represent a conversation between two robots
Author: Keith Elliott
Date: 4/23/13
*/

#ifndef Conversation_h
#define Conversation_h

#include "Arduino.h"
#include "Packet.h"

class Conversation{
 public:
  Conversation();
  bool update(Packet pkt);
  byte getNextSeqNum(byte target);

 private:
  byte sourceId;
  byte targetId;
  byte seq_num;
};

#endif
