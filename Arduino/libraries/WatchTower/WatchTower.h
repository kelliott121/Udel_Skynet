/*
 * Header file for distance sensing library
 */

#ifndef WATCHTOWER_h
#define WATCHTOWER_h

#include <Conversation.h>
#include <SPI.h>
#include <MirfSpiDriver.h>
#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>
#include <nRF24L01.h>
#include <Packet.h>
#include <Nordic.h>

typedef struct {
  byte id;
  unsigned int pos_x;
  unsigned int pos_y;
  unsigned int orientation;
} BOT;

#define MCP_ID 0
#define SMITH_ID 1
#define VIKI_ID 2
#define WOPR_ID 3

class WatchTower
{
	public:
	  WatchTower();
	  BOT get_bot(byte id);
	  void update_pos_x(byte id, unsigned int pos_x);
	  void update_pos_y(byte id, unsigned int pos_y);
	  void update_orientation(byte id, unsigned int orientation);
	  unsigned int get_packet_data(Packet p);

	  BOT NO_SUCH_BOT;
	  BOT MCP;
	  BOT WOPR;
	  BOT VIKI;
	  BOT SMITH;
};
#endif
