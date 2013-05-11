/*
 * Testing the distance library
 */

#include "Distance.h"
#include <Wire.h>

Distance dist = Distance();
int iteration = 0;
unsigned long last_duration = 0;

void setup() {    
  Serial.begin(9600); 
  attachInterrupt(0, receive_ir, FALLING);
}


void loop() { 

  /*
  int f = dist.check_front();
  Serial.print("Front distance: ");
  Serial.println(f);

  int l = dist.check_left();
  Serial.print("Left distance: ");
  Serial.println(l);
  
  int r = dist.check_right();
  Serial.print("Right distance: ");
  Serial.println(r);
  
  int b = dist.check_back();
  Serial.print("Back distance: ");
  Serial.println(b);  */
  
  Serial.print("Last IR distance: ");
  Serial.println(last_duration);
  //dist.send_ir_sonar_pulse();
  
  delay(1000);
}

void receive_ir(){
  noInterrupts();
  unsigned long duration, distance;
  dist.setShift(FS_BIT | LS_BIT | RS_BIT | BS_BIT, PULSE_TIME);
  duration = pulseIn(ULTRASOUND_RECEIVE_PIN, HIGH);
  

  //distance = (.13077*pow(duration, 1-.2421)*2);
  //Serial.print("Distance: ");
  //Serial.println(distance);
  interrupts();
}
