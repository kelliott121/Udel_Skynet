/*
 * Compass/motors library
 */

#include <Arduino.h> 
#include "Motors_Compass.h"

Motors_Compass::Motors_Compass()
{
  Xsf = 1;
  Ysf = 1;
  Xoff = 0;
  Yoff = 0;
  Wire.begin();
  motor3 = AF_DCMotor(3);
  motor4 = AF_DCMotor(4);
  compass = HMC5883L();
  compass.SetMeasurementMode(Measurement_Continuous);
}

///////////////////////
// Public functions
///////////////////////
void Motors_Compass::calibrate_compass()
{
  motor3.run(FORWARD);
  motor4.run(BACKWARD);
  motor3.setSpeed(DEFAULT_SPEED);
  motor4.setSpeed(DEFAULT_SPEED);

  int num_cycles = 0;

  //Initial reading
  MagnetometerRaw raw = compass.ReadRawAxis();
  float xMax = raw.XAxis, xMin = raw.XAxis;
  float yMax = raw.YAxis, yMin = raw.YAxis;
  float prev_x = raw.XAxis;
  float prev_y = raw.YAxis;
  
  while(num_cycles < CALIBRATION_CYCLES){
    delay(CALIBRATION_DELAY);
    raw = compass.ReadRawAxis();
    if ((abs(prev_x - raw.XAxis) < CALIBRATION_ACCEPTABLE_CHANGE) && 
	(abs(prev_y - raw.YAxis) < CALIBRATION_ACCEPTABLE_CHANGE) && 
	(raw.XAxis < CALIBRATION_MAX_READING) && (raw.YAxis < CALIBRATION_MAX_READING)){
      if (raw.XAxis > xMax) xMax = raw.XAxis;
      if (raw.XAxis < xMin) xMin = raw.XAxis;
      if (raw.YAxis > yMax) yMax = raw.YAxis;
      if (raw.YAxis < yMin) yMin = raw.YAxis;
    }
    prev_x = raw.XAxis;
    prev_y = raw.YAxis;   
    num_cycles = num_cycles + 1;
  }
  
  motor3.run(RELEASE);
  motor4.run(RELEASE);    
  
  if (((yMax-yMin)/(xMax-xMin)) > 1)
    Xsf = (yMax-yMin)/(xMax-xMin);
  if (((xMax-xMin)/(yMax-yMin)) > 1)
    Ysf = (xMax-xMin)/(yMax-yMin);
  Xoff = (((xMax - xMin)/2) - xMax) * Xsf;
  Yoff = (((yMax - yMin)/2) - yMax) * Ysf;
}

float Motors_Compass::get_compass_heading()
{
  MagnetometerRaw raw = compass.ReadRawAxis();
  float x = (raw.XAxis * Xsf) + Xoff;
  float y = (raw.YAxis * Ysf) + Yoff;
  float heading = atan2(y, x);
  heading += DECLINATION_ANGLE;
  if(heading < 0)
    heading += TWO_PI;
  if(heading > TWO_PI)
    heading -= TWO_PI;
  float headingDegrees = heading * (180/M_PI); 
  return headingDegrees;
}
