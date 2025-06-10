#include "falling_platform_wide.h"

////////////////////////////////
// Struct FallingPlatformWide //
////////////////////////////////

FallingPlatformWide::FallingPlatformWide()
{
    // Init Assets //
    state = FALLING_PLATFORM_WIDE_NEUTRAL;

    object_type = FALLING_PLATFORM_WIDE;
    sprite_ptr  = bn::sprite_items::falling_platform_wide.create_sprite(0, 0);
    sprite_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                   1,
								                                   bn::sprite_items::falling_platform_wide.tiles_item(),
								                                   0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4);

    // Init Variables //
    collider_offset_x = FALLING_PLATFORM_WIDE_COLLIDER_OFFSET_X;
    collider_offset_y = FALLING_PLATFORM_WIDE_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        FALLING_PLATFORM_WIDE_COLLIDER_WIDTH, 
                        FALLING_PLATFORM_WIDE_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;

    hitpoints = FALLING_PLATFORM_WIDE_HITPOINTS;

    fall_timer = 0;
}

FallingPlatformWide::FallingPlatformWide(const FallingPlatformWide& other) : GameObject(other)
{
    fall_timer = other.fall_timer;
}

FallingPlatformWide::~FallingPlatformWide()
{

}

FallingPlatformWide& FallingPlatformWide::operator =(const FallingPlatformWide& other)
{
    fall_timer = other.fall_timer;

    return *this;
}

//////////////////////////////
// State Function Overrides //
//////////////////////////////

void FallingPlatformWide::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                                             const bn::regular_bg_ptr&                      bg_ptr, 
                                             const bn::span<const bn::regular_bg_map_cell>& cells,
                                             const bn::regular_bg_item&                     bg_item,
                                             const bn::camera_ptr&                          camera)
{
    ///////////////////////////////////////
    // FallingPlatformWide State Machine //
    ///////////////////////////////////////
	
    switch(state)
    {
		case FALLING_PLATFORM_WIDE_NEUTRAL:
        break;

        case FALLING_PLATFORM_WIDE_FALLING:

            fall_timer--;
            fall_timer = clamp(0, FALLING_PLATFORM_WIDE_FALL_TIMER, fall_timer);

            if(fall_timer <= 0) {rigidbody.addForce(FALLING_PLATFORM_WIDE_GRAVITY_FORCE);}

        break;
		
		default:
		break;
    }
}

void FallingPlatformWide::setState(ObjectState new_state)
{
    // Set State
	state = new_state;

	// Set animations & other state specific variables
	switch(new_state)
	{
		
		case NONE:
		break;

		case FALLING_PLATFORM_WIDE_NEUTRAL:

            fall_timer = 0;

            animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                        1,
                                                                        bn::sprite_items::falling_platform_wide.tiles_item(),
                                                                        0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4);


        break;

        case FALLING_PLATFORM_WIDE_FALLING:

            fall_timer = FALLING_PLATFORM_WIDE_FALL_TIMER;

            animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                        0,
                                                                        bn::sprite_items::falling_platform_wide.tiles_item(),
                                                                        0, 0, 1, 1, 2, 2, 3, 3, 4, 4);

        break;

		default:
		break;
	}
}

/////////////////////////
// Collision Overrides //
/////////////////////////

void FallingPlatformWide::resolveCollision(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                                           const bn::regular_bg_ptr&                      bg_ptr, 
                                           const bn::span<const bn::regular_bg_map_cell>& cells,
                                           const bn::regular_bg_item&                     bg_item)
{

}