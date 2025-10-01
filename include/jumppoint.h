#ifndef JUMPPOINT_H
#define JUMPPOINT_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "game_object.h"

// Assets
#include "bn_sprite_items_jumppoint.h"

////////////////////////
// Struct Jumppoint //
////////////////////////

#define JUMPPOINT_COLLIDER_WIDTH  32
#define JUMPPOINT_COLLIDER_HEIGHT 32

#define JUMPPOINT_COLLIDER_OFFSET_X 0
#define JUMPPOINT_COLLIDER_OFFSET_Y 0

#define JUMPPOINT_HITPOINTS 0

#define JUMPPOINT_30_FPS true

#define JUMPPOINT_COST 15

struct Jumppoint : GameObject {

    //////////////////////
    // Struct Jumppoint //
    //////////////////////

    Jumppoint();
    Jumppoint(const Jumppoint& other);
    ~Jumppoint();

    Jumppoint& operator =(const Jumppoint& other);

    void setCollectingAnimation();
    void setCollectedAnimation();

    uint32 cost;

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

    void updateHitstunState() override;

    /////////////////////////
    // Collision Overrides //
    /////////////////////////

    // None..

};

#endif