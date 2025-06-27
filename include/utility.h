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

#define LOAD_RANGE_W  500
#define LOAD_RANGE_H  400
#define SCREEN_W 240
#define SCREEN_H 160
#define SCREEN_LOAD_PADDING 128

#define CAM_PLAYER_Y_OFFSET -16

#define ROOM_MAX_COLLIDERS 1800
#define ROOM_MAX_WIDTH     128
#define ROOM_MAX_HEIGHT    64

// === Object Indexes === //
#define PLAYER_OBJECT_LIST_INDEX 0

// === Tile Indexes === //
#define NO_BLOCK_INDEX              0
#define HARD_BLOCK_MIN_INDEX        1
#define HARD_BLOCK_MAX_INDEX        189

#define H_GEAR_LEFT    196
#define H_GEAR_MID_MIN 197
#define H_GEAR_MID_MAX 199
#define H_GEAR_RIGHT   200

#define V_GEAR_TOP    201
#define V_GEAR_MID_1  203
#define V_GEAR_MID_2  205
#define V_GEAR_MID_3  218
#define V_GEAR_BOTTOM 220

#define SPIKE_MIN_INDEX           222
#define UP_SPIKE_BLOCK_1_INDEX    222
#define UP_SPIKE_BLOCK_2_INDEX    228
#define DOWN_SPIKE_BLOCK_1_INDEX  223
#define DOWN_SPIKE_BLOCK_2_INDEX  229
#define LEFT_SPIKE_BLOCK_1_INDEX  224
#define LEFT_SPIKE_BLOCK_2_INDEX  225
#define RIGHT_SPIKE_BLOCK_1_INDEX 226
#define RIGHT_SPIKE_BLOCK_2_INDEX 227 
#define SPIKE_MAX_INDEX           227

#define ONEWAY_BLOCK_MIN_INDEX 230
#define ONEWAY_BLOCK_MAX_INDEX 233

#define LEFT_SHALLOW_SLOPE_1_INDEX  234
#define LEFT_SHALLOW_SLOPE_2_INDEX  235
#define LEFT_SHALLOW_SLOPE_3_INDEX  236
#define LEFT_SHALLOW_SLOPE_4_INDEX  237

#define LEFT_STEEP_SLOPE_1_INDEX 238
#define LEFT_STEEP_SLOPE_2_INDEX 239

#define RIGHT_SHALLOW_SLOPE_1_INDEX  240
#define RIGHT_SHALLOW_SLOPE_2_INDEX  241
#define RIGHT_SHALLOW_SLOPE_3_INDEX  242
#define RIGHT_SHALLOW_SLOPE_4_INDEX  243

#define RIGHT_STEEP_SLOPE_1_INDEX 244
#define RIGHT_STEEP_SLOPE_2_INDEX 245

#define TILE_TOTAL_COUNT 245

// Tile Dimensions //
#define TILE_WIDTH  8
#define TILE_HEIGHT 8

#define WORLD_X_OFFSET 4
#define WORLD_Y_OFFSET 4

// Collider Dimensions //
#define ONEWAYBLOCK_COLLIDER_WIDTH     8
#define ONEWAYBLOCK_COLLIDER_HEIGHT    1
#define ONEWAYBLOCK_COLLIDER_Y_OFFSET -4

// Tile Macros
#define SPIKE_DAMAGE 10

// Enums
enum XDirection
{
    LEFT   = -1,
    X_NONE =  0,
    RIGHT  =  1
};

enum YDirection
{
    UP     = -1,
    Y_NONE =  0,
    DOWN   =  1,
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
extern int32               global_bg_hitflash_frames;
extern int32               global_hitstop_frames;
extern int32               global_screenshake_frames;
extern ScreenShakeSeverity global_screenshake_severity;

// Debug Mode
#define DEBUG_BUILD false

// Functions

int32 getTileAtBGIndex(uint32 x, uint32 y, 
                       bn::regular_bg_ptr                      bg_ptr, 
                       bn::span<const bn::regular_bg_map_cell> cells,
                       bn::regular_bg_item                     bg_item);

#endif
