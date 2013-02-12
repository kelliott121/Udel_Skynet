#include <AFMotor.h>
#include <Wire.h>
#include <HMC5883L.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
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
  Serial.println("Beginning test!");
  
  Serial.println("Starting the I2C interface.");
  Wire.begin(); // Start the I2C interface.

  Serial.println("Constructing new HMC5883L");
  compass = HMC5883L(); // Construct a new HMC5883 compass.
  
  Serial.println("Setting measurement mode to continous.");
  error = compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
  if(error != 0) // If there is an error, print it out.
    Serial.println(compass.GetErrorText(error));  

  Serial.println("hello?");
  calibrateCompass();
}

void loop() {
  uint8_t i;

  Serial.print("tick");

  //forwardOne();
  //turnNoCompass();

  forwardOne();
  turnRight90();
  forwardOne();
  turnRight90();
  forwardOne();
  turnRight90();
  forwardOne();
  turnRight90();
}

void forwardOne(){
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);

  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor3.setSpeed(255);
  motor4.setSpeed(255);

  delay(5000);
  
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);

  delay(1000);
}

void turnNoCompass(){
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);

  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor3.setSpeed(255);
  motor4.setSpeed(255);  

  delay(5000); 

  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);  

  delay(1000); 
}

void turnRight90(){
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);

  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor3.setSpeed(255);
  motor4.setSpeed(255);

  float difference = 0;
  float initial_heading = getCompassHeading();
  float last_heading = initial_heading, current_heading = initial_heading;
  while(difference < 90){
    current_heading = getCompassHeading();
    
    if (abs(current_heading - last_heading) > 50)
      current_heading = last_heading;
      
    if ((initial_heading > 270) && (current_heading < 90)) 
      current_heading = current_heading + 360; 
      
    difference = current_heading - initial_heading; 

    Serial.print("Initial: ");
    Serial.print(initial_heading);  
    Serial.print(", Current: ");
    Serial.print(current_heading);  
    Serial.print(", Difference: ");
    Serial.println(difference);     

    last_heading = current_heading;
    delay(200);
  }  
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);  

  delay(1000);  
}

float getCompassHeading(){
  MagnetometerRaw raw = compass.ReadRawAxis();
  float x = (raw.XAxis * Xsf) + Xoff;
  float y = (raw.YAxis * Ysf) + Yoff;
  float heading = atan2(y, x);
  //float declinationAngle = 0.0457;
  //heading += declinationAngle;
  if(heading < 0)
    heading += 2*PI;
  if(heading > 2*PI)
    heading -= 2*PI;
  float headingDegrees = heading * 180/M_PI; 
  return headingDegrees;
}

void calibrateCompass(){

  Serial.println("Beginning calibration");

  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);

  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor3.setSpeed(255);
  motor4.setSpeed(255);

  int num_cycles = 0;
  MagnetometerRaw raw = compass.ReadRawAxis();
  float xMax = raw.XAxis, xMin = raw.XAxis;
  float yMax = raw.YAxis, yMin = raw.YAxis;
  float prev_x = raw.XAxis;
  float prev_y = raw.YAxis;
  while(num_cycles < 300){
    delay(100);
    raw = compass.ReadRawAxis();
    if ((abs(prev_x - raw.XAxis) < 50) && (abs(prev_y - raw.YAxis) < 50) && (raw.XAxis < 1000) && (raw.YAxis < 1000)){
      if (raw.XAxis > xMax) xMax = raw.XAxis;
      if (raw.XAxis < xMin) xMin = raw.XAxis;
      if (raw.YAxis > yMax) yMax = raw.YAxis;
      if (raw.YAxis < yMin) yMin = raw.YAxis;
      Serial.print("Raw x: ");
      Serial.print(raw.XAxis);
      Serial.print(", Raw y: ");
      Serial.println(raw.YAxis);
    }
    else{
      Serial.print("Outlier! Raw x: ");
      Serial.print(raw.XAxis);
      Serial.print(", Raw y: ");
      Serial.println(raw.YAxis);   
      num_cycles = num_cycles + 1;      
    }
    prev_x = raw.XAxis;
    prev_y = raw.YAxis;   
    num_cycles = num_cycles + 1;
  }
  
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);    
  
  Serial.print("Max x: ");
  Serial.print(xMax);
  Serial.print(", Min x: ");
  Serial.print(xMin);
  Serial.print(", Max y: ");
  Serial.print(yMax);
  Serial.print(", Min y: ");
  Serial.println(yMin);

  if (((yMax-yMin)/(xMax-xMin)) > 1)
    Xsf = (yMax-yMin)/(xMax-xMin);
  if (((xMax-xMin)/(yMax-yMin)) > 1)
    Ysf = (xMax-xMin)/(yMax-yMin);
  Xoff = (((xMax - xMin)/2) - xMax) * Xsf;
  Yoff = (((yMax - yMin)/2) - yMax) * Ysf;

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
