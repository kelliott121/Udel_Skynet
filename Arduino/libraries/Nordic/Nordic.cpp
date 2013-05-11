/*
Nordic.cpp - Wrapper library to customize Mirf for our swarm purposes
Author: Keith Elliott
Date: 3/6/13
*/

#include "Nordic.h"

Nordic::Nordic(byte newID){
  ID = newID;
  //seq_num = 0;
  conversation = new Conversation();

  Mirf.csnPin = CSN;
  Mirf.cePin = CE;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();

  Mirf.payload = SIZE;
  Mirf.channel = 0;
  Mirf.config();

  //Set 1Mbps data rate to slightly increase range
  Mirf.configRegister(RF_SETUP, 0x06);

  Mirf.setTADDR((byte*)BROADCAST);
  Mirf.setRADDR((byte*)BROADCAST);

  token = false;
}

void Nordic::reinit(){
  Mirf.csnPin = CSN;
  Mirf.cePin = CE;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();

  Mirf.payload = SIZE;
  Mirf.channel = 0;
  Mirf.config();

  //Set 1Mbps data rate to slightly increase range
  Mirf.configRegister(RF_SETUP, 0x06);

  Mirf.setTADDR((byte*)BROADCAST);
  Mirf.setRADDR((byte*)BROADCAST);
}

void Nordic::giveToken(){
  token = true;
}

bool Nordic::passToken(byte target){
  bool gotACK = false;
  if (hasToken()){
    byte data[2] = {0,0};
    Packet packet(conversation->getNextSeqNum(target), target, ID, TOKEN, data);

    conversation->update(packet);
    byte taddr[5] = {0,0,0,0,target};

    for (int t=TIMEOUT; t >= 0 && !gotACK; t--){
      Mirf.send(packet.pack());
      while (Mirf.isSending());
      Serial.println("Sent token");
      packet.print(DEC);
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
	  Serial.println("Got ACK");
	  gotACK = true;
	  token = false;
	}
      }
    }
  }
  return gotACK;
}

bool Nordic::sendCommand(byte target, byte direction, byte time){
  byte data[2] = {direction, time};
  //Packet packet(seq_num, target, ID, COMMAND, data);
  Packet packet(conversation->getNextSeqNum(target), target, ID, COMMAND, data);

  conversation->update(packet);

  byte taddr[5] = {0, 0, 0, 0, target};
  //Mirf.setTADDR(taddr);

  bool gotACK = false;

  for (int t=TIMEOUT; t>=0 && !gotACK; t--){
    Mirf.send(packet.pack());
    while (Mirf.isSending());
    Serial.println("Sent packet");
    //Serial.print("Of type: ");
    //Serial.println(packet.getType());
    packet.print(DEC);
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
	Serial.println("Got ACK");
	gotACK = true;
      }
    }
  }
  
  return gotACK;
}

bool Nordic::sendFwdDist(byte target, unsigned int dist){
  byte data[2] = {(byte)(dist >> 8), (byte)(dist & 0xFF)};
  //Packet packet(seq_num, target, ID, POS_X, coordinate);
  Packet packet(conversation->getNextSeqNum(target), target, ID, DIST_FWD, data);

  conversation->update(packet);

  byte taddr[5] = {0, 0, 0, 0, target};
  //Mirf.setTADDR(taddr);

  bool gotACK = false;

  for (int t=TIMEOUT; t>=0 && !gotACK; t--){
    Mirf.send(packet.pack());
    while (Mirf.isSending());
    Serial.println("Sent packet");
    packet.print(DEC);
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
	Serial.println("Got ACK");
	gotACK = true;
      }
    }
  }
  
  return gotACK;
}

bool Nordic::sendBwdDist(byte target, unsigned int dist){
  byte data[2] = {(byte)(dist >> 8), (byte)(dist & 0xFF)};
  //Packet packet(seq_num, target, ID, POS_X, coordinate);
  Packet packet(conversation->getNextSeqNum(target), target, ID, DIST_BWD, data);

  conversation->update(packet);

  byte taddr[5] = {0, 0, 0, 0, target};
  //Mirf.setTADDR(taddr);

  bool gotACK = false;

  for (int t=TIMEOUT; t>=0 && !gotACK; t--){
    Mirf.send(packet.pack());
    while (Mirf.isSending());
    Serial.println("Sent packet");
    packet.print(DEC);
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
	Serial.println("Got ACK");
	gotACK = true;
      }
    }
  }
  
  return gotACK;
}

