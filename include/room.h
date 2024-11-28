#ifndef ROOM_H
#define ROOM_H

// Butano
#include "bn_log.h"
#include "bn_core.h"

#include "bn_vector.h"
#include "bn_camera_ptr.h"
#include "bn_span.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_map_cell.h"
#include "bn_regular_bg_map_cell_info.h"

// Assets
#include "bn_regular_bg_items_test_bg.h"
#include "bn_regular_bg_items_test_room.h"
#include "bn_regular_bg_items_test_room_2.h"

// My Libs
#include "utility.h"

#define BLOCK_INDEX       1
#define ONEWAYBLOCK_INDEX 2
#define SLOW_BLOCK_INDEX  3

#define TILE_WIDTH  8
#define TILE_HEIGHT 8

#define ONEWAYBLOCK_COLLIDER_WIDTH     8
#define ONEWAYBLOCK_COLLIDER_HEIGHT    1
#define ONEWAYBLOCK_COLLIDER_Y_OFFSET -4

enum RoomName 
{
    ROOM_TEST = 0,
    ROOM_TEST_2,
};

struct Room
{
    bn::optional<bn::regular_bg_ptr>        bg_ptr;
    bn::optional<bn::regular_bg_ptr>        backdrop_ptr;
    bn::optional<bn::regular_bg_item>       bg_item;
    bn::span<const bn::regular_bg_map_cell> cells;
    uint32 tile_width;
    uint32 tile_height;

    Room(RoomName room_name, 
         const bn::camera_ptr& camera_ptr);
    ~Room();
    int32 getTileAtIndex(uint32 x, uint32 y) const;
};

#endif