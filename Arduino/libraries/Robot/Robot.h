/*
Robot.h - High level library for robot interaction
Author: Keith Elliott
Date: 5/11/13
*/

#ifndef Robot_h
#define Robot_h

#include "Arduino.h"
#include "Nordic.h"
#include "Packet.h"
#include "Drive.h"
#include "Distance.h"
//#include "Compass.h"

class Robot{
 public:
  Robot(byte id);
  
  void waitForTurn();
  void takeTurn();
  void sendData();

 private:
  Nordic * nordic;
  Drive * drive;
  Distance * distance;
  //Compass * compass;
  
};

#endif
