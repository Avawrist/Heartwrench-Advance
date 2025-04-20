#include "phase_orb.h"

PhaseOrb::PhaseOrb()
{

    // Reset Variables //
    sprite_ptr.reset();
    animate_action_ptr.reset();

    // Init Variables //
    object_type = PHASE_ORB;
    x_dir = 0;
    y_dir = 0;
    sprite_ptr = bn::sprite_items::phase_orb.create_sprite(0, 0);
	sprite_ptr->set_z_order(PHASE_ORB_Z_ORDER);
    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  2,
								  								  bn::sprite_items::phase_orb.tiles_item(),
								  								  0,
								  								  0);

	collider          = Collider(x(), y(), PHASE_ORB_COLLIDER_WIDTH, PHASE_ORB_COLLIDER_HEIGHT);
	collider_x_axis   = collider;
	collider_y_axis   = collider;
	collider_offset_x = PHASE_ORB_COLLIDER_OFFSET_X;
	collider_offset_y = PHASE_ORB_COLLIDER_OFFSET_Y;

}

PhaseOrb::PhaseOrb(const PhaseOrb& other) : GameObject(other)
{
    x_dir = other.x_dir;
    y_dir = other.y_dir;
}

PhaseOrb::~PhaseOrb()
{

}

PhaseOrb& PhaseOrb::operator =(const PhaseOrb& other)
{
    x_dir = other.x_dir;
    y_dir = other.y_dir;

    return *this;
}

void PhaseOrb::update(const RoomBounds&                              room_bounds,
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