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
  
  byte autoIdentify();

  //Define the different types of send
  bool sendCommand(byte target, byte direction, byte time);
  Packet waitForCommand(long timeout);

  bool passToken();
  void sendACK(Packet packet);
  bool startSonic();
  bool requestData();

 private:
  byte ID;
  byte seq_num;
};

#endif
