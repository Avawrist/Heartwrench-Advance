#include "candelabra.h"

//////////////////////
// Struct Candelabra //
//////////////////////

Candelabra::Candelabra()
{
        // Init Assets //
    object_type = CANDELABRA;
    sprite_ptr  = bn::sprite_items::candelabra.create_sprite(0, 0);
    sprite_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                   0,
								                                   bn::sprite_items::candelabra.tiles_item(),
								                                   0, 0);

    // Init Variables //
    collider_offset_x = CANDELABRA_COLLIDER_OFFSET_X;
    collider_offset_y = CANDELABRA_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        CANDELABRA_COLLIDER_WIDTH, 
                        CANDELABRA_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;

}

Candelabra::Candelabra(const Candelabra& other) : Prop(other)
{

}

Candelabra::~Candelabra()
{

}

Candelabra& Candelabra::operator =(const Candelabra& other)
{
    return *this;
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

// None..

//////////////////////////////
// State Function Overrides //
//////////////////////////////

// None..

/////////////////////////
// Collision Overrides //
/////////////////////////

// None..
