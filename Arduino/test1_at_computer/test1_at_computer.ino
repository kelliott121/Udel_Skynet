#include <Wire.h>

String str_from_comp;
char char_from_comp;

void setup() {       
  str_from_comp = "";
  char_from_comp = 0;  
  Serial.begin(9600);  
  pinMode(13, OUTPUT);  
}

void loop() {

  while(Serial.available()) {
    char_from_comp = Serial.read();
    str_from_comp.concat(char_from_comp);
    if(char_from_comp == 0x04){
      //send comm over wireless module
      debug_blink();
      str_from_comp = "";
      char_from_comp = 0;        
    }
  }
  
}

//BLINK
void debug_blink(){
  digitalWrite(13, HIGH); 
  delay(100);           
  digitalWrite(13, LOW);   
  delay(100);                
}
