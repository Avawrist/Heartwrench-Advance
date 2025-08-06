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
#define GLOBAL_TIMER_MAX 600

#define MAX_GAME_OBJECTS     15
#define MAX_UNLOADED_OBJECTS 25

#define MAX_SPRITE_TILES 1350

#define SCREEN_W 240
#define SCREEN_H 160
#define SCREEN_LOAD_PADDING 64
#define LOAD_RANGE_W SCREEN_W + (4 * SCREEN_LOAD_PADDING)
#define LOAD_RANGE_H SCREEN_H + (4 * SCREEN_LOAD_PADDING)

#define CAM_PLAYER_Y_OFFSET -16
#define CAM_MAX_LOOK_Y       16
#define CAM_MAX_LOOK_X       0
#define CAM_MAX_DIR_LOOK_X   32

#define LEVEL_WIDTH  5120
#define LEVEL_HEIGHT 2560
#define LEVEL_OBJECT_CELL_WIDTH  160
#define LEVEL_OBJECT_CELL_HEIGHT 80

#define ROOM_MAX_COLLIDERS 1800
#define ROOM_MAX_WIDTH     128
#define ROOM_MAX_HEIGHT    64

// === Object Indexes === //
#define PLAYER_OBJECT_LIST_INDEX 0

// === Tile Indexes === //
#define NO_BLOCK_INDEX       0
#define HARD_BLOCK_MIN_INDEX 1
#define HARD_BLOCK_MAX_INDEX 90

#define H_GEAR_LEFT    98
#define H_GEAR_MID_MIN 99
#define H_GEAR_MID_MAX 101
#define H_GEAR_RIGHT   102

#define V_GEAR_TOP    103
#define V_GEAR_MID_1  120
#define V_GEAR_MID_2  105
#define V_GEAR_MID_3  107
#define V_GEAR_BOTTOM 122

#define SPIKE_MIN_INDEX 124
#define SPIKE_MAX_INDEX 136

#define CLIMBABLE_MIN_INDEX 137
#define CLIMBABLE_MAX_INDEX 152

#define ONEWAY_BLOCK_MIN_INDEX 153
#define ONEWAY_BLOCK_MAX_INDEX 156

#define SHALLOW_SLOPE_1_INDEX  157
#define SHALLOW_SLOPE_2_INDEX  158
#define SHALLOW_SLOPE_3_INDEX  159
#define SHALLOW_SLOPE_4_INDEX  160

#define STEEP_SLOPE_1_INDEX 161
#define STEEP_SLOPE_2_INDEX 162

#define TILE_TOTAL_COUNT 255

// Tile Dimensions //
#define TILE_WIDTH  8
#define TILE_HEIGHT 8

#define WORLD_X_OFFSET 4
#define WORLD_Y_OFFSET 4

// Collider Dimensions //
#define ONEWAYBLOCK_COLLIDER_WIDTH     8
#define ONEWAYBLOCK_COLLIDER_HEIGHT    1
#define ONEWAYBLOCK_COLLIDER_Y_OFFSET -4

// HUD
#define HUD_FLASH_FRAMES 8

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

extern int32 global_hud_hp_flash_frames;
extern int32 global_hud_currency_flash_frames;

extern int32 global_timer;

// Debug Mode
#define DEBUG_BUILD false

// Functions

int32 getTileAtBGIndex(uint32 x, uint32 y, 
                       bn::regular_bg_ptr                      bg_ptr, 
                       bn::span<const bn::regular_bg_map_cell> cells,
                       bn::regular_bg_item                     bg_item);

uint8 getDynamicTileAtBGIndex(uint32 x, uint32 y, 
                              bn::regular_bg_ptr  bg_ptr, 
                              uint8               cells[LEVEL_OBJECT_CELL_WIDTH][LEVEL_OBJECT_CELL_HEIGHT],
                              bn::regular_bg_item bg_item);

#endif
