/*
 * Testing the shift register
 */

#include "Shift.h"
#include <Wire.h>

Shift shift_register = Shift();

void setup() {    
  Serial.begin(9600);     
}

void loop() {
  shift_register.enableNone();
  Serial.println(shift_register.currentEnabled());  
  delay(100);
  shift_register.enableA();
  Serial.println(shift_register.currentEnabled());
  delay(100);  
  shift_register.enableB();
  Serial.println(shift_register.currentEnabled());
  delay(100);  
  shift_register.enableC();
  Serial.println(shift_register.currentEnabled());
  delay(100);  
  shift_register.enableD();
  Serial.println(shift_register.currentEnabled());
  delay(100);  
  shift_register.enableE();
  Serial.println(shift_register.currentEnabled());
  delay(100);  
  shift_register.enableF();
  Serial.println(shift_register.currentEnabled());
  delay(100);  
  shift_register.enableG();
  Serial.println(shift_register.currentEnabled());
  delay(100);  
  shift_register.enableH();
  Serial.println(shift_register.currentEnabled()); 
  delay(100);  
  shift_register.enableAll();
  Serial.println(shift_register.currentEnabled());  
  delay(100);  
}