bool Nordic::sendLeftDist(byte target, unsigned int dist){
  byte data[2] = {(byte)(dist >> 8), (byte)(dist & 0xFF)};
  //Packet packet(seq_num, target, ID, POS_X, coordinate);
  Packet packet(conversation->getNextSeqNum(target), target, ID, DIST_LEFT, data);

  conversation->update(packet);

  byte taddr[5] = {0, 0, 0, 0, target};
  //Mirf.setTADDR(taddr);

  bool gotACK = false;

  for (int t=TIMEOUT; t>=0 && !gotACK; t--){
    Mirf.send(packet.pack());
    while (Mirf.isSending());
    Serial.println("Sent packet");
    packet.print(DEC);
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
	Serial.println("Got ACK");
	gotACK = true;
      }
    }
  }
  
  return gotACK;
}

bool Nordic::sendRightDist(byte target, unsigned int dist){
  byte data[2] = {(byte)(dist >> 8), (byte)(dist & 0xFF)};
  //Packet packet(seq_num, target, ID, POS_X, coordinate);
  Packet packet(conversation->getNextSeqNum(target), target, ID, DIST_RIGHT, data);

  conversation->update(packet);

  byte taddr[5] = {0, 0, 0, 0, target};
  //Mirf.setTADDR(taddr);

  bool gotACK = false;

  for (int t=TIMEOUT; t>=0 && !gotACK; t--){
    Mirf.send(packet.pack());
    while (Mirf.isSending());
    Serial.println("Sent packet");
    packet.print(DEC);
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
	Serial.println("Got ACK");
	gotACK = true;
      }
    }
  }
  
  return gotACK;
}

bool Nordic::sendHeading(byte target, unsigned int heading){
  byte data[2] = {(byte)(heading >> 8), (byte)(heading & 0xFF)};
  //Packet packet(seq_num, target, ID, POS_X, coordinate);
  Packet packet(conversation->getNextSeqNum(target), target, ID, HEADING, data);

  conversation->update(packet);

  byte taddr[5] = {0, 0, 0, 0, target};
  //Mirf.setTADDR(taddr);

  bool gotACK = false;

  for (int t=TIMEOUT; t>=0 && !gotACK; t--){
    Mirf.send(packet.pack());
    while (Mirf.isSending());
    Serial.println("Sent packet");
    packet.print(DEC);
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
	Serial.println("Got ACK");
	gotACK = true;
      }
    }
  }
  
  return gotACK;
}

bool Nordic::sendPosX(byte target, unsigned int coord){
  byte data[2] = {(byte)(coord >> 8), (byte)(coord & 0xFF)};
  //Packet packet(seq_num, target, ID, POS_X, coordinate);
  Packet packet(conversation->getNextSeqNum(target), target, ID, POS_X, data);

  conversation->update(packet);

  byte taddr[5] = {0, 0, 0, 0, target};
  //Mirf.setTADDR(taddr);

  bool gotACK = false;

  for (int t=TIMEOUT; t>=0 && !gotACK; t--){
    Mirf.send(packet.pack());
    while (Mirf.isSending());
    Serial.println("Sent packet");
    packet.print(DEC);
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
	Serial.println("Got ACK");
	gotACK = true;
      }
    }
  }
  
  return gotACK;
}

bool Nordic::sendPosY(byte target, unsigned int coord){
  byte data[2] = {(byte)(coord >> 8), (byte)(coord & 0xFF)};
  //Packet packet(seq_num, target, ID, POS_Y, coordinate);
  Packet packet(conversation->getNextSeqNum(target), target, ID, POS_Y, data);

  conversation->update(packet);

  byte taddr[5] = {0, 0, 0, 0, target};
  //Mirf.setTADDR(taddr);

  bool gotACK = false;

  for (int t=TIMEOUT; t>=0 && !gotACK; t--){
    Mirf.send(packet.pack());
    while (Mirf.isSending());
    Serial.println("Sent packet");
    packet.print(DEC);
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
	Serial.println("Got ACK");
	gotACK = true;
      }
    }
  }
  
  return gotACK;
}

bool Nordic::reqPos(byte target){
  byte data[2] = {NULLDATA, NULLDATA};
  //Packet packet(seq_num, target, ID, REQ_POS, coordinate);
  Packet packet(conversation->getNextSeqNum(target), target, ID, REQ_POS, data);

  conversation->update(packet);

  byte taddr[5] = {0, 0, 0, 0, target};
  //Mirf.setTADDR(taddr);

  bool gotACK = false;

  for (int t=TIMEOUT; t>=0 && !gotACK; t--){
    Mirf.send(packet.pack());
    while (Mirf.isSending());
    Serial.println("Sent packet");
    packet.print(DEC);
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
	Serial.println("Got ACK");
	gotACK = true;
      }
    }
  }
  
  return gotACK;
}

