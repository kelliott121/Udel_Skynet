#include <MirfSpiDriver.h>
#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>
#include <nRF24L01.h>

#include <SPI.h>

byte token = 45;
char message[] = "token";
boolean token = true;
//boolean token = false;
void setup()
{
  // init the transceiver
  Serial.begin(9600);
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();

  // we transmit only a single byte each time
  Mirf.payload = 6;
  // selecting a channel which is not too noisy
  Mirf.channel = 0;
  Mirf.config();

  // Set 1MHz data rate
  //Mirf.configRegister(RF_SETUP,0x06);

  // Set address - this one must match the 
  // address the receiver is using!
  Mirf.setTADDR((byte *)"TX_01");
  Mirf.setRADDR((byte *)"TX_01");
}

void loop() 
{
  Mirf.send((byte*)message);
  while( Mirf.isSending() );

  // ... just take your time
  delay(100);
}



