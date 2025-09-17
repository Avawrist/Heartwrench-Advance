#ifndef SCREW_H
#define SCREW_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "game_object.h"

// Assets
#include "bn_sprite_items_screw.h"

//////////////////
// Struct Screw //
//////////////////

#define SCREW_COLLIDER_WIDTH  24
#define SCREW_COLLIDER_HEIGHT 1

#define SCREW_COLLIDER_OFFSET_X 0
#define SCREW_COLLIDER_OFFSET_Y -1

#define SCREW_HITPOINTS 1

#define SCREW_30_FPS false

#define SCREW_STEP 2
#define SCREW_MIN_Y_OFFSET -1
#define SCREW_MAX_Y_OFFSET 15

#define SCREW_STOP_FRAME     0
#define SCREW_HITSTOP_FRAMES 6

struct Screw : GameObject {

    //////////////////
    // Struct Screw //
    //////////////////

    Screw();
    Screw(const Screw& other);
    ~Screw();

    Screw& operator =(const Screw& other);

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