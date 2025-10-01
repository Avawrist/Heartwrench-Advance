#ifndef WRENCHPOINT_H
#define WRENCHPOINT_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "game_object.h"

// Assets
#include "bn_sprite_items_wrenchpoint.h"

////////////////////////
// Struct Wrenchpoint //
////////////////////////

#define WRENCHPOINT_COLLIDER_WIDTH  32
#define WRENCHPOINT_COLLIDER_HEIGHT 32

#define WRENCHPOINT_COLLIDER_OFFSET_X 0
#define WRENCHPOINT_COLLIDER_OFFSET_Y 0

#define WRENCHPOINT_HITPOINTS 0

#define WRENCHPOINT_30_FPS true

#define WRENCHPOINT_COST 10

struct Wrenchpoint : GameObject {

    //////////////////////
    // Struct Wrenchpoint //
    //////////////////////

    Wrenchpoint();
    Wrenchpoint(const Wrenchpoint& other);
    ~Wrenchpoint();

    Wrenchpoint& operator =(const Wrenchpoint& other);

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