#ifndef HEALTHPOINT_H
#define HEALTHPOINT_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "game_object.h"

// Assets
#include "bn_sprite_items_healthpoint.h"

////////////////////////
// Struct Healthpoint //
////////////////////////

#define HEALTHPOINT_COLLIDER_WIDTH  32
#define HEALTHPOINT_COLLIDER_HEIGHT 32

#define HEALTHPOINT_COLLIDER_OFFSET_X 0
#define HEALTHPOINT_COLLIDER_OFFSET_Y 0

#define HEALTHPOINT_HITPOINTS 0

#define HEALTHPOINT_30_FPS true

#define HEALTHPOINT_COST 20

#define HEALTHPOINT_RESTORE_POINTS 6

struct Healthpoint : GameObject {

    //////////////////////
    // Struct Healthpoint //
    //////////////////////

    Healthpoint();
    Healthpoint(const Healthpoint& other);
    ~Healthpoint();

    Healthpoint& operator =(const Healthpoint& other);

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