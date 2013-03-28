/*
 * Shift register library
 */

#include <Arduino.h> 
#include "Shift.h"

Shift::Shift()
{
  //Set the pins to outputs
  pinMode(SR_ENABLE_PIN, OUTPUT);
  pinMode(SR_CLEAR_PIN, OUTPUT);
  pinMode(SR_CLOCK_PIN, OUTPUT);
  pinMode(SR_INPUT_PIN, OUTPUT);
  //Initialize pins
  digitalWrite(SR_ENABLE_PIN, HIGH);
  digitalWrite(SR_CLEAR_PIN, LOW);
  digitalWrite(SR_CLOCK_PIN, LOW);
  digitalWrite(SR_INPUT_PIN, LOW);
  //Init variables
  m_current_val = 0;
}

void Shift::enableNone(){setRegister(SR_NONE_VAL);}
void Shift::enableA(){setRegister(SR_A_VAL);}
void Shift::enableB(){setRegister(SR_B_VAL);}
void Shift::enableC(){setRegister(SR_C_VAL);}
void Shift::enableD(){setRegister(SR_D_VAL);}
void Shift::enableE(){setRegister(SR_E_VAL);}
void Shift::enableF(){setRegister(SR_F_VAL);}
void Shift::enableG(){setRegister(SR_G_VAL);}
void Shift::enableH(){setRegister(SR_H_VAL);}
void Shift::enableAll(){setRegister(SR_ALL_VAL);}

int Shift::currentEnabled()
{
  switch(m_current_val){
  case SR_A_VAL:
    return A;
  case SR_B_VAL:
    return B;
  case SR_C_VAL:
    return C;
  case SR_D_VAL:
    return D;
  case SR_E_VAL:
    return E;
  case SR_F_VAL:
    return F;
  case SR_G_VAL:
    return G;
  case SR_H_VAL:
    return H;
  case SR_ALL_VAL:
    return ALL;
  default:
    return NONE;
  }
}

void Shift::setRegister(uint8_t val)
{
  //Initialize
  disableOutput();
  clearRegister();
  uint8_t high_bit = 1;
  //Shift values in
  for(int i=0; i<8; i++){
    if(val & high_bit)
      digitalWrite(SR_INPUT_PIN, HIGH);
    else
      digitalWrite(SR_INPUT_PIN, LOW);
    shiftOnce();
    high_bit = (high_bit << 1);
  }
  //Send it out
  shiftOnce();
  enableOutput();
  m_current_val = val;
}

void Shift::enableOutput()
{
  digitalWrite(SR_ENABLE_PIN, LOW);
}

void Shift::disableOutput()
{
  digitalWrite(SR_ENABLE_PIN, HIGH);
}

void Shift::clearRegister()
{
  digitalWrite(SR_CLEAR_PIN, LOW);
  delay(1);
  digitalWrite(SR_CLEAR_PIN, HIGH);
}

void Shift::shiftOnce()
{
  delay(1);
  digitalWrite(SR_CLOCK_PIN, HIGH);
  delay(1);
  digitalWrite(SR_CLOCK_PIN, LOW);		 
}
