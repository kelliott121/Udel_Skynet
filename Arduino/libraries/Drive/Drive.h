/*
Drive.h - Wrapper library to simplify the usage of the AFMotor libraries
Author: Keith Elliott
Date: 4/25/13
*/

#ifndef Drive_h
#define Drive_h

#include "Arduino.h"
#include <AFMotor.h>

class Drive{
 public:
  Drive();
  
  void forward(double time);
  void backward(double time);
  void left(double time);
  void right(double time);
  
  void setSpeed(uint8_t newSpeed);

 private:
  AF_DCMotor * leftMotor;
  AF_DCMotor * rightMotor;
  uint8_t speed;
};
#endif
