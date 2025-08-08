#ifndef CHECKPOINT_H
#define CHECKPOINT_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "game_object.h"

//////////////////////
// Struct Checkpoint //
//////////////////////

#define CHECKPOINT_COLLIDER_WIDTH  32
#define CHECKPOINT_COLLIDER_HEIGHT 32

#define CHECKPOINT_COLLIDER_OFFSET_X 0
#define CHECKPOINT_COLLIDER_OFFSET_Y 0

#define CHECKPOINT_HITPOINTS 0

#define CHECKPOINT_30_FPS true

#define CHECKPOINT_COST 15

struct Checkpoint : GameObject {

    //////////////////////
    // Struct Checkpoint //
    //////////////////////

    Checkpoint();
    Checkpoint(const Checkpoint& other);
    ~Checkpoint();

    Checkpoint& operator =(const Checkpoint& other);

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

    /////////////////////////
    // Collision Overrides //
    /////////////////////////

    // None..

};

#endif