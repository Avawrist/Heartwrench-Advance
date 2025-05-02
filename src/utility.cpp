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

int32 hitstop_frames                     = 0;
int32 screenshake_frames                 = 0;
ScreenShakeSeverity screenshake_severity = WEAK;

// Functions

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