#include <Wire.h>
#include <SPI.h>
#include <Shift.h>
#include <Packet.h>
#include <Nordic.h>
#include <Motors_Compass.h>
#include <MirfSpiDriver.h>
#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>
#include <nRF24L01.h>
#include <HMC5883L.h>
#include <Distance.h>
#include <Conversation.h>//
#include <AFMotor.h>
#include <Robot.h>
#include <Config.h>
//
Robot robot(ROBO_ID);

void setup(){
  robot.init();
}

void loop(){
  robot.waitForTurn();
  robot.takeTurn();
}
