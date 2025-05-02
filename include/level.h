#ifndef LEVEL_H
#define LEVEL_H

// Butano
#include "bn_log.h"
#include "bn_core.h"
#include "bn_random.h"
#include "bn_math.h"
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
#include "bn_regular_bg_items_test_painted_bg.h"
#include "bn_regular_bg_items_test_object_bg.h"

// My Libs
#include "room.h"
#include "utility.h"
#include "player.h"

//////////////////
// Struct Level //
//////////////////

// 240 and 160 MUST be divisible by SCROLL_SPEED.
#define SCROLL_SPEED 8

#define MIN_X_SHAKE_RANGE -3
#define MAX_X_SHAKE_RANGE  3
#define MIN_Y_SHAKE_RANGE -3
#define MAX_Y_SHAKE_RANGE  3

// BG Z orders (Higher orders are drawn first)
#define PAINTED_BG_ORDER 4
#define BACKDROP_ORDER   3
#define MAIN_BG_ORDER    2
#define OBJECT_BG_ORDER  1

enum LevelName 
{
    NO_LEVEL = 0,
    LEVEL_TEST,
};

struct Level
{
    Room current_room;

    bn::optional<bn::camera_ptr>            camera;
    bn::optional<bn::regular_bg_ptr>        main_bg_ptr;
    bn::optional<bn::regular_bg_ptr>        backdrop_ptr;
    bn::optional<bn::regular_bg_ptr>        painted_bg_ptr;
    bn::optional<bn::regular_bg_ptr>        object_bg_ptr;
    
    bn::optional<bn::regular_bg_item>       bg_item;
    bn::span<const bn::regular_bg_map_cell> cells;
    
    bn::optional<bn::regular_bg_item>       object_bg_item;
    bn::span<const bn::regular_bg_map_cell> object_cells;

    uint32 tile_width;
    uint32 tile_height;

    LevelName current_level_name;

    bn::point player_spawn;

    bool  cam_is_scrolling;
    int32 cam_x_offset;
    int32 cam_y_offset;

    bn::random random_engine;

    Level();
    Level(LevelName level_name);
    Level(const Level& other);
    ~Level();

    void operator =(const Level& other);

    void load(LevelName level_name);
    void clear();
    void reload();

    void updateAndDraw();
    void updateCamera();
    void reloadOnDeath();
    void freeObjects();
    void updateIndexes();
    void transitionRoom();
};

#endif