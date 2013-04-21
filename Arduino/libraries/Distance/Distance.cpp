/*
 * Distance sensing library
 */

#include <Arduino.h> 
#include "Distance.h"

Distance::Distance()
{
  //Set pin directions
  pinMode(SHIFT_SER_PIN, OUTPUT);
  pinMode(SHIFT_OE_PIN, OUTPUT);
  pinMode(SHIFT_CLK_PIN, OUTPUT);
  pinMode(PWM_PIN, OUTPUT);
  pinMode(IR_RECEIVE_PIN, INPUT);
  pinMode(ULTRASOUND_RECEIVE_PIN, INPUT);
  //Initialize output pins
  digitalWrite(SHIFT_SER_PIN, LOW);
  digitalWrite(SHIFT_OE_PIN, HIGH);
  digitalWrite(SHIFT_CLK_PIN, LOW);
  //Initialize variables

  
}

///////////////////////
// Public functions
///////////////////////
int Distance::check_front()
{
  return checkModule(FS_BIT);
}

int Distance::check_left()
{
  return checkModule(LS_BIT);
}

int Distance::check_right()
{
  return checkModule(RS_BIT);
}

int Distance::check_back()
{
  return checkModule(BS_BIT);
}

void Distance::send_ir_sonar_pulse()
{
  checkModule(FS_BIT | LS_BIT | RS_BIT | BS_BIT | IR_BIT);
}

void Distance::test_init(){
  TCCR2A = _BV(COM2A0) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(WGM22) | _BV(CS20);
  OCR2A = B11000111;
}

int Distance::test_loop(){
  //digitalWrite(A1, HIGH);
  //delayMicroseconds(500);
  //digitalWrite(A1, LOW);
  long duration, distance;
  delay(250);
  setShift(B10000000, 500);
  duration = pulseIn(ULTRASOUND_RECEIVE_PIN, HIGH);
  distance = (duration * CM_PER_MICROSECOND) + FIXED_OFFSET;
  return (int)distance;
}

void receive_ir(){
  long duration, distance;
  setShift(FS_BIT | LS_BIT | RS_BIT | BS_BIT, PULSE_TIME);
  duration = pulseIn(ULTRASOUND_RECEIVE_PIN, HIGH);
  distance = (.13077*pow(duration, 1-.2421)*2);
  Serial.print("Distance: ");
  Serial.println(distance);
}

///////////////////////
// Internal functions
///////////////////////
int Distance::checkModule(uint8_t module)
{
  long duration, distance;
  startPwm();
  setShift(module, PULSE_TIME);
  duration = pulseIn(ULTRASOUND_RECEIVE_PIN, HIGH);
  //distance = (duration * CM_PER_MICROSECOND) + FIXED_OFFSET;
  distance = (.13077*pow(duration,1-.2421));
  stopPwm();
  delay(250);
  return (int)distance;
}

///////////////////////
// Shift Register 
///////////////////////
void Distance::setShift(uint8_t val, int micro)
{
  uint8_t high_bit = 1;
  //Shift values in
  for(int i=0; i<8; i++){
    if(val & high_bit)
      digitalWrite(SHIFT_SER_PIN, HIGH);
    else
      digitalWrite(SHIFT_SER_PIN, LOW);
    shiftOnce();
    high_bit = (high_bit << 1);
  }
  //Send it out
  shiftOnce();
  enableOutput();
  delayMicroseconds(micro);
  disableOutput();
}

void Distance::enableOutput()
{
  digitalWrite(SHIFT_OE_PIN, LOW);
}

void Distance::disableOutput()
{
  digitalWrite(SHIFT_OE_PIN, HIGH);
}

void Distance::shiftOnce()
{
  delayMicroseconds(10);
  digitalWrite(SHIFT_CLK_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(SHIFT_CLK_PIN, LOW);		 
}

///////////////////////
// PWM
///////////////////////
void Distance::startPwm()
{
  TCCR2A = _BV(COM2A0) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(WGM22) | _BV(CS20);
  OCR2A = B11000111;
}

void Distance::stopPwm()
{
  TCCR2A = 0;
  TCCR2B = 0;
}
