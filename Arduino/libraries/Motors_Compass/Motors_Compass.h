/*
 * Header file for compass/motors library
 */

#ifndef MOTORS_COMPASS_h
#define MOTORS_COMPASS_h

#include <Arduino.h>
#include <../AFmotor/AFMotor.h>
#include <../HMC5883L/HMC5883L.h>
#include <Wire.h>

#define CALIBRATION_DELAY 100
#define CALIBRATION_CYCLES 300
#define CALIBRATION_ACCEPTABLE_CHANGE 50
#define CALIBRATION_MAX_READING 1000

#define DEFAULT_SPEED 255
#define DECLINATION_ANGLE 0
#define TWO_PI (2*PI)

class Motors_Compass
{
	public:
          Motors_Compass();
	  void calibrate_compass();
	  float get_compass_heading();

	  float Xsf;
	  float Ysf;
	  float Xoff;
	  float Yoff;

	  AF_DCMotor motor3;
	  AF_DCMotor motor4;

	  HMC5883L compass;
};
#endif
