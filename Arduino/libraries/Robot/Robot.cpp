/*
Robot.cpp - High level library for robot interaction
Author: Keith Elliott
Date: 5/11/13
*/

#include "Robot.h"

Robot::Robot(byte id){
  //Init nordic module to id
  //Init compass module
  //Init drive module and pass it the compass
  //Calibrate compass
  //Init sonar module
  //If id == 0 give self token
  //Record position as  (id, 0)
}

void Robot::waitForTurn(){
  //Wait for a packet
  
  //If token packet received
  //return

  //If location query received
  //send location

  //If command received
  //Execute command
}

void Robot::takeTurn(){
  //Request position of other robots
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
