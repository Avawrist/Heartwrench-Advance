#ifndef SMALL_VASE_H
#define SMALL_VASE_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "game_object.h"

//////////////////////
// Struct SmallVase //
//////////////////////

#define SMALL_VASE_COLLIDER_WIDTH  32
#define SMALL_VASE_COLLIDER_HEIGHT 32

#define SMALL_VASE_COLLIDER_OFFSET_X 0
#define SMALL_VASE_COLLIDER_OFFSET_Y 0

#define SMALL_VASE_HITPOINTS 1

struct SmallVase : GameObject {

    //////////////////////
    // Struct SmallVase //
    //////////////////////

    SmallVase();
    SmallVase(const SmallVase& other);
    ~SmallVase();

    SmallVase& operator =(const SmallVase& other);

    //////////////////////////
    // GameObject Overrides //
    //////////////////////////

    void update(const RoomBounds& 							   room_bounds,
                bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                const bn::regular_bg_ptr&                      bg_ptr, 
                const bn::span<const bn::regular_bg_map_cell>& cells,
                const bn::regular_bg_item&                     bg_item,
                const bn::camera_ptr&                          camera) override;

    //////////////////////////////
    // State Function Overrides //
    //////////////////////////////

	void updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                            const bn::regular_bg_ptr&                      bg_ptr, 
                            const bn::span<const bn::regular_bg_map_cell>& cells,
                            const bn::regular_bg_item&                     bg_item,
                            const bn::camera_ptr&                          camera) override;

    void setState(ObjectState new_state) override;

    /////////////////////////
    // Collision Overrides //
    /////////////////////////

    // None..

};

#endif