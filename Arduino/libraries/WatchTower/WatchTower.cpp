/*
 * Distance sensing library
 */

#include <Arduino.h> 
#include "WatchTower.h"

WatchTower::WatchTower()
{
  NO_SUCH_BOT = {-1, -1, -1, -1};
  MCP = {MCP_ID, -1, -1, -1};
  WOPR = {WOPR_ID, -1, -1, -1};
  VIKI = {VIKI_ID, -1, -1, -1};
  SMITH = {SMITH_ID, -1, -1, -1};
}

///////////////////////
// Public functions
///////////////////////
BOT WatchTower::get_bot(byte id){
  switch (id) {
    case 0:
      return MCP;
    case 1:
      return WOPR;
    case 2:
      return VIKI;
    case 3:
      return SMITH;
    default:
      return NO_SUCH_BOT;    
  }   
}

void WatchTower::update_pos_x(byte id, unsigned int pos_x) {
  get_bot(id).pos_x = pos_x;
}

void WatchTower::update_pos_y(byte id, unsigned int pos_y) {
  get_bot(id).pos_y = pos_y; 
}

void WatchTower::update_orientation(byte id, unsigned int orientation) {
  get_bot(id).orientation = orientation;
}

unsigned int WatchTower::get_packet_data(Packet p){
  byte* data_ptr = p.getData();
  unsigned int i1 = (*data_ptr);
  unsigned int i2 = (*(++data_ptr));
  return ( i1<<8 | i2 ); 
}








