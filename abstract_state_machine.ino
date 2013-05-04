#include <NewPing.h>

#include <DistanceGP2Y0A21YK.h>
#include <PololuWheelEncoders.h>

//STATE DEFINES
#define START_STATE 0
#define FWD_STATE 1
#define BWD_STATE 2
#define RIGHT_STATE 3
#define LEFT_STATE 4

//SONAR PINS
#define TRIGGER_PIN  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN  2     // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

//SWITCH PIN
#define SWITCHES 7

//MOTOR DEFINES
#define LEFT 1
#define RIGHT 0
#define FORWARD 0
#define BACKWARD 1
#define TOPSPEED 255
#define STBY 10

//MAZE DEFINES
#define SQUARE_SIZE 16.8

//MOTOR PINS
//Motor A
int PWMA = 3; //Speed control 
int AIN1 = 9; //Direction
int AIN2 = 8; //Direction

//Motor B
int PWMB = 5; //Speed control
int BIN1 = 11; //Direction
int BIN2 = 12; //Direction

//MOTOR TUNING VALUES
int leftTune = 0;
int rightTune = 0;
int distanceLeft;
int distanceRight;
int distanceFront;
int startDistanceFront;

int state;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

//DISTANCE SENSORS
DistanceGP2Y0A21YK leftDist;
DistanceGP2Y0A21YK rightDist;

//ENCODERS
PololuWheelEncoders encoders;

void setup() {
  //state = START_STATE;
  //state = FWD_STATE;
  Serial.begin(9600);

  //SETUP DISTANCE SENSORS
  leftDist.begin(A0);
  rightDist.begin(A1);

  //SETUP ENCODERS
  encoders.init(A2, A3, A4, A5);

  pinMode(SWITCHES, INPUT);
  digitalWrite(SWITCHES, HIGH);

  delay(5000);
}

void loop() {
  getData();
  startDistanceFront = distanceFront;
  sendData();
  getState();
  if (digitalRead(SWITCHES) == LOW){
    move_bwd();
    delay(250);
    stop();
  }
  else if (state == FWD_STATE){
    fwd_state();
  }
  else if (state == RIGHT_STATE){
    r_state();
  }
  else if (state == LEFT_STATE){
    l_state();
  }
  else{
    /*while (sonar.ping()/US_ROUNDTRIP_CM > SQUARE_SIZE/5){
     //TURN LEFT
     move(RIGHT, TOPSPEED/4 + (rightTune++), FORWARD);
     move(LEFT, TOPSPEED/4 + (rightTune++), FORWARD);
     delay(100);
     stop();
     }
     state = FWD_STATE;*/
  }
  stop();
  delay(1000);
}

void getData(){
  distanceLeft = leftDist.getDistanceCentimeter();
  distanceRight = rightDist.getDistanceCentimeter();
  distanceFront = sonar.ping()/US_ROUNDTRIP_CM;
  //startDistanceFront = distanceFront;
}

void sendData(){

}

void getState(){
  if (distanceFront > 5){
    state = FWD_STATE; 
  }
  else if (distanceLeft > SQUARE_SIZE){
    state = LEFT_STATE;
  }
  else if (distanceRight > SQUARE_SIZE){
    state = RIGHT_STATE;
  }
  else{
    state = RIGHT_STATE;
  }
}

void move(int motor, int speed, int direction){
  //Move specific motor at speed and direction
  //motor: 0 for B 1 for A
  //speed: 0 is off, and 255 is full speed
  //direction: 0 clockwise, 1 counter-clockwise

  digitalWrite(STBY, HIGH); //disable standby

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if(direction == 1){
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if(motor == 1){
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, speed);
  }
  else{
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, speed);
  }
}

void stop(){
  //enable standby  
  digitalWrite(STBY, LOW); 
}

void move_right(int mult){
  //TURN RIGHT
  move(LEFT, TOPSPEED/4+(mult * rightTune++), FORWARD);
  move(RIGHT, TOPSPEED/4+(mult * rightTune++), BACKWARD);
}

void veer_right(){
  //TURN RIGHT
  move(LEFT, TOPSPEED/4, FORWARD);
  //move(RIGHT, TOPSPEED/4+10, BACKWARD);
}

void move_left(int mult){
  //TURN LEFT
  move(RIGHT, TOPSPEED/4+(mult * leftTune++), FORWARD);
  move(LEFT, TOPSPEED/4+(mult * leftTune++), BACKWARD);
}

void veer_left(){
  //TURN LEFT
  move(RIGHT, TOPSPEED/4, FORWARD);
  //move(LEFT, TOPSPEED/4+10, BACKWARD);
}

void move_fwd(){
  move(RIGHT, TOPSPEED/3, FORWARD);
  move(LEFT, TOPSPEED/3, FORWARD);
}

void move_bwd(){
  move(RIGHT, TOPSPEED/3, BACKWARD);
  move(LEFT, TOPSPEED/3, BACKWARD);
}

