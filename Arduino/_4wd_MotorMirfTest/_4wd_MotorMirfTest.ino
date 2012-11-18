#include <AFMotor.h>
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Initializing Motors");

  //----------AFMOTOR SETUP----------
  // turn on motor
  motor1.setSpeed(200);
  motor2.setSpeed(200);
  motor3.setSpeed(200);
  motor4.setSpeed(200);

  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);

  //----------MIRF SETUP----------
  // * Pins:
  // * Hardware SPI:
  // * MISO -> 12
  // * MOSI -> 11
  // * SCK -> 13
  // *
  // * Configurable:
  // * CE -> 7
  // * CSN -> 8
  Serial.println("Initializing wireless");
  Mirf.cePin = 7;
  Mirf.csnPin = 8;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  
  //Configure receiving address
  Mirf.setRADDR((byte *)"bot01");
  //Set payload size (must be same on client and server)
  Mirf.payload = sizeof(byte);
  
  Mirf.config();
  
  Serial.println("Setup Complete!");
}

void loop() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);

  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor3.setSpeed(255);
  motor4.setSpeed(255);

  delay(5000);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);

  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor3.setSpeed(255);
  motor4.setSpeed(255);

  delay(5000);
  
  motor1.setSpeed(0);
  motor2.setSpeed(0);
  motor3.setSpeed(0);
  motor4.setSpeed(0);
  
  byte data = 1;
  
  //Pass the baton
  Serial.println("Sending data");
  Mirf.setTADDR((byte *) "bot02");
  Mirf.send((byte *) &data);
  //Wait until baton is sent
  while (Mirf.isSending());
  
  //Wait until baton is received
  while (!Mirf.dataReady());
  
  Mirf.getData((byte *)&data);
  Serial.print("Received: ");
  Serial.println(data);
}


