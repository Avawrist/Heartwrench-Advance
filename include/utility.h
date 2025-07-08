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

#define MAX_SPRITE_TILES 1400

#define SCREEN_W 240
#define SCREEN_H 160
#define SCREEN_LOAD_PADDING 64
#define LOAD_RANGE_W SCREEN_W + (2 * SCREEN_LOAD_PADDING)
#define LOAD_RANGE_H SCREEN_H + (2 * SCREEN_LOAD_PADDING)

#define CAM_PLAYER_Y_OFFSET -24
#define CAM_MAX_LOOK_Y       16
#define CAM_MAX_LOOK_X       16
#define CAM_MAX_DIR_LOOK_X   16

#define ROOM_MAX_COLLIDERS 1800
#define ROOM_MAX_WIDTH     128
#define ROOM_MAX_HEIGHT    64

// === Object Indexes === //
#define PLAYER_OBJECT_LIST_INDEX 0

// === Tile Indexes === //
#define NO_BLOCK_INDEX              0
#define HARD_BLOCK_MIN_INDEX        1
#define HARD_BLOCK_MAX_INDEX        153

#define H_GEAR_LEFT    160
#define H_GEAR_MID_MIN 161
#define H_GEAR_MID_MAX 163
#define H_GEAR_RIGHT   164

#define V_GEAR_TOP    165
#define V_GEAR_MID_1  177
#define V_GEAR_MID_2  167
#define V_GEAR_MID_3  168
#define V_GEAR_BOTTOM 166

#define SPIKE_MIN_INDEX 179
#define SPIKE_MAX_INDEX 182

#define ONEWAY_BLOCK_MIN_INDEX 183
#define ONEWAY_BLOCK_MAX_INDEX 185

#define SHALLOW_SLOPE_1_INDEX  186
#define SHALLOW_SLOPE_2_INDEX  187
#define SHALLOW_SLOPE_3_INDEX  188
#define SHALLOW_SLOPE_4_INDEX  189

#define STEEP_SLOPE_1_INDEX 190
#define STEEP_SLOPE_2_INDEX 191

#define TILE_TOTAL_COUNT 191

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
extern int32               global_tiles_in_VRAM;
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
