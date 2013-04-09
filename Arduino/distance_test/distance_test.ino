/*
 * Testing the distance library
 */

#include "Distance.h"
#include <Wire.h>

Distance dist = Distance();

void setup() {    
  Serial.begin(9600); 
  //dist.test_init();
}

void loop() { 
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
  Serial.println(b);  
}
