#ifndef LEVEL_H
#define LEVEL_H

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
#include "bn_regular_bg_items_test_level.h"

// My Libs
#include "room.h"
#include "utility.h"
#include "player.h"

//////////////////
// Struct Level //
//////////////////

// MUST divide screen dimensions 240/160 cleanly.
#define SCROLL_SPEED 4

enum LevelName 
{
    NO_LEVEL = 0,
    LEVEL_TEST,
};

struct Level
{
    Room* current_room_ptr;

    bn::optional<bn::camera_ptr>            camera;
    bn::optional<bn::regular_bg_ptr>        bg_ptr;
    bn::optional<bn::regular_bg_ptr>        backdrop_ptr;
    bn::optional<bn::regular_bg_item>       bg_item;
    bn::span<const bn::regular_bg_map_cell> cells;
    uint32 tile_width;
    uint32 tile_height;

    LevelName current_level;

    bn::point player_spawn;

    bool cam_is_scrolling;
    int32 cam_x_offset;
    int32 cam_y_offset;

    Level(LevelName level_name);
    ~Level();

    void load(LevelName level_name);
    void clear();
    void reload();

    void updateAndDraw();
    void updateCamera();
    void reloadOnDeath();
    void freeInactiveObjects();
    void updateIndexes();
    void transitionRoom();

};

#endif