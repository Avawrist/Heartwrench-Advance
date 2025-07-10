#ifndef LARGE_VASE_H
#define LARGE_VASE_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "game_object.h"

//////////////////////
// Struct LargeVase //
//////////////////////

#define LARGE_VASE_COLLIDER_WIDTH  32
#define LARGE_VASE_COLLIDER_HEIGHT 32

#define LARGE_VASE_COLLIDER_OFFSET_X 0
#define LARGE_VASE_COLLIDER_OFFSET_Y 0

#define LARGE_VASE_HITPOINTS 1

struct LargeVase : GameObject {

    //////////////////////
    // Struct LargeVase //
    //////////////////////

    LargeVase();
    LargeVase(const LargeVase& other);
    ~LargeVase();

    LargeVase& operator =(const LargeVase& other);

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