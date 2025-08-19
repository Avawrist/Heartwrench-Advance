#ifndef FINISH_SEAL_H
#define FINISH_SEAL_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "game_object.h"

//////////////////////
// Struct FinishSeal //
//////////////////////

#define FINISH_SEAL_COLLIDER_WIDTH  64
#define FINISH_SEAL_COLLIDER_HEIGHT 64

#define FINISH_SEAL_COLLIDER_OFFSET_X 0
#define FINISH_SEAL_COLLIDER_OFFSET_Y 0

#define FINISH_SEAL_HITPOINTS 3

#define FINISH_SEAL_30_FPS true

struct FinishSeal : GameObject {

    //////////////////////
    // Struct FinishSeal //
    //////////////////////

    bool level_complete;

    FinishSeal();
    FinishSeal(const FinishSeal& other);
    ~FinishSeal();

    FinishSeal& operator =(const FinishSeal& other);

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