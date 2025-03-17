#ifndef ROOM_H
#define ROOM_H

// Butano
#include "bn_log.h"
#include "bn_core.h"

#include "bn_vector.h"
#include "bn_camera_ptr.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_map_cell.h"
#include "bn_regular_bg_map_cell_info.h"
#include "bn_memory.h"

// My Libs
#include "room_bounds.h"
#include "utility.h"
#include "collider.h"
#include "game_object.h"
#include "player.h"
#include "devil_platform.h"
#include "angel_platform.h"
#include "test_enemy.h"

///////////////////////////
// Struct UnloadedObject //
///////////////////////////

struct UnloadedObject
{
    bn::point  room_pos;
    ObjectType object_type;

    UnloadedObject();
    UnloadedObject(bn::point _room_pos, ObjectType _object_type);
    UnloadedObject(const UnloadedObject& other);
    ~UnloadedObject();

    void operator =(const UnloadedObject& other);
};

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

    bn::vector<GameObject*,    MAX_GAME_OBJECTS>     game_objects;
    bn::vector<UnloadedObject, MAX_UNLOADED_OBJECTS> unloaded_objects;
    
    RoomBounds room_bounds;

    RoomName top_neighbor;
    RoomName right_neighbor;
    RoomName bottom_neighbor;
    RoomName left_neighbor;

    Room();
    Room(RoomName room_name, bn::camera_ptr camera_ptr);
    Room(const Room& other);
    ~Room();

    void operator =(const Room& other);

    int32 addObject(GameObject* object_ptr, const bn::camera_ptr& camera_ptr);
    int32 addObject(const UnloadedObject& object, const bn::camera_ptr& camera_ptr);
    int32 addUnloadedObject(const UnloadedObject& new_object); // This will be called when the room is loaded.
    void  clear();
    void  load(RoomName room_name, const bn::camera_ptr& camera_ptr);
    void  monitorUnloadedObjects(const bn::camera_ptr& camera_ptr); // This will be called every frame to 
                                                                    // test for objects that should be loaded.
                                 
};

#endif