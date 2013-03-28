/*
 * Header file for shift register library
 */

#ifndef SHIFT_h
#define SHIFT_h

#include <Arduino.h>

#define SR_ENABLE_PIN 9
#define SR_CLEAR_PIN 11
#define SR_CLOCK_PIN 10
#define SR_INPUT_PIN 8

#define SR_NONE_VAL B00000000
#define SR_A_VAL B10000000
#define SR_B_VAL B01000000
#define SR_C_VAL B00100000
#define SR_D_VAL B00010000
#define SR_E_VAL B00001000
#define SR_F_VAL B00000100
#define SR_G_VAL B00000010
#define SR_H_VAL B00000001
#define SR_ALL_VAL B11111111

#define NONE 0
#define A 1
#define B 2
#define C 3
#define D 4
#define E 5
#define F 6
#define G 7
#define H 8
#define ALL 9

class Shift
{
	public:
	  Shift();
	  void enableNone();
	  void enableB();
	  void enableA();
	  void enableC();
	  void enableD();
	  void enableE();
	  void enableF();
	  void enableG();
	  void enableH();
	  void enableAll();
	  int currentEnabled();

	protected:
	  void setRegister(uint8_t val);
	  void enableOutput();
	  void disableOutput();
	  void clearRegister();
	  void shiftOnce();

	private:
	  int m_current_val;
};
#endif
