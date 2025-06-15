#ifndef PROP_H
#define PROP_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "game_object.h"

/////////////////
// Struct Prop //
/////////////////

#define PROP_HITPOINTS 0
#define PROP_DAMAGE    0

struct Prop : GameObject 
{

    /////////////////
    // Struct Prop //
    /////////////////

    Prop();
    Prop(const Prop& other);
    ~Prop();

    Prop& operator =(const Prop& other);

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

    // None..

    /////////////////////////
    // Collision Overrides //
    /////////////////////////

    // None..

};

#endif