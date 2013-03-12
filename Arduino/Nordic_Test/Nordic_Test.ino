#include <SPI.h>

#include <MirfSpiDriver.h>
#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>
#include <nRF24L01.h>

#include <Packet.h>

#include <Nordic.h>

//byte id = 0;
byte id = 1;

Nordic nord(id);

void setup() {
  Serial.begin(9600);
  Serial.println("Beginning Test");
  Serial.println(sizeof(Packet));
}

void loop() {
  if (id){
    Packet pkt;
    while (pkt.getSourceId() == NULLDATA){
      pkt = nord.waitForCommand(1000);
      //Serial.println(sizeof(pkt.pack()));
      //Serial.println(pkt.getTargetId());
    }
    //Serial.print("Received packet from: ");
    //Serial.println(pkt.getSourceId(), DEC);

    //Serial.print("Message: ");
    byte * data = pkt.getData();

    //Serial.print(data[0], DEC);
    //Serial.print(" ");
    //Serial.println(data[1], DEC);
  }
  else{
    while (!nord.sendCommand(1, 'f', 200));
    //Serial.println("Packet sending error");
  }

  delay(5000);
}


