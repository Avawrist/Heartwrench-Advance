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

// Level Objects
#include "tile_passage.h"
#include "phase_orb.h"
#include "falling_platform_wide.h"
#include "falling_platform_thin.h"
#include "push_block.h"
#include "push_block_mini.h"
#include "auto_platform.h"
#include "smash_block_large.h"
#include "smash_block_mini.h"
#include "smash_block_ziggurat_l.h"
#include "smash_block_ziggurat_c.h"
#include "smash_block_ziggurat_r.h"
#include "large_vase.h"
#include "small_vase.h"
#include "hp_totem.h"
#include "hp_drop.h"
#include "star_jar.h"
#include "skull_drop.h"
#include "gear_drop.h"
#include "screw.h"
#include "checkpoint.h"
#include "wrenchpoint.h"
#include "jumppoint.h"
#include "healthpoint.h"
#include "sealed_gate.h"
#include "sub_area_gate.h"

#include "bounce_bell.h"
#include "auto_bounce_bell.h"

// Enemies
#include "thorn_column.h"
#include "thorn_bar.h"
#include "ground_ghoul.h"
#include "bell_troll.h"
#include "winged_troll_l.h"
#include "winged_troll_r.h"

// Props
#include "candelabra.h"

// Special Objects
#include "player.h"

/////////////////
// Struct Room //
/////////////////

struct Room
{
    bn::vector<GameObject*,    MAX_GAME_OBJECTS>     game_objects;
    bn::vector<UnloadedObject, MAX_UNLOADED_OBJECTS> unloaded_objects;
    
    RoomBounds room_bounds;

    RoomName room_name;
    RoomName top_neighbor;
    RoomName right_neighbor;
    RoomName bottom_neighbor;
    RoomName left_neighbor;

    bool first_frame = true;

    Room();
    Room(RoomName                                       _room_name, 
         bn::camera_ptr                                 camera_ptr,                
         const bn::regular_bg_ptr&                      object_bg_ptr, 
         const bn::regular_bg_item&                     object_bg_item,
         uint8**                                        object_cells,
         const bn::fixed_point                          player_spawn);
    Room(const Room& other);
    ~Room();

    void operator =(const Room& other);

    int32 addObject(ObjectRequest& object_request, const bn::camera_ptr& camera_ptr);
    int32 addObject(const UnloadedObject& object, const bn::camera_ptr& camera_ptr);
    int32 addUnloadedObject(const UnloadedObject& new_object); // This will be called when the room is loaded.
    int32 findUnloadedObjectIndex(int32 object_id);
    void clear();
    void load(RoomName                                        _room_name, 
               const bn::camera_ptr&                          camera_ptr, 
               const bn::regular_bg_ptr&                      object_bg_ptr, 
               const bn::regular_bg_item&                     object_bg_item,
               uint8**                                        object_cells,
               const bn::fixed_point                          player_spawn);
    void prepObjects(const bn::regular_bg_ptr&  object_bg_ptr, 
                     const bn::regular_bg_item& object_bg_item,
                     uint8**                    object_cells);
    void monitorObjectRequests(const bn::camera_ptr& camera_ptr);
    void monitorUnloadedObjects(const bn::camera_ptr& camera_ptr); // This will be called every frame to 
                                                                    // test for objects that should be loaded.
    void updateIndexes();
    bn::point center();

    void setStarCollected();
    bool starIsCollected();
};

#endif