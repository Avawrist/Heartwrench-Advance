#ifndef OPEN_GATE_H
#define OPEN_GATE_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "game_object.h"

// Assets
#include "bn_sprite_items_open_gate.h"

/////////////////////
// Struct OpenGate //
/////////////////////

#define OPEN_GATE_COLLIDER_WIDTH  44
#define OPEN_GATE_COLLIDER_HEIGHT 64

#define OPEN_GATE_COLLIDER_OFFSET_X 0
#define OPEN_GATE_COLLIDER_OFFSET_Y 0

#define OPEN_GATE_HITPOINTS 1

#define OPEN_GATE_30_FPS false

struct OpenGate : GameObject {

    ////////////////////////
    // Struct OpenGate //
    ////////////////////////

    OpenGate();
    OpenGate(const OpenGate& other);
    ~OpenGate();

    OpenGate& operator =(const OpenGate& other);

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