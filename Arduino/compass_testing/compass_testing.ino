#include <AFMotor.h>
#include <Wire.h>
#include <HMC5883L.h>

#define DEBUG 0
#define THRESHOLD 5

#define TURNING_DELAY 50
#define TURNING_SPEED 255
#define MAX_TURNS 500

#define STRAIGHT_DELAY 10
#define STRAIGHT_SPEED 255
#define STRAIGHT_ITERATIONS 500

#define COMPASS_READS_DELAY 100

AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

HMC5883L compass;

int error = 0;
float Xsf = 1;
float Ysf = 1;
float Xoff = 0;
float Yoff = 0;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Wire.begin(); // Start the I2C interface.
  compass = HMC5883L(); // Construct a new HMC5883 compass.
  error = compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
  if(error != 0) // If there is an error, print it out.
    Serial.println(compass.GetErrorText(error));  
  calibrateCompass();
  delay(5000);
}

void loop() {
  straight_with_correction();
  delay(1000);
  turn(180);
  delay(1000);
  straight_with_correction();
  delay(1000);
  turn(180);
  delay(1000);
}

void turn(float num_degrees){
  float heading = getCompassHeading();
  heading += num_degrees;
  if(heading > 360)
    heading -= 360;
  turn_to_orientation(heading);
}

void turn_to_orientation(float desired_heading){

  float current_heading = getCompassHeading();
  int diff_from_desired = heading_difference(current_heading, desired_heading);
  int turns = 0;

  if(DEBUG){
    Serial.print("Attempting to turn to: ");
    Serial.println(desired_heading);
    Serial.print("Starting heading: ");
    Serial.println(current_heading); 
    Serial.print("Diff from desired: ");
    Serial.println(diff_from_desired);  
  }  
  
  while(diff_from_desired > THRESHOLD && turns < MAX_TURNS){ 
   
    turns++;
    
    if(difference_is_ccw(current_heading, desired_heading)){
      motor3.run(BACKWARD);
      motor4.run(FORWARD);      
    }
    else{
      motor3.run(FORWARD);
      motor4.run(BACKWARD);
    }
    motor3.setSpeed(TURNING_SPEED);
    motor4.setSpeed(TURNING_SPEED);
    
    delay(TURNING_DELAY);  
   
    motor3.run(RELEASE);
    motor4.run(RELEASE); 
  
    if(TURNING_DELAY < COMPASS_READS_DELAY)
      delay(COMPASS_READS_DELAY - TURNING_DELAY);

    current_heading = getCompassHeading();
    diff_from_desired = heading_difference(current_heading, desired_heading); 
  
    if(DEBUG){  
      Serial.print("Current heading: ");
      Serial.println(current_heading);  
      Serial.print("Diff from desired: ");
      Serial.println(diff_from_desired); 
    } 
  }
  
  if(DEBUG){
    Serial.print("Final heading: ");
    Serial.println(getCompassHeading());
  }  
}

int heading_difference(float current, float desired){
  int diff = ((int)(desired-current+3600))%360;
  return diff <= 180 ? diff : 360 - diff;
}

boolean difference_is_ccw(float current, float desired){
  float diff = desired - current; 
  return diff > 0 ? diff > 180 : diff >= -180;
}

void straight_with_correction(){
  
  float start_heading = getCompassHeading();
  
  for(int i=0; i<STRAIGHT_ITERATIONS; i++){
    
    motor3.run(BACKWARD);
    motor4.run(BACKWARD);
    motor3.setSpeed(STRAIGHT_SPEED);
    motor4.setSpeed(STRAIGHT_SPEED);
    delay(STRAIGHT_DELAY); 
    motor3.run(RELEASE);
    motor4.run(RELEASE); 
    
    turn_to_orientation(start_heading);
  }
}

float getCompassHeading(){
  MagnetometerRaw raw = compass.ReadRawAxis();
  float x = (raw.XAxis * Xsf) + Xoff;
  float y = (raw.YAxis * Ysf) + Yoff;
  float heading = atan2(y, x);
  if(heading < 0)
    heading += 2*PI;
  if(heading > 2*PI)
    heading -= 2*PI;
  float headingDegrees = heading * 180/M_PI; 
  return headingDegrees;
}

