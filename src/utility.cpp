#include "utility.h"

int32 left_shallow_slope_1_arr[TILE_WIDTH] = {1, 1, 1, 1, 2, 2, 2, 2};
int32 left_shallow_slope_2_arr[TILE_WIDTH] = {3, 3, 3, 3, 4, 4, 4, 4};
int32 left_shallow_slope_3_arr[TILE_WIDTH] = {5, 5, 5, 5, 6, 6, 6, 6};
int32 left_shallow_slope_4_arr[TILE_WIDTH] = {7, 7, 7, 7, 8, 8, 8, 8};

int32 left_steep_slope_1_arr[TILE_WIDTH] = {1, 1, 2, 2, 3, 3, 4, 4};
int32 left_steep_slope_2_arr[TILE_WIDTH] = {5, 5, 6, 6, 7, 7, 8, 8};

int32 right_shallow_slope_1_arr[TILE_WIDTH] = {2, 2, 2, 2, 1, 1, 1, 1};
int32 right_shallow_slope_2_arr[TILE_WIDTH] = {4, 4, 4, 4, 3, 3, 3, 3};
int32 right_shallow_slope_3_arr[TILE_WIDTH] = {6, 6, 6, 6, 5, 5, 5, 5};
int32 right_shallow_slope_4_arr[TILE_WIDTH] = {8, 8, 8, 8, 7, 7, 7, 7};

int32 right_steep_slope_1_arr[TILE_WIDTH] = {4, 4, 3, 3, 2, 2, 1, 1};
int32 right_steep_slope_2_arr[TILE_WIDTH] = {8, 8, 7, 7, 6, 6, 5, 5};

int32 global_tiles_in_VRAM                      = 0;
int32 global_bg_hitflash_frames                 = 0;
int32 global_hitstop_frames                     = 0;
int32 global_screenshake_frames                 = 0;
ScreenShakeSeverity global_screenshake_severity = NO_SHAKE;

uint32 global_level_currency = 0;
uint32 global_world_currency = 0;

int32 global_hud_hp_flash_frames       = 0;
int32 global_hud_currency_flash_frames = 0;

int32 global_timer = 0;

bool global_bell_struck = false;

int32 global_ow_player_location_x = -24;
int32 global_ow_player_location_y = -8;

// Functions

bool tileIsSolid(uint32 tile_index)
{
    if(tile_index >= HARD_BLOCK_MIN_INDEX   && tile_index <= HARD_BLOCK_MAX_INDEX)   {return true;}
    if(tile_index >= SPIKE_MIN_INDEX        && tile_index <= SPIKE_MAX_INDEX)        {return true;}

    return false;
}

int32 getTileAtBGIndex(uint32 x, uint32 y, 
                       bn::regular_bg_ptr                      bg_ptr, 
                       bn::span<const bn::regular_bg_map_cell> cells,
                       bn::regular_bg_item                     bg_item)
{

    // Returns -1 if input range is invalid. 
    if(x > (uint32)((bg_ptr.dimensions().width() / 8) - 1) ||
    y > (uint32)((bg_ptr.dimensions().height() / 8) - 1))
    {return -1;}

    bn::regular_bg_map_cell       cell_index = cells[bg_item.map_item().cell_index(x, y)];
    bn::regular_bg_map_cell_info  cell_info(cell_index);    

    return cell_info.tile_index();

}

uint8 getDynamicTileAtBGIndex(uint32 x, uint32 y, uint8** cells)
{
    if(x >= LEVEL_OBJECT_CELL_WIDTH)  {return 0;}
    if(y >= LEVEL_OBJECT_CELL_HEIGHT) {return 0;}

    uint8 cell_tile_index = cells[x][y];

    return cell_tile_index;
}

void setDynamicTileAtBGIndex(uint32 x, uint32 y, uint8 value, uint8** cells)
{
    if(x >= LEVEL_OBJECT_CELL_WIDTH)  {return;}
    if(y >= LEVEL_OBJECT_CELL_HEIGHT) {return;}

    cells[x][y] = value;
}