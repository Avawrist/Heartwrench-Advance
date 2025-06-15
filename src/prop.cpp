#include "prop.h"

/////////////////
// Struct Prop //
/////////////////

Prop::Prop()
{
    state = NONE;

    hitpoints = PROP_HITPOINTS;
    damage    = PROP_DAMAGE;
}

Prop::Prop(const Prop& other) : GameObject(other)
{

}

Prop::~Prop()
{

}

Prop& Prop::operator =(const Prop& other)
{
    return *this;
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

void Prop::update(const RoomBounds& 							   room_bounds,
                    bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                    const bn::regular_bg_ptr&                      bg_ptr, 
                    const bn::span<const bn::regular_bg_map_cell>& cells,
                    const bn::regular_bg_item&                     bg_item,
                    const bn::camera_ptr&                          camera)
{
    //////////////////////////////
    // Monitor unloading bounds //
    //////////////////////////////
    
	updateInactiveState(camera);
}

//////////////////////////////
// State Function Overrides //
//////////////////////////////

// None..

/////////////////////////
// Collision Overrides //
/////////////////////////

// None..
