/*
 * Testing the distance library
 */

#include "Distance.h"
#include <Wire.h>

Distance dist = Distance();

void setup() {    
  Serial.begin(9600); 
  //dist.test_init();
  attachInterrupt(0, receive_ir, FALLING);
}

void loop() { 
  int f = dist.check_front();
  Serial.print("Front distance: ");
  Serial.println(f);
  delay(1000);

  int l = dist.check_left();
  Serial.print("Left distance: ");
  Serial.println(l);
  delay(1000);
  
  int r = dist.check_right();
  Serial.print("Right distance: ");
  Serial.println(r);
  delay(1000);
  
  int b = dist.check_back();
  Serial.print("Back distance: ");
  Serial.println(b);  
  delay(1000);
}

void receive_ir(){

  long duration, distance;
  dist.setShift(FS_BIT | LS_BIT | RS_BIT | BS_BIT, PULSE_TIME);
  duration = pulseIn(ULTRASOUND_RECEIVE_PIN, HIGH);
  distance = (.13077*pow(duration, 1-.2421)*2);
  Serial.print("Distance: ");
  Serial.println(distance);
}
