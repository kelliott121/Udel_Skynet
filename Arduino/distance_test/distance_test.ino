/*
 * Testing the distance library
 */

#include "Distance.h"
#include <Wire.h>

Distance dist = Distance();

void setup() {    
  Serial.begin(9600);     
}

void loop() { 
  delay(1000); 
  int f = dist.check_front();
  int l = dist.check_left();
  int r = dist.check_right();
  int b = dist.check_back();
  Serial.print("Front distance: ");
  Serial.println(f);
  Serial.print("Left distance: ");
  Serial.println(l);
  Serial.print("Right distance: ");
  Serial.println(r);
  Serial.print("Back distance: ");
  Serial.println(b);  
}
