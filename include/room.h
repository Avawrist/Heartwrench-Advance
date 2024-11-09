#ifndef ROOM_H
#define ROOM_H

// Butano
#include "bn_log.h"
#include "bn_core.h"

#include "bn_regular_bg_ptr.h"
#include "bn_span.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_map_cell.h"
#include "bn_regular_bg_map_cell_info.h"

// Assets
#include "bn_regular_bg_items_test_bg.h"
#include "bn_regular_bg_items_test_room.h"

// My Libs
#include "utility.h"
#include "game_object.h"
#include "player.h"
#include "block.h"

#define BLOCK_INDEX 1
#define BLOCK_WIDTH 8

enum RoomName 
{
    ROOM_TEST = 0,
};

struct Room
{
    bn::optional<bn::regular_bg_ptr> bg_ptr;
    bn::optional<bn::regular_bg_ptr> backdrop_ptr;
    uint32 width;
    uint32 height;

    Room(RoomName room_name, const camera_ptr& camera_ptr, bn::vector<GameObject*>& game_objects);

};

#endif