void fwd_state(){
  while (startDistanceFront - distanceFront < SQUARE_SIZE && distanceFront > 5){
    //if (distanceFront < 5){
    //  stop();
    //}
    if (distanceLeft < SQUARE_SIZE && distanceRight < SQUARE_SIZE){
      //WALL ON BOTH SIDES 
      centered_fwd();
    }
    else if (distanceLeft >= SQUARE_SIZE && distanceRight < SQUARE_SIZE){
      //WALL ON RIGHT
      follow_right_wall();
      //stop();
    }
    else if (distanceRight >= SQUARE_SIZE && distanceLeft < SQUARE_SIZE){
      //WALL ON LEFT
      follow_left_wall();
      //stop();
    }
    else{
      //NO WALLS!
      move_fwd();
      //stop();
    }
    
    if (digitalRead(SWITCHES) == LOW){
      move_bwd();
      delay(250);
      stop();
    }
    
    delay(10);
    getData();

    
  }
  leftTune = 0;
  rightTune = 0;
}

void centered_fwd(){
  if (distanceLeft - distanceRight > 1){
    move_left(1);
  }
  else if (distanceRight - distanceLeft > 1){
    move_right(1);
  }
  else{
    move_fwd();
  } 
}

void follow_left_wall(){
  if (distanceLeft < SQUARE_SIZE/2 - SQUARE_SIZE/10){
    veer_right();
  }
  else if (distanceLeft > SQUARE_SIZE/2 + SQUARE_SIZE/10){
    veer_left();
  }
  else{
    move_fwd(); 
  }
}

void follow_right_wall(){
  if (distanceRight < SQUARE_SIZE/2 - SQUARE_SIZE/10){
    veer_left();
  }
  else if (distanceRight > SQUARE_SIZE/2 + SQUARE_SIZE/10){
    veer_right();
  }
  else{
    move_fwd(); 
  }
}

void l_state(){
  while (distanceFront < SQUARE_SIZE || distanceRight < SQUARE_SIZE/2 - SQUARE_SIZE/10){
    move_left(1);

    if (digitalRead(SWITCHES) == LOW){
      move_bwd();
      delay(250);
      stop();
    }
    delay(10);
    getData();
  }
  leftTune = 0;
  rightTune = 0;
  delay(100);
  while (distanceFront < SQUARE_SIZE || distanceRight < SQUARE_SIZE/2 - SQUARE_SIZE/10){
    move_left(1);

    if (digitalRead(SWITCHES) == LOW){
      move_bwd();
      delay(250);
      stop();
    }

    delay(10);
    getData();
  }
  leftTune = 0;
  rightTune = 0;
}

void r_state(){
  while (distanceFront < SQUARE_SIZE || distanceLeft < SQUARE_SIZE/2 - SQUARE_SIZE/10){
    move_right(1);

    if (digitalRead(SWITCHES) == LOW){
      move_bwd();
      delay(250);
      stop();
    }

    delay(10);
    getData();
  }
  leftTune = 0;
  rightTune = 0;
  delay(100);
  while (distanceFront < SQUARE_SIZE || distanceLeft < SQUARE_SIZE/2 - SQUARE_SIZE/10){
    move_right(1);

    if (digitalRead(SWITCHES) == LOW){
      move_bwd();
      delay(250);
      stop();
    }
    delay(10);
    getData();
  }
  leftTune = 0;
  rightTune = 0;
}

/* 
 void r_state(){
 distanceLeft = leftDist.getDistanceCentimeter();
 distanceRight = rightDist.getDistanceCentimeter();
 while (sonar.ping()/US_ROUNDTRIP_CM < .8*SQUARE_SIZE || distanceLeft < SQUARE_SIZE/3){
 //TURN RIGHT
 move(LEFT, TOPSPEED/4 + (leftTune++), FORWARD);
 move(RIGHT, TOPSPEED/4 + (leftTune++), BACKWARD);
 delay(100);
 stop();
 distanceLeft = leftDist.getDistanceCentimeter();
 distanceRight = rightDist.getDistanceCentimeter();
 }
 state = FWD_STATE;
 }
 
 void l_state(){
 distanceLeft = leftDist.getDistanceCentimeter();
 distanceRight = rightDist.getDistanceCentimeter();
 while (sonar.ping()/US_ROUNDTRIP_CM < .8*SQUARE_SIZE || distanceRight < SQUARE_SIZE/3){
 //TURN LEFT
 move(RIGHT, TOPSPEED/4 + (rightTune++), FORWARD);
 move(LEFT, TOPSPEED/4 + (rightTune++), BACKWARD);
 delay(100);
 stop();
 distanceLeft = leftDist.getDistanceCentimeter();
 distanceRight = rightDist.getDistanceCentimeter();
 }
 state = FWD_STATE;
 }
 */
















