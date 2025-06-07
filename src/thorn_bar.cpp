#include "thorn_bar.h"

ThornBar::ThornBar()
{
    // Init Assets //
    object_type = THORN_BAR;
    sprite_ptr  = bn::sprite_items::thorn_bar.create_sprite(0, 0);
    sprite_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                   4,
								                                   bn::sprite_items::thorn_bar.tiles_item(),
								                                   0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 
                                                                   4, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1);

    // Init Variables //
    collider_offset_x = THORN_BAR_COLLIDER_OFFSET_X;
    collider_offset_y = THORN_BAR_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        THORN_BAR_COLLIDER_WIDTH, 
                        THORN_BAR_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;

    hitpoints = THORN_BAR_HITPOINTS;
}

ThornBar::ThornBar(const ThornBar& other) : Enemy(other)
{

}

ThornBar::~ThornBar()
{

}

ThornBar& ThornBar::operator =(const ThornBar& other)
{
    return *this;
}

void ThornBar::update(const RoomBounds&                                 room_bounds,
                      bn::vector<GameObject*, MAX_GAME_OBJECTS>&        game_objects,
                      const bn::regular_bg_ptr&                         bg_ptr, 
                      const bn::span<const bn::regular_bg_map_cell>&    cells,
                      const bn::regular_bg_item&                        bg_item,
                      const bn::camera_ptr&                             camera)
{
    /////////////////////////////////
	// Generic Object Update stuff //
	/////////////////////////////////
	
	GameObject::update(room_bounds,
                       game_objects,
                       bg_ptr,
                       cells,
                       bg_item,
                       camera);
}