#include <Bluetooth_HC05.h>

Bluetooth_HC05 hc05;

void setup()
{
  /* Speed: 38400; HC-05 RESET: pin 2; HC-05 MODE (PIO11): pin 3 */
  hc05.begin(38400, 2, 3, HC05_MODE_COMMAND);
  /* Wait until HC-05 starts */
  delay(700);
  /* Allow HC-05 to initiate connections */
  hc05.setRole(HC05_ROLE_MASTER);
  /* Cannot connect without this */
  hc05.initSerialPortProfile();
  /* It's ridiculous: HC-05 cannot connect to anything without inquiring -
     no matter will it found any device or not!
     Iquiring may take pretty much time, thus 10 seconds of timeout. */
  hc05.inquire(NULL, 10000);
  /* Slave module says "+ADDR:11:4:290255" on "AT+ADDR?" command */
  BluetoothAddress slave = { 0x60, 0xD8, 0x19, 0xBE, 0x9A, 0x49 };
  //BluetoothAddress slave = { 0x00, 0x1F, 0x81, 0x00, 0x08, 0x30};
  hc05.connect(slave);
}

void loop()
{
  hc05.println("Blablabla");
  delay(1000);
}
