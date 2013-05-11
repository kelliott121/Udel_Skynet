/*
 * Header file for compass/motors library
 */

#ifndef MOTORS_COMPASS_h
#define MOTORS_COMPASS_h

#include <Arduino.h>
#include <../AFmotor/AFMotor.h>
#include <../HMC5883L/HMC5883L.h>
#include <Wire.h>

#define THRESHOLD 1

#define TURNING_DELAY 25
#define TURNING_SPEED 150
#define TURNING_CALIBRATION_SPEED 125
#define MAX_TURNS 100

#define STRAIGHT_DELAY 75
#define STRAIGHT_SPEED 150
#define STRAIGHT_ITERATIONS 1000

#define COMPASS_READS_DELAY 100

#define TWO_PI (2*PI)

class Motors_Compass
{
 public:
  void initialize();
  void turn_to_orientation(float);
  int heading_difference(float, float);
  boolean difference_is_ccw(float, float);
  void go_straight(float, float);
  float get_compass_heading();
  void calibrate_compass();
  
  float Xsf;
  float Ysf;
  float Xoff;
  float Yoff;

  AF_DCMotor *motor3;
  AF_DCMotor *motor4;

  HMC5883L compass;

};
#endif
