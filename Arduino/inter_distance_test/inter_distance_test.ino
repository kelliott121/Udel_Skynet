#include <Conversation.h>

#include <SPI.h>

#include <Packet.h>

#include <MirfSpiDriver.h>
#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>
#include <nRF24L01.h>

#include <Nordic.h>

/*
 * Testing the distance library
 */

#include "Distance.h"
#include <Wire.h>

//SENDER
//byte id = 0;
//RECEIVER
byte id = 1;

Nordic nord(id);
Distance dist;

void setup() {
  Serial.begin(9600); 
  Serial.println("Beginning test");
  //dist.test_init();
  if (id){
    attachInterrupt(0, receive_ir, FALLING);
    Serial.println("Interrupt Initialized");
  }
}

void loop() {
  //nord.reinit();
  if (!id){
    Serial.println("Sending pulse");
    dist.send_ir_sonar_pulse();
    delay(100);
    //nord.reinit();
    //pinMode(11, OUTPUT);
    //digitalWrite(11, LOW);
    Packet pkt = nord.waitForInterDistance(10000);
    byte * data;
    data = pkt.getData();
    unsigned int duration = 0;
    duration = duration | (data[1]<<8);
    duration = duration | data[0];
    if (duration < 65535 && pkt.getTargetId() != NULLDATA){
      Serial.println("####################");

      Serial.print("Duration = ");
      Serial.println(duration);
      Serial.print("Distance ");
      double distance = ((double)duration)*0.1447 - 600.44;
      Serial.println(distance);
      Serial.println("####################");
    }
  }
  else{
    delay(100);
  }
}

void receive_ir(){
  //Serial.println("wtf?");
  long duration, distance;
  //Serial.println("Gets here");
  dist.setShift(FS_BIT | LS_BIT | RS_BIT | BS_BIT, PULSE_TIME);
  duration = pulseIn(ULTRASOUND_RECEIVE_PIN, HIGH);
  //distance = (.13077*pow(duration, 1-.2421)*2);
  //Serial.print("Distance: ") ;
  //Serial.println(distance);
  //Serial.print("Duration:");
  Serial.println(duration);
  //delay(10);

  //pinMode(11, OUTPUT);
  //digitalWrite(11, LOW);
  nord.sendInterDistance(!id, (unsigned int)duration);
}