bool Nordic::sendInterDistance(byte target, unsigned int dist){
  //Serial.print("Before data: ");
  //Serial.println(dist);
  byte data[2] = {(byte)(dist >> 8), (byte)(dist & 0x00FF)};
  //Packet packet(seq_num, target, ID, COMMAND, data);
  Packet packet(conversation->getNextSeqNum(target), target, ID, INTER_DIST, data);

  conversation->update(packet);

  byte taddr[5] = {0, 0, 0, 0, target};
  //Mirf.setTADDR(taddr);

  bool gotACK = false;
  
  for (int t=TIMEOUT; t>=0 && !gotACK; t--){
    
    Mirf.send(packet.pack());
    while (Mirf.isSending());
    //Serial.println("Sent packet");
    //Serial.print("Of type: ");
    //Serial.println(packet.getType());
    //packet.print(DEC);
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
	Serial.println("Got ACK");
	gotACK = true;
      }
    }
  }

  return gotACK;
}

Packet Nordic::waitForPacket(long timeout){
  Serial.println("Waiting for packet");
  
  while (1){
    if (Mirf.dataReady()){
      Serial.println("Packet is here");
      byte ack[sizeof(Packet)];
      Mirf.getData(ack);
      Packet packet(ack);

      if (conversation->update(packet) && packet.getTargetId() == ID){
	packet.print(DEC);
	sendACK(packet);
	if (packet.getType() == TOKEN){
	  token = true;
	}
	return packet;
      }
      else{
	Serial.println("Duplicate");
      }
      
    }
    delay(100);
    timeout-=100;

    if (timeout <= 0){
      byte nullpkt[6] = {NULLDATA, NULLDATA, NULLDATA, NULLDATA, NULLDATA, NULLDATA};
      Packet pkt(nullpkt);
      Serial.println("Timeout reached");
      return pkt;
    }
  }
}

Packet Nordic::waitForCommand(long timeout){
  Serial.println("Waiting for command");
  while (1){
    if (Mirf.dataReady()){
      Serial.println("Packet is here");
      byte ack[sizeof(Packet)];
      Mirf.getData(ack);
      Packet packet(ack);

      if (conversation->update(packet) && packet.getType() == COMMAND){
	packet.print(DEC);
	sendACK(packet);
	return packet;
      }
      Serial.println("Duplicate");
    }
    
    delay(100);
    timeout-=100;
    
    if (timeout <= 0){
      byte nullpkt[6] = {NULLDATA, NULLDATA, NULLDATA, NULLDATA, NULLDATA, NULLDATA};
      Packet pkt(nullpkt);
      Serial.println("Timeout reached");
      return pkt;
    }
  }
}

Packet Nordic::waitForToken(long timeout){
  Serial.println("Waiting for token");
  while (1){
    if (Mirf.dataReady()){
      Serial.println("Packet is here");
      byte ack[sizeof(Packet)];
      Mirf.getData(ack);
      Packet packet(ack);

      if (conversation->update(packet) && packet.getType() == TOKEN && packet.getTargetId() == ID){
	packet.print(DEC);
	sendACK(packet);
	token = true;
	return packet;
      }
      Serial.println("Duplicate");
    }
    
    delay(100);
    timeout-=100;
    
    if (timeout <= 0){
      byte nullpkt[6] = {NULLDATA, NULLDATA, NULLDATA, NULLDATA, NULLDATA, NULLDATA};
      Packet pkt(nullpkt);
      Serial.println("Timeout reached");
      return pkt;
    }
  }
}

Packet Nordic::waitForInterDistance(long timeout){
  Serial.println("Waiting for inter distance request");
  while (1){
    if (Mirf.dataReady()){
      Serial.println("Packet is here");
      byte ack[sizeof(Packet)];
      Mirf.getData(ack);
      Packet packet(ack);

      if (conversation->update(packet) && packet.getTargetId() == ID && packet.getType() == INTER_DIST){
	packet.print(DEC);
	//Serial.print("Packet type: ");
	//Serial.println(packet.getType());
	//if (packet.getType() == COMMAND){
	//Serial.println("Received a Command Packet");
	sendACK(packet);
	return packet;
	//}
      }
      packet.print(DEC);
      Serial.println("Duplicate");
    }
    
    delay(100);
    timeout-=100;
    
    if (timeout <= 0){
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

bool Nordic::hasToken(){
  return token;
}
