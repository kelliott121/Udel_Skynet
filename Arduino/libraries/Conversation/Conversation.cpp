/*
Conversation.cpp - Library to represent a conversation between two robots
Author: Keith Elliott
Date: 4/23/13
*/

#include "Conversation.h"

Conversation::Conversation(){
  sourceId = -1;
  targetId = -1;
  seq_num = -1;
}

//Returns true if the packet is not a duplicate
bool Conversation::update(Packet pkt){
  bool dirty = false;
  if (pkt.getSourceId() != sourceId || pkt.getTargetId() != targetId){
    dirty = true;
    
    sourceId = pkt.getSourceId();
    targetId = pkt.getTargetId();
    seq_num = 0;
  } 
  else if (pkt.getSequenceNumber() != seq_num){
    seq_num = pkt.getSequenceNumber();
    dirty = true;
  }

  return dirty;
}

byte Conversation::getNextSeqNum(byte target){
  if (target == targetId){
    return seq_num+1;
  }
  else{
    return 0;
  }
}
