#include "phase_orb.h"

/////////////////////
// Struct PhaseOrb //
/////////////////////

PhaseOrb::PhaseOrb()
{
    // Init Collider - Child objects will init the rest
	collider          = Collider(x(), y(), PHASE_ORB_COLLIDER_WIDTH, PHASE_ORB_COLLIDER_HEIGHT);
	collider_x_axis   = collider;
	collider_y_axis   = collider;
	collider_offset_x = PHASE_ORB_COLLIDER_OFFSET_X;
	collider_offset_y = PHASE_ORB_COLLIDER_OFFSET_Y;

    phase_destination = bn::fixed_point(0, 0);
}

PhaseOrb::PhaseOrb(const PhaseOrb& other) : GameObject(other)
{
    phase_destination = other.phase_destination;
}

PhaseOrb::~PhaseOrb()
{

}

PhaseOrb& PhaseOrb::operator =(const PhaseOrb& other)
{
    phase_destination = other.phase_destination;
    
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

//////////////////////////////
// Struct PhaseOrb Children //
//////////////////////////////

PhaseOrbUp::PhaseOrbUp()
{

    // Reset Variables //
    sprite_ptr.reset();
    animate_action_ptr.reset();

    // Init Variables //
    object_type = PHASE_ORB_UP;
    sprite_ptr = bn::sprite_items::phase_orb_up.create_sprite(0, 0);
	sprite_ptr->set_z_order(PHASE_ORB_Z_ORDER);
    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  2,
								  								  bn::sprite_items::phase_orb_up.tiles_item(),
								  								  0,
								  								  0);

    phase_destination = bn::fixed_point(x(), y() - PHASE_DISTANCE);

}

PhaseOrbDown::PhaseOrbDown()
{

    // Reset Variables //
    sprite_ptr.reset();
    animate_action_ptr.reset();

    // Init Variables //
    object_type = PHASE_ORB_DOWN;
    sprite_ptr = bn::sprite_items::phase_orb_down.create_sprite(0, 0);
	sprite_ptr->set_z_order(PHASE_ORB_Z_ORDER);
    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  2,
								  								  bn::sprite_items::phase_orb_down.tiles_item(),
								  								  0,
								  								  0);

    phase_destination = bn::fixed_point(x(), y() + PHASE_DISTANCE);

}

PhaseOrbLeft::PhaseOrbLeft()
{

    // Reset Variables //
    sprite_ptr.reset();
    animate_action_ptr.reset();

    // Init Variables //
    object_type = PHASE_ORB_LEFT;
    sprite_ptr = bn::sprite_items::phase_orb_left.create_sprite(0, 0);
	sprite_ptr->set_z_order(PHASE_ORB_Z_ORDER);
    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  2,
								  								  bn::sprite_items::phase_orb_left.tiles_item(),
								  								  0,
								  								  0);

    phase_destination = bn::fixed_point(x() - PHASE_DISTANCE, y());

}

PhaseOrbRight::PhaseOrbRight()
{

    // Reset Variables //
    sprite_ptr.reset();
    animate_action_ptr.reset();

    // Init Variables //
    object_type = PHASE_ORB_RIGHT;
    sprite_ptr = bn::sprite_items::phase_orb_right.create_sprite(0, 0);
	sprite_ptr->set_z_order(PHASE_ORB_Z_ORDER);
    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  2,
								  								  bn::sprite_items::phase_orb_right.tiles_item(),
								  								  0,
								  								  0);

    phase_destination = bn::fixed_point(x(), y() + PHASE_DISTANCE);

}