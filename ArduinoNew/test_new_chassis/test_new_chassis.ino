#include <AFMotor.h>
#include <Wire.h>

//AF_DCMotor motor1(1);
//AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Beginning test!");
}

void loop() {
  uint8_t i;

  Serial.println("tick");

  forwardOne();
  turnNoCompass();
}

void forwardOne(){
  //motor1.run(FORWARD);
  //motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);

  //motor1.setSpeed(255);
  //motor2.setSpeed(255);
  motor3.setSpeed(255);
  motor4.setSpeed(255);

  delay(5000);
  
  //motor1.run(RELEASE);
  //motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);

  delay(1000);
}

void turnNoCompass(){
  //motor1.run(BACKWARD);
  //motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);

  //motor1.setSpeed(255);
  //motor2.setSpeed(255);
  motor3.setSpeed(255);
  motor4.setSpeed(255);  

  delay(5000); 

  //motor1.run(RELEASE);
  //motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);  

  delay(1000); 
}
