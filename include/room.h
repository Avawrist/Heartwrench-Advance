#ifndef ROOM_H
#define ROOM_H

// Butano
#include "bn_log.h"
#include "bn_core.h"

#include "bn_vector.h"
#include "bn_camera_ptr.h"

// My Libs
#include "room_bounds.h"
#include "utility.h"
#include "game_object.h"
#include "player.h"
#include "devil_platform.h"
#include "angel_platform.h"
#include "test_enemy.h"

/////////////////
// Struct Room //
/////////////////

enum RoomName 
{
    NO_ROOM = 0,
    ROOM_TEST_1,
    ROOM_TEST_2,
};

struct Room
{

    bn::vector<GameObject*, MAX_GAME_OBJECTS> game_objects;

    RoomBounds room_bounds;

    RoomName top_neighbor;
    RoomName right_neighbor;
    RoomName bottom_neighbor;
    RoomName left_neighbor;

    Room(RoomName room_name, bn::camera_ptr camera_ptr);
    ~Room();

    int32  addObject(GameObject* object_ptr, bn::camera_ptr camera_ptr);
    void   clear();
    void   load(RoomName room_name, bn::camera_ptr camera_ptr);
    
};

#endif