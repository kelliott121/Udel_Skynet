#include <AFMotor.h>
#include <Wire.h>
#include <MirfSpiDriver.h>
#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>
#include <nRF24L01.h>
#include <SPI.h>

AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

byte[5] outgoing_msg;
byte[5] incoming_msg;
int msg_id;

void setup() {                
  Serial.begin(9600);  
  motor3.setSpeed(255);
  motor4.setSpeed(255); 
  Mirf.csnPin = 3;
  Mirf.cePin = 9;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.payload = 5;
  Mirf.channel = 0;
  Mirf.config();
  Mirf.setTADDR((byte *)"TX_01");
  Mirf.setRADDR((byte *)"TX_01"); 
  msg_id = 0;  
}

void loop() {
  if (Mirf.dataReady()){
    Mirf.getData(c);
    if (c[3] == 0x04){
      // We already dealt with this message
      if (c[0] < msg_id){
        Mirf.send((byte*)ack);
      }
      // This is the first time we've seen this message
      else if (c[0] == msg_id){
        Mirf.send((byte*)ack);        
        while( Mirf.isSending() );     
        left_turn(500);
        c[0] = c[1] = c[2] = c[3] = 0;
        msg_id++;
      }
    }     
  }
}

//FORWARD
void forward(int delay_amt){
  motor3.run(FORWARD);
  motor4.run(FORWARD);

  delay(delay_amt);
  
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  
  delay(delay_amt);  
}

//REVERSE
void reverse(int delay_amt){
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);

  delay(delay_amt);
  
  motor3.run(RELEASE);
  motor4.run(RELEASE);

  delay(delay_amt);
}

//LEFT
void left_turn(int delay_amt){
  motor3.run(BACKWARD);
  motor4.run(FORWARD);

  delay(delay_amt);

  motor3.run(RELEASE);
  motor4.run(RELEASE);
  
  delay(delay_amt);  
}

//RIGHT
void right_turn(int delay_amt){
  motor3.run(FORWARD);
  motor4.run(BACKWARD);

  delay(delay_amt);
  
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  
  delay(delay_amt);  
}
