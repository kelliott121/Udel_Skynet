/*
 * Header file for distance sensing library
 */

#ifndef DISTANCE_h
#define DISTANCE_h

#include <Arduino.h>

#define SHIFT_SER_PIN A0
#define SHIFT_OE_PIN A1
#define SHIFT_CLK_PIN A2

#define PWM_PIN 11

#define IR_RECEIVE_PIN 2
#define ULTRASOUND_RECEIVE_PIN 3

#define FS_BIT B10000000
#define LS_BIT B01000000
#define RS_BIT B00100000 
#define BS_BIT B00010000
#define IR_BIT B00001000

#define CM_PER_MICROSECOND .034
#define FIXED_OFFSET 0

class Distance
{
	public:
	  Distance();
	  int check_front();
	  int check_left();
	  int check_right();
	  int check_back();
	  void send_ir_sonar_pulse();

	protected:
	  int checkModule(uint8_t module);
	  //Shift Register
	  void setShift(uint8_t val, uint8_t micro);
	  void enableOutput();
	  void disableOutput();
	  void shiftOnce();
	  //PWM
	  void startPwm();
	  void stopPwm();

	  //private:
};
#endif
