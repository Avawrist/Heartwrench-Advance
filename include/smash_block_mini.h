#ifndef SMASH_BLOCK_MINI_H
#define SMASH_BLOCK_MINI_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "game_object.h"

////////////////////////////
// Struct SmashBlockMini //
////////////////////////////

#define SMASH_BLOCK_MINI_COLLIDER_WIDTH  16
#define SMASH_BLOCK_MINI_COLLIDER_HEIGHT 16

#define SMASH_BLOCK_MINI_COLLIDER_OFFSET_X 0
#define SMASH_BLOCK_MINI_COLLIDER_OFFSET_Y 0

#define SMASH_BLOCK_MINI_HITPOINTS 1

#define SMASH_BLOCK_MINI_30_FPS true

#define SMASH_BLOCK_MINI_HIT_STOP_TRIGGER_FRAME 3
#define SMASH_BLOCK_MINI_HIT_STOP_FRAMES        2

struct SmashBlockMini : GameObject {

    ////////////////////////////
    // Struct SmashBlockMini //
    ////////////////////////////

    SmashBlockMini();
    SmashBlockMini(const SmashBlockMini& other);
    ~SmashBlockMini();

    SmashBlockMini& operator =(const SmashBlockMini& other);

    //////////////////////////
    // GameObject Overrides //
    //////////////////////////

    void update(const RoomBounds& 							   room_bounds,
                bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                const bn::regular_bg_ptr&                      bg_ptr, 
                const bn::span<const bn::regular_bg_map_cell>& cells,
                const bn::regular_bg_item&                     bg_item,
                const bn::camera_ptr&                          camera) override;

    void updateDeathState() override;

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