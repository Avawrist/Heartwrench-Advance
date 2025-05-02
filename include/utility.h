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

#define LOAD_RANGE_W  360
#define LOAD_RANGE_H  280
#define SCREEN_W 240
#define SCREEN_H 160

#define ROOM_MAX_COLLIDERS 1800
#define ROOM_MAX_WIDTH     128
#define ROOM_MAX_HEIGHT    64

// === Object Indexes === //
#define PLAYER_OBJECT_LIST_INDEX 0

// === Tile Indexes === //
#define NO_BLOCK_INDEX              0
#define HARD_BLOCK_MIN_INDEX        1
#define HARD_BLOCK_MAX_INDEX        158

#define UP_SPIKE_BLOCK_1_INDEX    159  
#define UP_SPIKE_BLOCK_2_INDEX    165
#define DOWN_SPIKE_BLOCK_1_INDEX  160
#define DOWN_SPIKE_BLOCK_2_INDEX  166
#define LEFT_SPIKE_BLOCK_1_INDEX  161
#define LEFT_SPIKE_BLOCK_2_INDEX  162
#define RIGHT_SPIKE_BLOCK_1_INDEX 163
#define RIGHT_SPIKE_BLOCK_2_INDEX 164 

#define ONEWAY_BLOCK_MIN_INDEX 167
#define ONEWAY_BLOCK_MAX_INDEX 170

#define LEFT_SHALLOW_SLOPE_1_INDEX  171
#define LEFT_SHALLOW_SLOPE_2_INDEX  172
#define LEFT_SHALLOW_SLOPE_3_INDEX  173
#define LEFT_SHALLOW_SLOPE_4_INDEX  174

#define LEFT_STEEP_SLOPE_1_INDEX 175
#define LEFT_STEEP_SLOPE_2_INDEX 176

#define RIGHT_SHALLOW_SLOPE_1_INDEX  177
#define RIGHT_SHALLOW_SLOPE_2_INDEX  178
#define RIGHT_SHALLOW_SLOPE_3_INDEX  179
#define RIGHT_SHALLOW_SLOPE_4_INDEX  180

#define RIGHT_STEEP_SLOPE_1_INDEX 181
#define RIGHT_STEEP_SLOPE_2_INDEX 182

#define TILE_TOTAL_COUNT 183

// Tile Dimensions //
#define TILE_WIDTH  8
#define TILE_HEIGHT 8

#define WORLD_X_OFFSET 4
#define WORLD_Y_OFFSET 4

// Collider Dimensions //
#define ONEWAYBLOCK_COLLIDER_WIDTH     8
#define ONEWAYBLOCK_COLLIDER_HEIGHT    1
#define ONEWAYBLOCK_COLLIDER_Y_OFFSET -4

// Enums
enum XDirection
{
    LEFT  = -1,
    RIGHT =  1
};

enum YDirection
{
    UP    = -1,
    DOWN  =  1,
};

enum ScreenShakeSeverity
{
    NO_SHAKE     = 0,
    WEAK_SHAKE   = 1,
    STRONG_SHAKE = 2,
};

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

// Global Variables
extern int32               hitstop_frames;
extern int32               screenshake_frames;
extern ScreenShakeSeverity screenshake_severity;

// Debug Mode
#define DEBUG_BUILD false

// Functions

int32 getTileAtBGIndex(uint32 x, uint32 y, 
                       bn::regular_bg_ptr                      bg_ptr, 
                       bn::span<const bn::regular_bg_map_cell> cells,
                       bn::regular_bg_item                     bg_item);

#endif
