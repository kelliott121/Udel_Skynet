/*
 * Testing the distance library
 */

#include "Distance.h"
#include <Wire.h>

Distance dist = Distance();
int iteration = 0;
unsigned long distances[10] = {0};

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
  
  //Serial.println("Sending");
  //dist.send_ir_sonar_pulse();
  
  delay(1000);
}

void receive_ir(){
  unsigned long duration, distance;
  dist.setShift(FS_BIT | LS_BIT | RS_BIT | BS_BIT, PULSE_TIME);
  duration = pulseIn(ULTRASOUND_RECEIVE_PIN, HIGH);
  
  if(iteration < 10){
    if(duration < 10000){
      Serial.print("Adding: ");
      Serial.print(duration);   
      Serial.print(", Iteration: ");
      Serial.println(iteration); 
      distances[iteration] = duration;
      iteration++;
    }else{
      Serial.print("Timeout: ");
      Serial.print(duration);         
    }
  }
  else{
    Serial.print("Average: ");
    unsigned long avg = (distances[0] + distances[1] + distances[2] + distances[3] + distances[4] + distances[5] + distances[6] + distances[7] + distances[8] + distances[9]) / 10.0;
    Serial.println(avg);
    iteration = 0;
  }
  
  

  //distance = (.13077*pow(duration, 1-.2421)*2);
  //Serial.print("Distance: ");
  //Serial.println(distance);
}
