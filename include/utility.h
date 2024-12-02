#ifndef UTILITY_H
#define UTILITY_H

#include "bn_log.h"

// Global Macros
#define MAX_GAME_OBJECTS 50

#define PLAYER_OBJECT_LIST_INDEX 0
#define SCYTHE_OBJECT_LIST_INDEX 1

#define BLOCK_INDEX       1
#define ONEWAYBLOCK_INDEX 2
#define SLOW_BLOCK_INDEX  3

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
