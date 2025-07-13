#include "new_object_template.h"
/*
//////////////////////
// Struct NewObject //
//////////////////////

NewObject::NewObject()
{
    // Init Assets //
    object_type = NEW_OBJECT;
    sprite_ptr  = bn::sprite_items::new_object.create_sprite(0, 0);
    sprite_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                   0,
								                                   bn::sprite_items::new_object.tiles_item(),
								                                   0, 0);

    // Init Variables //
    collider_offset_x = NEW_OBJECT_COLLIDER_OFFSET_X;
    collider_offset_y = NEW_OBJECT_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        NEW_OBJECT_COLLIDER_WIDTH, 
                        NEW_OBJECT_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;

    hitpoints = NEW_OBJECT_HITPOINTS;

    thirty_fps = NEW_OBJECT_30_FPS;
}

NewObject::NewObject(const NewObject& other) : GameObject(other)
{

}

NewObject::~NewObject()
{

}

NewObject& NewObject::operator =(const NewObject& other)
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
*/