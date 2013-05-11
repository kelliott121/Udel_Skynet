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
//#include "Drive.h"
#include "Motors_Compass.h"
#include "Distance.h"
//#include "Compass.h"

#define NUM_BOTS 4
#define LEFT 0
#define RIGHT 1
#define FRONT 2
#define BACK 3

#define DISTANCE 100

typedef struct Position{
  int x;
  int y;
} Position;

class Robot{
 public:
  Robot(byte id);

  void init();
  
  void waitForTurn();
  void takeTurn();
  void sendData();

 private:
  Nordic * nordic;
  //Drive * drive;
  Motors_Compass * motors_compass;
  Distance * distance;
  //Compass * compass;
  Position position;
  byte ID;
};

#endif
