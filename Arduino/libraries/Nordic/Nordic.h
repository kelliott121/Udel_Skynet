/*
Nordic.h - Wrapper library to customize Mirf for our swarm purposes
Author: Keith Elliott
Date: 3/6/13
*/

#ifndef Nordic_h
#define Nordic_h

#include "Arduino.h"
#include "Packet.h"
#include <MirfSpiDriver.h>
#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>
#include <nRF24L01.h>
#include <SPI.h>
#include "Conversation.h"

//The Mirf CSN and CE pins are hardcoded on the shield we soldered
#define CSN 3;
#define CE 9;

//Define specific channels for the bots
//In order to take advantage of how the Nordic modules work,
//the source ID will be the receiving channel
#define BROADCAST "BCAST"
#define SERVER "SERVR"

//The number of times the device should try to send a message
#define TIMEOUT 10
#define SIZE 6

class Nordic{
 public:
  Nordic(byte newID);
  void reinit();
  
  byte autoIdentify();

  //Define the different types of send
  bool sendCommand(byte target, byte direction, byte time);
  bool passToken(byte target);
  bool sendInterDistance(byte target, unsigned int data);
  bool sendPosX(byte target, unsigned int coord);
  bool sendPosY(byte target, unsigned int coord);
  bool reqPos(byte target);

  Packet waitForPacket(long timeout);
  Packet waitForCommand(long timeout);
  Packet waitForToken(long timeout);
  Packet waitForInterDistance(long timeout);

  void sendACK(Packet packet);
  bool startSonic();
  bool requestData();
  bool hasToken();
  void giveToken();

 private:
  byte ID;
  //byte seq_num;
  Conversation * conversation;
  bool token;
};

#endif
