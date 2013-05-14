#include "Distance.h"

Distance dist = Distance();


void setup(){

}

void loop(){
  dist.send_ir_sonar_pulse();
  delay(10);
}
