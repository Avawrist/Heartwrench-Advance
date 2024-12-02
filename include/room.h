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
#include "player.h"
#include "devil_platform.h"
#include "angel_platform.h"
#include "scythe_platform.h"

enum RoomName 
{
    ROOM_TEST = 0,
    ROOM_TEST_2,
};

struct Room
{
    bn::vector<GameObject*, MAX_GAME_OBJECTS> game_objects;

    bn::optional<bn::regular_bg_ptr>        bg_ptr;
    bn::optional<bn::regular_bg_ptr>        backdrop_ptr;
    bn::optional<bn::regular_bg_item>       bg_item;
    bn::span<const bn::regular_bg_map_cell> cells;
    uint32 tile_width;
    uint32 tile_height;

    bn::point player_spawn;

    Room(RoomName room_name, 
         const bn::camera_ptr& camera);
    ~Room();

    void clear();
    void load(RoomName room_name, 
              const bn::camera_ptr& camera);

};

#endif