void calibrateCompass(){

  if(DEBUG)
    Serial.println("Beginning calibration");
 
  motor3.run(BACKWARD);
  motor4.run(FORWARD);
  
  motor3.setSpeed(225);
  motor4.setSpeed(225);

  int num_cycles = 0;
  MagnetometerRaw raw = compass.ReadRawAxis();
  float xMax = raw.XAxis, xMin = raw.XAxis;
  float yMax = raw.YAxis, yMin = raw.YAxis;
  float prev_x = raw.XAxis;
  float prev_y = raw.YAxis;
  
  while(num_cycles < 100){
    delay(COMPASS_READS_DELAY);
    raw = compass.ReadRawAxis();
    if ((abs(prev_x - raw.XAxis) < 50) && (abs(prev_y - raw.YAxis) < 50) && (raw.XAxis < 1000) && (raw.YAxis < 1000)){
      if (raw.XAxis > xMax) xMax = raw.XAxis;
      if (raw.XAxis < xMin) xMin = raw.XAxis;
      if (raw.YAxis > yMax) yMax = raw.YAxis;
      if (raw.YAxis < yMin) yMin = raw.YAxis;
      if(DEBUG){
        Serial.print("Raw x: ");
        Serial.print(raw.XAxis);
        Serial.print(", Raw y: ");
        Serial.println(raw.YAxis);
      }
    }
    else{
      if(DEBUG){
        Serial.print("Outlier! Raw x: ");
        Serial.print(raw.XAxis);
        Serial.print(", Raw y: ");
        Serial.println(raw.YAxis); 
      }  
      num_cycles = num_cycles + 1;      
    }
    prev_x = raw.XAxis;
    prev_y = raw.YAxis;   
    num_cycles = num_cycles + 1;
  }
  
  motor3.run(FORWARD);
  motor4.run(BACKWARD);  
  
  while(num_cycles < 200){
    delay(COMPASS_READS_DELAY);
    raw = compass.ReadRawAxis();
    if ((abs(prev_x - raw.XAxis) < 50) && (abs(prev_y - raw.YAxis) < 50) && (raw.XAxis < 1000) && (raw.YAxis < 1000)){
      if (raw.XAxis > xMax) xMax = raw.XAxis;
      if (raw.XAxis < xMin) xMin = raw.XAxis;
      if (raw.YAxis > yMax) yMax = raw.YAxis;
      if (raw.YAxis < yMin) yMin = raw.YAxis;
      if(DEBUG){
        Serial.print("Raw x: ");
        Serial.print(raw.XAxis);
        Serial.print(", Raw y: ");
        Serial.println(raw.YAxis);
      }
    }
    else{
      if(DEBUG){
        Serial.print("Outlier! Raw x: ");
        Serial.print(raw.XAxis);
        Serial.print(", Raw y: ");
        Serial.println(raw.YAxis);  
      } 
      num_cycles = num_cycles + 1;      
    }
    prev_x = raw.XAxis;
    prev_y = raw.YAxis;   
    num_cycles = num_cycles + 1;
  }
  
  motor3.run(RELEASE);
  motor4.run(RELEASE);    
  
  if(DEBUG){
    Serial.print("Max x: ");
    Serial.print(xMax);
    Serial.print(", Min x: ");
    Serial.print(xMin);
    Serial.print(", Max y: ");
    Serial.print(yMax);
    Serial.print(", Min y: ");
    Serial.println(yMin);
  }

  if (((yMax-yMin)/(xMax-xMin)) > 1)
    Xsf = (yMax-yMin)/(xMax-xMin);
  if (((xMax-xMin)/(yMax-yMin)) > 1)
    Ysf = (xMax-xMin)/(yMax-yMin);
  Xoff = (((xMax - xMin)/2) - xMax) * Xsf;
  Yoff = (((yMax - yMin)/2) - yMax) * Ysf;

  if(DEBUG){
    Serial.print("X scale factor: ");
    Serial.print(Xsf);
    Serial.print(", Y scale factor: ");
    Serial.println(Ysf);
    Serial.print("X offset: ");
    Serial.print(Xoff);
    Serial.print(", Y offset: ");
    Serial.println(Yoff);
    Serial.println("Calibration complete");
  }
  
}
