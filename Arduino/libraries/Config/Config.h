#ifndef CONFIG_H
#define CONFIG_H


//All but one should be commented out
//#define MCP
//#define SMITH
#define VIKI
//#define WOPR

#ifdef MCP
#define ROBO_ID 0
#define X_POS 20
#define Y_POS 0
#define STRAIGHT_SPEED 150
#define STRAIGHT_ITERATIONS 1000
#endif

#ifdef SMITH
#define ROBO_ID 1
#define X_POS 0
#define Y_POS 20
#define STRAIGHT_SPEED 150
#define STRAIGHT_ITERATIONS 1000
#endif

#ifdef VIKI
#define ROBO_ID 2
#define X_POS 0
#define Y_POS -20
#define STRAIGHT_SPEED 150
#define STRAIGHT_ITERATIONS 1000
#endif

#ifdef WOPR
#define ROBO_ID 3
#define X_POS -20
#define Y_POS 0
#define STRAIGHT_SPEED 255
#define STRAIGHT_ITERATIONS 10
#endif


#endif
