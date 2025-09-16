#ifndef SMASH_BLOCK_ZIGGURAT_R_H
#define SMASH_BLOCK_ZIGGURAT_R_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "game_object.h"

// Assets
#include "bn_sprite_items_smash_block_ziggurat_r.h"

////////////////////////////////
// Struct SmashBlockZigguratR //
////////////////////////////////

#define SMASH_BLOCK_ZIGGURAT_R_COLLIDER_WIDTH  32
#define SMASH_BLOCK_ZIGGURAT_R_COLLIDER_HEIGHT 32

#define SMASH_BLOCK_ZIGGURAT_R_COLLIDER_OFFSET_X 0
#define SMASH_BLOCK_ZIGGURAT_R_COLLIDER_OFFSET_Y 0

#define SMASH_BLOCK_ZIGGURAT_R_HITPOINTS 1

#define SMASH_BLOCK_ZIGGURAT_R_30_FPS true

#define SMASH_BLOCK_ZIGGURAT_R_HIT_STOP_TRIGGER_FRAME 3
#define SMASH_BLOCK_ZIGGURAT_R_HIT_STOP_FRAMES        2

struct SmashBlockZigguratR : GameObject {

    ////////////////////////////////
    // Struct SmashBlockZigguratR //
    ////////////////////////////////

    SmashBlockZigguratR();
    SmashBlockZigguratR(const SmashBlockZigguratR& other);
    ~SmashBlockZigguratR();

    SmashBlockZigguratR& operator =(const SmashBlockZigguratR& other);

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