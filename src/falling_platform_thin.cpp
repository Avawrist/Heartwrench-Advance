#include "falling_platform_thin.h"

////////////////////////////////
// Struct FallingPlatformThin //
////////////////////////////////

FallingPlatformThin::FallingPlatformThin()
{
    // Init Assets //
    state = FALLING_PLATFORM_THIN_NEUTRAL;

    object_type = FALLING_PLATFORM_THIN;
    sprite_ptr  = bn::sprite_items::falling_platform_thin.create_sprite(0, 0);
    sprite_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                   2,
								                                   bn::sprite_items::falling_platform_thin.tiles_item(),
								                                   0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3);

    // Init Variables //
    collider_offset_x = FALLING_PLATFORM_THIN_COLLIDER_OFFSET_X;
    collider_offset_y = FALLING_PLATFORM_THIN_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        FALLING_PLATFORM_THIN_COLLIDER_WIDTH, 
                        FALLING_PLATFORM_THIN_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;

    hitpoints = FALLING_PLATFORM_THIN_HITPOINTS;

    fall_timer = 0;
}

FallingPlatformThin::FallingPlatformThin(const FallingPlatformThin& other) : GameObject(other)
{
    fall_timer = other.fall_timer;
}

FallingPlatformThin::~FallingPlatformThin()
{

}

FallingPlatformThin& FallingPlatformThin::operator =(const FallingPlatformThin& other)
{
    fall_timer = other.fall_timer;

    return *this;
}

//////////////////////////////
// State Function Overrides //
//////////////////////////////

void FallingPlatformThin::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                                             const bn::regular_bg_ptr&                      bg_ptr, 
                                             const bn::span<const bn::regular_bg_map_cell>& cells,
                                             const bn::regular_bg_item&                     bg_item,
                                             const bn::camera_ptr&                          camera)
{
    ///////////////////////////////////////
    // FallingPlatformThin State Machine //
    ///////////////////////////////////////
	
    switch(state)
    {
		case FALLING_PLATFORM_THIN_NEUTRAL:
        break;

        case FALLING_PLATFORM_THIN_FALLING:

            fall_timer++;
            fall_timer = clamp(0, FALLING_PLATFORM_THIN_SPEEDUP_TIMER, fall_timer);

            if(fall_timer >= FALLING_PLATFORM_THIN_FALL_TIMER) 
            {rigidbody.addForce(FALLING_PLATFORM_THIN_GRAVITY_FORCE);}

            if(fall_timer >= FALLING_PLATFORM_THIN_SPEEDUP_TIMER)
            {rigidbody.addForce(FALLING_PLATFORM_THIN_GRAVITY_FORCE);}

        break;
		
		default:
		break;
    }
}

void FallingPlatformThin::setState(ObjectState new_state)
{
    // Set State
	state = new_state;

	// Set animations & other state specific variables
	switch(new_state)
	{
		
		case NONE:
		break;

		case FALLING_PLATFORM_THIN_NEUTRAL:

            fall_timer = 0;

            animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                            2,
                                                                            bn::sprite_items::falling_platform_thin.tiles_item(),
                                                                            0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3);

        break;

        case FALLING_PLATFORM_THIN_FALLING:

            fall_timer = FALLING_PLATFORM_THIN_FALL_TIMER;

            animate_action_ptr  = bn::create_sprite_animate_action_once(sprite_ptr.value(),
                                                                            1,
                                                                            bn::sprite_items::falling_platform_thin.tiles_item(),
                                                                            4, 5, 6, 7);


        break;

		default:
		break;
	}
}

/////////////////////////
// Collision Overrides //
/////////////////////////

void FallingPlatformThin::resolvePlayerCollision(GameObject& object)
{
    Collider roof_test_collider = Collider(collider.x(),
                                           collider.y() + FALLING_PLATFORM_THIN_ROOF_OFFSET,
                                           collider.width,
                                           FALLING_PLATFORM_THIN_ROOF_COLLIDER_HEIGHT);

    // If object is riding the platform:
    if(roof_test_collider.isCollision(object.collider) &&
       object.collider.p4.y() < collider.p1.y() - rigidbody.final_dir.y() &&
       object.state != PLAYER_AIR_NEUTRAL) 
    {        
        if(rigidbody.final_dir.y() > 0)
        {
            // If descending, apply force to BOTH axes
            // so the object hugs the platform tight.
            object.rigidbody.addForce(Force(bn::fixed_point_t<12>(0, rigidbody.final_dir.y()), 1));
        }
    }
}

void FallingPlatformThin::resolveTileCollision(const bn::regular_bg_ptr&                      bg_ptr, 
                                               const bn::span<const bn::regular_bg_map_cell>& cells,
                                               const bn::regular_bg_item&                     bg_item)
{

}