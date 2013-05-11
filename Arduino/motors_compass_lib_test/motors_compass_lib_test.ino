#include <AFMotor.h>
#include <HMC5883L.h>
#include <Motors_Compass.h>
#include <Wire.h>

#define DIR 180

Motors_Compass robot;

void setup() {
  Serial.begin(9600);       
  robot.initialize();
  robot.calibrate_compass();
  delay(15000);
    robot.go_straight(DIR, 20);
  delay(30000);
    robot.go_straight(DIR, 5);
  delay(15000);
    robot.go_straight(DIR, 10);
  delay(15000);
    robot.go_straight(DIR, 15);
  delay(15000);
  robot.go_straight(DIR, 20);
  delay(15000);
    robot.go_straight(DIR, 25);
  delay(15000);
    robot.go_straight(DIR, 30);
  delay(15000);
    robot.go_straight(DIR, 35);
  delay(15000);
    robot.go_straight(DIR, 40);
  delay(15000);
    robot.go_straight(DIR, 45);
  delay(15000);
    robot.go_straight(DIR, 50);
  delay(15000);
    robot.go_straight(DIR, 60);
  delay(15000);
    robot.go_straight(DIR, 70);
  delay(15000);
    robot.go_straight(DIR, 80);
  delay(15000);
}

void loop() {
  /*robot.go_straight(0);
  delay(15000);
  robot.go_straight(0);
  delay(1000); */ 
}

