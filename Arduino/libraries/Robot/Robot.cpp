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
  motors_compass->calibrate_compass();
}

void Robot::waitForTurn(){
  while (!nordic->hasToken()){
    //Wait for a packet
    Packet pkt;
    while(pkt.getSourceId() == NULLDATA){
      pkt = nordic->waitForPacket(1000);
    }
    //If token packet received
    if (pkt.getType() == TOKEN){
      //return;
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
  Packet pkt;
  for (i = ((ID+1)%NUM_BOTS); i != ID; i = ((i+1)%NUM_BOTS)){
    while (pkt.getSourceId() != NULLDATA){
      nordic->reqPos((byte)i);
      pkt = nordic->waitForPacket(5000);
    }
  }

  int distances[4];
  //Get distances in all directions not occupied by other robots
  distances[LEFT] = distance->check_left();
  distances[RIGHT] = distance->check_right();
  distances[FRONT] = distance->check_front();
  distances[BACK] = distance->check_back();

  //Pick direction with clockwise precedence
  //Turn towards direction
  //Travel straight in direction briefly
  float heading = motors_compass->get_compass_heading();
  if (distances[FRONT] > 20){
    //GO FORWARD
    motors_compass->go_straight(heading, DISTANCE);
  }
  else if (distances[LEFT] > 20){
    //GO LEFT
    motors_compass->go_straight(heading-90, DISTANCE);
  }
  else if (distances[BACK] > 20){
    //GO BACK
    motors_compass->go_straight(heading+180, DISTANCE);
  }
  else if (distances[RIGHT] > 20){
    //GO RIGHT
    motors_compass->go_straight(heading+90, DISTANCE);
  }

  //Send IR Sonar pulse
  //Request distances from other robots
  //Triangulate current position
  
  //Take sonar readings
  distances[LEFT] = distance->check_left();
  distances[RIGHT] = distance->check_right();
  distances[FRONT] = distance->check_front();
  distances[BACK] = distance->check_back();
  
  //Send data to watchtower
  nordic->sendPosX(WATCHTOWER, position.x);
  nordic->sendPosY(WATCHTOWER, position.y);
  nordic->sendHeading(WATCHTOWER, (int)motors_compass->get_compass_heading());
  nordic->sendFwdDist(WATCHTOWER, distances[FRONT]);
  nordic->sendBwdDist(WATCHTOWER, distances[BACK]);
  nordic->sendLeftDist(WATCHTOWER, distances[LEFT]);
  nordic->sendRightDist(WATCHTOWER, distances[RIGHT]);

  //Pass token to next in line
  while (!nordic->passToken((ID+1)%NUM_BOTS));
}

void Robot::sendData(){
  //Send current location
  //Send orientation
  //Send data from (left, right, forward, backward)
}
