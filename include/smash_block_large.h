#ifndef SMASH_BLOCK_LARGE_H
#define SMASH_BLOCK_LARGE_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "game_object.h"

// Assets
#include "bn_sprite_items_smash_block_large.h"

////////////////////////////
// Struct SmashBlockLarge //
////////////////////////////

#define SMASH_BLOCK_LARGE_COLLIDER_WIDTH  32
#define SMASH_BLOCK_LARGE_COLLIDER_HEIGHT 32

#define SMASH_BLOCK_LARGE_COLLIDER_OFFSET_X 0
#define SMASH_BLOCK_LARGE_COLLIDER_OFFSET_Y 0

#define SMASH_BLOCK_LARGE_HITPOINTS 1

#define SMASH_BLOCK_LARGE_30_FPS true

struct SmashBlockLarge : GameObject {

    ////////////////////////////
    // Struct SmashBlockLarge //
    ////////////////////////////

    SmashBlockLarge();
    SmashBlockLarge(const SmashBlockLarge& other);
    ~SmashBlockLarge();

    SmashBlockLarge& operator =(const SmashBlockLarge& other);

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