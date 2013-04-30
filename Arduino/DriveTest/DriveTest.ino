#include <AFMotor.h>
#include <Drive.h>

Drive drive;
void setup() {
  
}

void loop() {
  // put your main code here, to run repeatedly: 
  drive.left(1);
  delay(5000);
  drive.right(1);
  delay(5000);
}
