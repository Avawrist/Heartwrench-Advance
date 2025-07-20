#ifndef HP_TOTEM_H
#define HP_TOTEM_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "game_object.h"

////////////////////
// Struct HPTotem //
////////////////////

#define HP_TOTEM_COLLIDER_WIDTH  32
#define HP_TOTEM_COLLIDER_HEIGHT 32

#define HP_TOTEM_COLLIDER_OFFSET_X 0
#define HP_TOTEM_COLLIDER_OFFSET_Y 0

#define HP_TOTEM_HITPOINTS 1

#define HP_TOTEM_30_FPS true

struct HPTotem : GameObject {

    ////////////////////
    // Struct HPTotem //
    ////////////////////

    HPTotem();
    HPTotem(const HPTotem& other);
    ~HPTotem();

    HPTotem& operator =(const HPTotem& other);

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