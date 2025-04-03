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
#define MAX_GAME_OBJECTS     10
#define MAX_UNLOADED_OBJECTS 25

#define LOAD_RANGE_HALF_W 180
#define LOAD_RANGE_HALF_H 160

#define ROOM_MAX_COLLIDERS 1800
#define ROOM_MAX_WIDTH     128
#define ROOM_MAX_HEIGHT    64

// === Object Indexes === //
#define PLAYER_OBJECT_LIST_INDEX  0

// === Tile Indexes === //
#define NO_BLOCK_INDEX              0
#define HARD_BLOCK_MIN_INDEX        1
#define HARD_BLOCK_MAX_INDEX        137

#define ONEWAY_BLOCK_INDEX          146
#define SLOW_BLOCK_INDEX            147

#define UP_SPIKE_BLOCK_INDEX        148
#define DOWN_SPIKE_BLOCK_INDEX      149
#define RIGHT_SPIKE_BLOCK_INDEX     150
#define LEFT_SPIKE_BLOCK_INDEX      151

#define LEFT_SHALLOW_SLOPE_1_INDEX  152
#define LEFT_SHALLOW_SLOPE_2_INDEX  153
#define LEFT_SHALLOW_SLOPE_3_INDEX  154
#define LEFT_SHALLOW_SLOPE_4_INDEX  155

#define LEFT_STEEP_SLOPE_1_INDEX 156
#define LEFT_STEEP_SLOPE_2_INDEX 157

#define RIGHT_SHALLOW_SLOPE_1_INDEX  158
#define RIGHT_SHALLOW_SLOPE_2_INDEX  159
#define RIGHT_SHALLOW_SLOPE_3_INDEX  160
#define RIGHT_SHALLOW_SLOPE_4_INDEX  161

#define RIGHT_STEEP_SLOPE_1_INDEX 162
#define RIGHT_STEEP_SLOPE_2_INDEX 163

#define TILE_TOTAL_COUNT 164

// Tile Dimensions //

#define TILE_WIDTH  8
#define TILE_HEIGHT 8

#define WORLD_X_OFFSET 4
#define WORLD_Y_OFFSET 4

// Collider Dimensions //

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
