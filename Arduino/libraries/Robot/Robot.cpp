/*
Robot.cpp - High level library for robot interaction
Author: Keith Elliott
Date: 5/11/13
*/

#include "Robot.h"

Robot::Robot(byte id){
  ID = id;
  //Init nordic module to id
  nordic = new Nordic(id);
  //Init compass module
  motors_compass = new Motors_Compass();
  //Init sonar module
  distance = new Distance();

  if (id == 0){
    nordic->giveToken();
  }

  position.x = id * 25;
  position.y = 0;
}

void Robot::init(){
  //Calibrate compass
  motors_compass->initialize();
}

void Robot::waitForTurn(){
  while (1){
    //Wait for a packet
    Packet pkt;
    while(pkt.getSourceId() == NULLDATA){
      pkt = nordic->waitForPacket(1000);
    }
    //If token packet received
    if (pkt.getType() == TOKEN){
      return;
    }
    //If location query received
    else if (pkt.getType() == REQ_POS){
      //send location
      nordic->sendPosX(pkt.getSourceId(), position.x);
      nordic->sendPosY(pkt.getSourceId(), position.y);
    }
    //If command received
    else if (pkt.getType() == COMMAND){
      //Execute command
    }
  }
}

void Robot::takeTurn(){
  //Request position of other robots
  int i;
  for (i = (ID + 1) % NUM_BOTS; i != ID; i = (i+1)%NUM_BOTS){

  }
  //Get distances in all directions not occupied by other robots
  //Pick direction with clockwise precedence

  //Turn towards direction
  //Travel straight in direction briefly

  //Send IR Sonar pulse
  //Request distances from other robots
  //Triangulate current position
  
  //Take sonar readings
  //Send data to watchtower

  //Pass token to next in line
}

void Robot::sendData(){
  //Send current location
  //Send orientation
  //Send data from (left, right, forward, backward)
}
