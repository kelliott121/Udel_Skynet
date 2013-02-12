#include <SPI.h>

#include <MirfSpiDriver.h>
#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>
#include <nRF24L01.h>



void setup()
{
  Serial.begin(9600);
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  
  // name the receiving channel - must match tranmitter setting!
  Mirf.setRADDR((byte *)"TX_01");
  
  // just a single byte is transmitted
  Mirf.payload = 6;

  // we use channel 90 as it is outside of WLAN bands 
  // or channels used by wireless surveillance cameras 
  Mirf.channel = 0;
  
  // now config the device.... 
  Mirf.config();  
  
  // Set 1MHz data rate - this increases the range slightly
  //Mirf.configRegister(RF_SETUP,0x06);
}

void loop()
{

  byte c[6]; 

  // is there any data pending? 
  if( Mirf.dataReady() )
  {
     // well, get it
     Mirf.getData(c);

    // ... and write it out to the PC
    //if (c == 45)
    for (int i = 0; c[i] != 0; i++)
     Serial.print((char)c[i]);
    Serial.println();
     //Serial.println("Received Data");
  }
}

