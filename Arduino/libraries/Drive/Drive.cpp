/*
Drive.h - Wrapper library to simplify the usage of the AFMotor libraries
Author: Keith Elliott
Date: 4/25/13
*/

#include "Drive.h"

Drive::Drive(){
  leftMotor = new AF_DCMotor(3);
  rightMotor = new AF_DCMotor(4);
  speed = 255;
  leftMotor->setSpeed(0);
  rightMotor->setSpeed(0);
}

void Drive::forward(double time){
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
  leftMotor->setSpeed(speed);
  rightMotor->setSpeed(speed);

  delay((long)time * 1000);

  leftMotor->setSpeed(0);
  rightMotor->setSpeed(0);
}

void Drive::backward(double time){
  leftMotor->run(BACKWARD);
  rightMotor->run(BACKWARD);
  leftMotor->setSpeed(speed);
  rightMotor->setSpeed(speed);

  delay((long)time * 1000);

  leftMotor->setSpeed(0);
  rightMotor->setSpeed(0);
}

void Drive::left(double time){
  leftMotor->run(BACKWARD);
  rightMotor->run(FORWARD);
  leftMotor->setSpeed(speed);
  rightMotor->setSpeed(speed);

  delay((long)time * 1000);

  leftMotor->setSpeed(0);
  rightMotor->setSpeed(0);
}

void Drive::right(double time){
  leftMotor->run(FORWARD);
  rightMotor->run(BACKWARD);
  leftMotor->setSpeed(speed);
  rightMotor->setSpeed(speed);

  delay((long)time * 1000);

  leftMotor->setSpeed(0);
  rightMotor->setSpeed(0);
}

void Drive::setSpeed(uint8_t newSpeed){
  speed = newSpeed;
}
