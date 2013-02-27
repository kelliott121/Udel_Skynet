#include <AFMotor.h>

#include <MirfSpiDriver.h>
#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>
#include <nRF24L01.h>

#include <SPI.h>

char message[] = "token";
char ack[] = "gotit";
//boolean token = true;
boolean token = false;
byte c[6];

AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

void setup()
{
  // init the transceiver
  Serial.begin(9600);
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  
  Mirf.payload = 6;
  Mirf.channel = 0;
  Mirf.config();

  Mirf.setTADDR((byte *)"TX_01");
  Mirf.setRADDR((byte *)"TX_01");
}

void loop() 
{
  if (token){
    Serial.println("+");
    Mirf.send((byte*)message);
    while( Mirf.isSending() );
    Serial.println("Sent Token");
  }
  else{
    Serial.println("o");
  }

  if (Mirf.dataReady()){
    Mirf.getData(c);

    if (!strcmp((char*)c, "token")){
      Serial.println("Got Token");
      token = true;
      Mirf.send((byte*)ack);
      while( Mirf.isSending() );
      Serial.println("Sent ACK");
      motor3.setSpeed(255);
      motor4.setSpeed(255);
      motor3.run(FORWARD);
      motor4.run(FORWARD);
    }
    else if (!strcmp((char*)c, "gotit")){
      token = false;
      Serial.println("Got ACK");
      motor3.run(RELEASE);
      motor4.run(RELEASE);
    }
  }

  // ... just take your time
  delay(5000);
}






