#ifndef UTILITY_H
#define UTILITY_H

#include "bn_log.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_map_cell.h"
#include "bn_regular_bg_map_cell_info.h"

// Types
typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;
typedef char  int8;
typedef short int16;
typedef int   int32;

// Global Macros
#define MAX_GAME_OBJECTS 50

#define PLAYER_OBJECT_LIST_INDEX  0

#define NO_BLOCK_INDEX              0
#define SOFT_BLOCK_INDEX            1
#define HARD_BLOCK_INDEX            2

#define ONEWAY_BLOCK_INDEX          3

#define SLOW_BLOCK_INDEX            4

#define UP_SPIKE_BLOCK_INDEX        5 
#define DOWN_SPIKE_BLOCK_INDEX      6
#define RIGHT_SPIKE_BLOCK_INDEX     7
#define LEFT_SPIKE_BLOCK_INDEX      8

#define LEFT_SHALLOW_SLOPE_1_INDEX  9
#define LEFT_SHALLOW_SLOPE_2_INDEX  10
#define LEFT_SHALLOW_SLOPE_3_INDEX  11
#define LEFT_SHALLOW_SLOPE_4_INDEX  12

#define LEFT_STEEP_SLOPE_1_INDEX 13
#define LEFT_STEEP_SLOPE_2_INDEX 14

#define RIGHT_SHALLOW_SLOPE_1_INDEX  15
#define RIGHT_SHALLOW_SLOPE_2_INDEX  16
#define RIGHT_SHALLOW_SLOPE_3_INDEX  17
#define RIGHT_SHALLOW_SLOPE_4_INDEX  18

#define RIGHT_STEEP_SLOPE_1_INDEX 19
#define RIGHT_STEEP_SLOPE_2_INDEX 20

#define TILE_WIDTH  8
#define TILE_HEIGHT 8

#define ONEWAYBLOCK_COLLIDER_WIDTH     8
#define ONEWAYBLOCK_COLLIDER_HEIGHT    1
#define ONEWAYBLOCK_COLLIDER_Y_OFFSET -4

// Global Arrays
extern int32 left_shallow_slope_1_arr[TILE_WIDTH];
extern int32 left_shallow_slope_2_arr[TILE_WIDTH];
extern int32 left_shallow_slope_3_arr[TILE_WIDTH];
extern int32 left_shallow_slope_4_arr[TILE_WIDTH];

extern int32 left_steep_slope_1_arr[TILE_WIDTH];
extern int32 left_steep_slope_2_arr[TILE_WIDTH];

extern int32 right_shallow_slope_1_arr[TILE_WIDTH];
extern int32 right_shallow_slope_2_arr[TILE_WIDTH];
extern int32 right_shallow_slope_3_arr[TILE_WIDTH];
extern int32 right_shallow_slope_4_arr[TILE_WIDTH];

extern int32 right_steep_slope_1_arr[TILE_WIDTH];
extern int32 right_steep_slope_2_arr[TILE_WIDTH];

// Debug Mode
#define DEBUG_BUILD false

// Enums

enum Direction
{
    LEFT  = -1,
    RIGHT =  1,
    UP    = -1,
    DOWN  =  1,
};

// Functions

int32 getTileAtBGIndex(uint32 x, uint32 y, 
                       bn::regular_bg_ptr                      bg_ptr, 
                       bn::span<const bn::regular_bg_map_cell> cells,
                       bn::regular_bg_item                     bg_item);

#endif
