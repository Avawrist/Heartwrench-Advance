#ifndef UTILITY_H
#define UTILITY_H

#include "bn_log.h"

// Global Macros
#define MAX_GAME_OBJECTS 50

#define PLAYER_OBJECT_LIST_INDEX  0
#define SCYTHE_OBJECT_LIST_INDEX  1
#define EXIT_1_OBJECT_LIST_INDEX  2
#define EXIT_2_OBJECT_LIST_INDEX  3
#define EXIT_3_OBJECT_LIST_INDEX  4
#define EXIT_4_OBJECT_LIST_INDEX  5

#define NO_BLOCK_INDEX     0
#define SOFT_BLOCK_INDEX   1
#define HARD_BLOCK_INDEX   2
#define ONEWAY_BLOCK_INDEX 3
#define SLOW_BLOCK_INDEX   4
#define UP_SPIKE_BLOCK_INDEX    5 
#define DOWN_SPIKE_BLOCK_INDEX  6
#define LEFT_SPIKE_BLOCK_INDEX  7
#define RIGHT_SPIKE_BLOCK_INDEX 8

#define TILE_WIDTH  8
#define TILE_HEIGHT 8

#define ONEWAYBLOCK_COLLIDER_WIDTH     8
#define ONEWAYBLOCK_COLLIDER_HEIGHT    1
#define ONEWAYBLOCK_COLLIDER_Y_OFFSET -4

// Types
typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;
typedef char  int8;
typedef short int16;
typedef int   int32;

// Debug Mode
#define DEBUG_BUILD false

// Enums

enum Direction
{
    LEFT  = -1,
    RIGHT = 1,
    UP    = -1,
    DOWN  = 1
};

#endif
