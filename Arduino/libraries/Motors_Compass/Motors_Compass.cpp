/*
 * Compass/motors library
 */

#include <Arduino.h> 
#include "Motors_Compass.h"

///////////////////////
// Public functions
///////////////////////
void Motors_Compass::initialize()
{
  Xsf = 1;
  Ysf = 1;
  Xoff = 0;
  Yoff = 0;
  Wire.begin();
  motor3 = new AF_DCMotor(3);
  motor4 = new AF_DCMotor(4);
  compass = HMC5883L();
  compass.SetMeasurementMode(Measurement_Continuous);
}

void Motors_Compass::turn_to_orientation(float desired_heading){

  delay(COMPASS_READS_DELAY);

  float current_heading = get_compass_heading();
  int diff_from_desired = heading_difference(current_heading, desired_heading);
  int turns = 0;

  while((diff_from_desired > THRESHOLD) && (turns < MAX_TURNS)){ 
   
    turns++;
    
    if(difference_is_ccw(current_heading, desired_heading)){
      motor3->run(FORWARD);
      motor4->run(BACKWARD);      
    }
    else{
      motor3->run(BACKWARD);
      motor4->run(FORWARD);
    }
    motor3->setSpeed(TURNING_SPEED);
    motor4->setSpeed(TURNING_SPEED);
    
    delay(TURNING_DELAY);  
    
    motor3->run(RELEASE);
    motor4->run(RELEASE); 
    
    if(TURNING_DELAY < COMPASS_READS_DELAY)
      delay(COMPASS_READS_DELAY - TURNING_DELAY);
    
    current_heading = get_compass_heading();
    diff_from_desired = heading_difference(current_heading, desired_heading); 
  
  }
}

int Motors_Compass::heading_difference(float current, float desired){
  int diff = ((int)(desired-current+360))%360;
  return diff <= 180 ? diff : 360 - diff;
}

boolean Motors_Compass::difference_is_ccw(float current, float desired){
  float diff = desired - current; 
  return diff > 0 ? diff > 180 : diff >= -180;
}

void Motors_Compass::go_straight(float desired_heading, float distance){
  
  int iterations = (int)(distance);//*1.365);

  for(int i=0; i<iterations; i++){
    
    motor3->run(BACKWARD);
    motor4->run(BACKWARD);
    motor3->setSpeed(STRAIGHT_SPEED);
    motor4->setSpeed(STRAIGHT_SPEED);
    delay(STRAIGHT_DELAY); 
    motor3->run(RELEASE);
    motor4->run(RELEASE); 
    
    turn_to_orientation(desired_heading);
  }
}

float Motors_Compass::get_compass_heading(){
  MagnetometerRaw raw = compass.ReadRawAxis();
  float x = (raw.XAxis * Xsf) + Xoff;
  float y = (raw.YAxis * Ysf) + Yoff;
  float heading = atan2(y, x);
  if(heading < 0)
    heading += TWO_PI;
  if(heading > TWO_PI)
    heading -= TWO_PI;
  float headingDegrees = heading * 180/M_PI; 
  return headingDegrees;
}

void Motors_Compass::calibrate_compass(){

  motor3->run(BACKWARD);
  motor4->run(FORWARD);
  
  motor3->setSpeed(TURNING_CALIBRATION_SPEED);
  motor4->setSpeed(TURNING_CALIBRATION_SPEED);

  int num_cycles = 0;
  MagnetometerRaw raw = compass.ReadRawAxis();
  float xMax = raw.XAxis, xMin = raw.XAxis;
  float yMax = raw.YAxis, yMin = raw.YAxis;
  float prev_x = raw.XAxis;
  float prev_y = raw.YAxis;
  
  while(num_cycles < 100){
    delay(COMPASS_READS_DELAY);
    raw = compass.ReadRawAxis();
    if ((abs(prev_x - raw.XAxis) < 50) 
	&& (abs(prev_y - raw.YAxis) < 50) 
	&& (raw.XAxis < 1000) 
	&& (raw.YAxis < 1000)){
      if (raw.XAxis > xMax) xMax = raw.XAxis;
      if (raw.XAxis < xMin) xMin = raw.XAxis;
      if (raw.YAxis > yMax) yMax = raw.YAxis;
      if (raw.YAxis < yMin) yMin = raw.YAxis;
    }
    prev_x = raw.XAxis;
    prev_y = raw.YAxis;   
    num_cycles = num_cycles + 1;
  }
  
  motor3->run(FORWARD);
  motor4->run(BACKWARD);  
  
  while(num_cycles < 200){
    delay(COMPASS_READS_DELAY);
    raw = compass.ReadRawAxis();
    if ((abs(prev_x - raw.XAxis) < 50) 
	&& (abs(prev_y - raw.YAxis) < 50) 
	&& (raw.XAxis < 1000) 
	&& (raw.YAxis < 1000)){
      if (raw.XAxis > xMax) xMax = raw.XAxis;
      if (raw.XAxis < xMin) xMin = raw.XAxis;
      if (raw.YAxis > yMax) yMax = raw.YAxis;
      if (raw.YAxis < yMin) yMin = raw.YAxis;
    }
    prev_x = raw.XAxis;
    prev_y = raw.YAxis;   
    num_cycles = num_cycles + 1;
  }
  
  motor3->run(RELEASE);
  motor4->run(RELEASE);    
  
  if (((yMax-yMin)/(xMax-xMin)) > 1)
    Xsf = (yMax-yMin)/(xMax-xMin);
  if (((xMax-xMin)/(yMax-yMin)) > 1)
    Ysf = (xMax-xMin)/(yMax-yMin);
  Xoff = (((xMax - xMin)/2) - xMax) * Xsf;
  Yoff = (((yMax - yMin)/2) - yMax) * Ysf;

}
