#include <Arduino.h>
#include <Wire.h>
#include <Conversation.h>
#include <SPI.h>
#include <MirfSpiDriver.h>
#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>
#include <nRF24L01.h>
#include <Nordic.h>
#include <Packet.h>
#include <AFMotor.h>
#include <HMC5883L.h>
#include <Motors_Compass.h>
#include <Distance.h>

//Uncomment for print statements
#define DEBUG

//All but one should be commented out
#define MCP
//#define SMITH
//#define VIKI
//#define WOPR

#ifdef MCP
#define print_name() Serial.println("I am MCP")
byte id = 0;
int x_position = 0;
int y_position = 20;
#endif

#ifdef SMITH
#define print_name() Serial.println("I am Smith")
byte id = 1;
int x_position = 20;
int y_position = 0;
#endif

#ifdef VIKI
#define print_name() Serial.println("I am Viki")
byte id = 2;
int x_position = 0;
int y_position = -20;
#endif

#ifdef WOPR
#define print_name() Serial.println("I am WOPR")
byte id = 3;
int x_position = -20;
int y_position = 0;
#endif

Nordic nord(id);
Distance dist = Distance();
Motors_Compass motors_compass;

void setup() {
  Serial.begin(9600); 
  
  #ifdef DEBUG
  print_name();
  Serial.print("Id = ");
  Serial.println(id);
  Serial.print("Starting position = (");
  Serial.print(x_position);
  Serial.print(",");
  Serial.print(y_position);
  Serial.println(")");
  #endif  
  
  motors_compass.initialize();
  motors_compass.calibrate_compass();
  
  #ifdef DEBUG
  Serial.println("Finished initializing\n");
  #endif
  delay(5000);
}

void loop() {
  Packet pkt;
  while(pkt.getSourceId() == NULLDATA){  
    pkt = nord.waitForPacket(1000);
    #ifdef DEBUG
    Serial.println("");
    #endif
  }
  switch (pkt.getType()) {
    case REQ_POS:
      #ifdef DEBUG
      Serial.println("\nReceived a position request");
      #endif     
      send_position(pkt);
      break;    
    case REQ_DIST_LEFT:
      #ifdef DEBUG
      Serial.println("\nReceived a request for left distance");
      #endif      
      send_left_dist(pkt);
      break;
    case REQ_DIST_RIGHT:
      #ifdef DEBUG
      Serial.println("\nReceived a request for right distance");
      #endif      
      send_right_dist(pkt);
      break;
    case REQ_DIST_FWD:
      #ifdef DEBUG
      Serial.println("\nReceived a request for forward distance");
      #endif      
      send_front_dist(pkt);
      break;
    case REQ_DIST_BWD:
      #ifdef DEBUG
      Serial.println("\nReceived a request for back distance");
      #endif      
      send_back_dist(pkt);
      break;      
    case COMMAND:
      #ifdef DEBUG
      Serial.println("\nReceived a command");
      #endif      
      execute_command(pkt);
      break;       
  }    
}

void send_position(Packet pkt){
  int heading = (int)motors_compass.get_compass_heading();
  #ifdef DEBUG
  Serial.println("Attempting to send position");
  Serial.print("Position = (");
  Serial.print(x_position);
  Serial.print(",");
  Serial.print(y_position);
  Serial.println(")");
  Serial.print("Heading = ");
  Serial.println(heading);
  #endif    
  nord.sendPosX(pkt.getSourceId(), x_position);
  nord.sendPosY(pkt.getSourceId(), y_position);
  nord.sendHeading(pkt.getSourceId(), heading); 
}

void send_left_dist(Packet pkt){
  int left = dist.check_left();
  #ifdef DEBUG
  Serial.println("Attempting to send left distance");
  Serial.print("Distance = ");
  Serial.println(left);
  #endif   
  nord.sendLeftDist(pkt.getSourceId(), left);
}

void send_right_dist(Packet pkt){
  int right = 200; //Out of range
  #ifndef VIKI
  right = dist.check_right();
  #endif
  #ifdef DEBUG
  Serial.println("Attempting to send right distance");
  Serial.print("Distance = ");
  Serial.println(right);
  #endif     
  nord.sendRightDist(pkt.getSourceId(), right);
}

void send_front_dist(Packet pkt){
  int front = dist.check_front();
  #ifdef DEBUG
  Serial.println("Attempting to send front distance");
  Serial.print("Distance = ");
  Serial.println(front);
  #endif     
  nord.sendFwdDist(pkt.getSourceId(), front);
}

void send_back_dist(Packet pkt){
  int back = dist.check_back();
  #ifdef DEBUG
  Serial.println("Attempting to send back distance");
  Serial.print("Distance = ");
  Serial.println(back);
  #endif     
  nord.sendBwdDist(pkt.getSourceId(), back);
}

void execute_command(Packet pkt){
  #ifdef DEBUG
  Serial.println("Attempting to parse command");
  #endif   
  switch (get_first_data_byte(pkt)) {
    case MOVE:
      move(pkt);
      break; 
    case TURN:
      turn(pkt);  
      break;
  }  
}

void move(Packet pkt){
  float current_heading = motors_compass.get_compass_heading();
  float distance = (float)get_second_data_byte(pkt);
  #ifdef DEBUG
  Serial.print("Attempting to move ");
  Serial.print(distance);
  Serial.print(" cm at a heading of ");
  Serial.print(current_heading);
  Serial.println(" degrees");
  #endif   
  motors_compass.go_straight(current_heading, distance);
  float current_heading_radians = current_heading * M_PI/180;
  x_position += (int)(distance * cos(current_heading_radians));
  y_position += (int)(distance * sin(current_heading_radians));
  send_position(pkt);
}

void turn(Packet pkt){
  float desired_heading = (float)get_second_data_byte(pkt);
  #ifdef DEBUG
  Serial.print("Attempting to turn to a heading of ");
  Serial.print(desired_heading);
  Serial.println(" degrees");
  #endif   
  motors_compass.turn_to_orientation(desired_heading);
  send_position(pkt);
}

unsigned int get_first_data_byte(Packet pkt){
  byte* data_ptr = pkt.getData();
  unsigned int d = *data_ptr; 
  return d; 
}

unsigned int get_second_data_byte(Packet pkt){
  byte* data_ptr = pkt.getData();
  unsigned int d = *(++data_ptr); 
  return d;   
}

unsigned int get_both_data_bytes(Packet pkt){
  return (get_first_data_byte(pkt)<<8 | get_second_data_byte(pkt));  
}


