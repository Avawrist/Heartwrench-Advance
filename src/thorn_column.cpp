#include "thorn_column.h"

////////////////////////
// Struct ThornColumn //
////////////////////////

ThornColumn::ThornColumn()
{
    // Init Assets //
    object_type = THORN_COLUMN;
    sprite_ptr  = bn::sprite_items::thorn_column.create_sprite(0, 0);
    sprite_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                   4,
								                                   bn::sprite_items::thorn_column.tiles_item(),
								                                   0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 
                                                                   4, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1);

    // Init Variables //
    collider_offset_x = THORN_COLUMN_COLLIDER_OFFSET_X;
    collider_offset_y = THORN_COLUMN_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        THORN_COLUMN_COLLIDER_WIDTH, 
                        THORN_COLUMN_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;

    hitpoints = THORN_COLUMN_HITPOINTS;
}

ThornColumn::ThornColumn(const ThornColumn& other) : Enemy(other)
{

}

ThornColumn::~ThornColumn()
{

}

ThornColumn& ThornColumn::operator =(const ThornColumn& other)
{
    return *this;
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

void ThornColumn::updatePhysics()
{

}

//////////////////////////////
// State Function Overrides //
//////////////////////////////

// None..

/////////////////////////
// Collision Overrides //
/////////////////////////

void ThornColumn::resolveCollision(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                                   const bn::regular_bg_ptr&                      bg_ptr, 
                                   const bn::span<const bn::regular_bg_map_cell>& cells,
                                   const bn::regular_bg_item&                     bg_item)
{
    
}