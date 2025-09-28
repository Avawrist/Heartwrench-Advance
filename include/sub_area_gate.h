#ifndef SUB_AREA_GATE_H
#define SUB_AREA_GATE_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "game_object.h"

// Assets
#include "bn_sprite_items_sub_area_gate.h"

////////////////////////
// Struct SubAreaGate //
////////////////////////

#define SUB_AREA_GATE_COLLIDER_WIDTH  44
#define SUB_AREA_GATE_COLLIDER_HEIGHT 64

#define SUB_AREA_GATE_COLLIDER_OFFSET_X -24
#define SUB_AREA_GATE_COLLIDER_OFFSET_Y 0

#define SUB_AREA_GATE_COLLIDER_OPEN_OFFSET_X 0
#define SUB_AREA_GATE_COLLIDER_OPEN_OFFSET_Y 0

#define SUB_AREA_GATE_HITPOINTS 1

#define SUB_AREA_GATE_30_FPS false

#define SUB_AREA_GATE_MAX_FRAMES 4

struct SubAreaGate : GameObject {

    ////////////////////////
    // Struct SubAreaGate //
    ////////////////////////

    SubAreaGate();
    SubAreaGate(const SubAreaGate& other);
    ~SubAreaGate();

    SubAreaGate& operator =(const SubAreaGate& other);

    void setOpen1Anim();
    void setOpen2Anim();
    void setOpen3Anim();

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