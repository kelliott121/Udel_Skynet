#include <Motors_Compass.h>
#include <Wire.h>

Motors_Compass robot = Motors_Compass();

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Initializing");
  robot.calibrate_compass();
  Serial.print("X scale factor: ");
  Serial.print(robot.Xsf);
  Serial.print(", Y scale factor: ");
  Serial.println(robot.Ysf);
  Serial.print("X offset: ");
  Serial.print(robot.Xoff);
  Serial.print(", Y offset: ");
  Serial.println(robot.Yoff);
  
}

void loop() {

  delay(5000);
  
  float heading = robot.get_compass_heading();
  Serial.print("Heading = ");
  Serial.println(heading);
}

