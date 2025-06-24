#include "auto_platform.h"

/////////////////////////
// Struct AutoPlatform //
/////////////////////////

AutoPlatform::AutoPlatform()
{
    // Init Assets //
    object_type = AUTO_PLATFORM;
    sprite_ptr  = bn::sprite_items::auto_platform.create_sprite(0, 0);
    sprite_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                   0,
								                                   bn::sprite_items::auto_platform.tiles_item(),
								                                   0, 0);

    // Init Variables //
    collider_offset_x = AUTO_PLATFORM_COLLIDER_OFFSET_X;
    collider_offset_y = AUTO_PLATFORM_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        AUTO_PLATFORM_COLLIDER_WIDTH, 
                        AUTO_PLATFORM_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;

    state     = IDLE;
    hitpoints = AUTO_PLATFORM_HITPOINTS;

    changed_x_dir = false;
}

AutoPlatform::AutoPlatform(const AutoPlatform& other) : GameObject(other)
{
    changed_x_dir = other.changed_x_dir;
}

AutoPlatform::~AutoPlatform()
{

}

AutoPlatform& AutoPlatform::operator =(const AutoPlatform& other)
{
    changed_x_dir = other.changed_x_dir;

    return *this;
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

void AutoPlatform::checkIfDead()
{

}

void AutoPlatform::updateSpriteDirection()
{

}

void AutoPlatform::updateTimers()
{
    GameObject::updateTimers();

    changed_x_dir = false;
}

//////////////////////////////
// State Function Overrides //
//////////////////////////////

void AutoPlatform::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                                      const bn::regular_bg_ptr&                      bg_ptr, 
                                      const bn::span<const bn::regular_bg_map_cell>& cells,
                                      const bn::regular_bg_item&                     bg_item,
                                      const bn::camera_ptr&                          camera)
{
    switch(state)
    {
        case IDLE:

            // Rolling
            rigidbody.addForce(AUTO_PLATFORM_ROLL_FORCE);

            // Gravity
            //rigidbody.addForce(GAME_OBJECT_GRAVITY_FORCE);

        break;

        case OBJECT_HITSTUN:

            updateHitstunState();

        break;

        default:
        break;
    }
}

/////////////////////////
// Collision Overrides //
/////////////////////////

// Level Enemies
void AutoPlatform::resolveGroundGhoulCollision(GameObject& object)
{
    Collider roof_test_collider = Collider(collider.x(),
                                           collider.y() + AUTO_PLATFORM_ROOF_OFFSET,
                                           collider.width,
                                           AUTO_PLATFORM_ROOF_COLLIDER_HEIGHT);

    // If object is riding the platform:
    if(roof_test_collider.isCollision(object.collider) &&
       object.collider.p4.y() < collider.p1.y() - rigidbody.final_dir.y()) 
    {        
        if(rigidbody.final_dir.y() <= 0)
        {
            // If descending, applying force to the x axis is all that's needed.
            // The object gravity will take care of the rest. 
            object.rigidbody.addForce(Force(bn::fixed_point_t<12>(rigidbody.final_dir.x(), 0), 1));
        }
        else
        {
            // If ascending, apply force to BOTH axes and offset y by 1 
            // so the object hugs the platform tight.
            object.rigidbody.addForce(Force(bn::fixed_point_t<12>(rigidbody.final_dir.x(), rigidbody.final_dir.y() + 1), 1));
        }
    }
}

// Special Objects
void AutoPlatform::resolvePlayerCollision(GameObject& object)
{
    Collider roof_test_collider = Collider(collider.x(),
                                           collider.y() + AUTO_PLATFORM_ROOF_OFFSET,
                                           collider.width,
                                           AUTO_PLATFORM_ROOF_COLLIDER_HEIGHT);

    // If object is riding the platform:
    if(roof_test_collider.isCollision(object.collider) &&
       object.collider.p4.y() < collider.p1.y() - rigidbody.final_dir.y()) 
    {        
        if(rigidbody.final_dir.y() <= 0)
        {
            // If descending, applying force to the x axis is all that's needed.
            // The object gravity will take care of the rest. 
            if(!changed_x_dir)
            {object.rigidbody.addForce(Force(bn::fixed_point_t<12>(rigidbody.final_dir.x(), 0), 1));}
        }
        else
        {
            // If ascending, apply force to BOTH axes and offset y by 1 
            // so the object hugs the platform tight.
            if(!changed_x_dir)
            {object.rigidbody.addForce(Force(bn::fixed_point_t<12>(rigidbody.final_dir.x(), rigidbody.final_dir.y() + 1), 1));}
        }
    }
}

// Tiles
void AutoPlatform::resolveHardBlockCollision(const Collider& other_collider) {}

void AutoPlatform::resolveHGearLeftCollision(const Collider& other_collider)
{
    if(collider.isCollision(other_collider))
    {
        // Clamp to track height
        setY(other_collider.y() + (TILE_HEIGHT / 2) - AUTO_PLATFORM_COLLIDER_HEIGHT);

        // Lock Y direction
        y_dir = Y_NONE;

        // Apply end cap
		if(rigidbody.normalized_dir.x() < 0 && x() < (other_collider.x() - (TILE_WIDTH / 2)))
		{
            // Update X direction
            x_dir = RIGHT;
            changed_x_dir = true;

			setX(other_collider.x() - (TILE_WIDTH / 2));
			rigidbody.removeXForces();
		}
    }
}

void AutoPlatform::resolveHGearMidCollision(const Collider& other_collider)
{
    if(collider.isCollision(other_collider))
    {
        // Clamp to track height
        setY(other_collider.y() + (TILE_HEIGHT / 2) - AUTO_PLATFORM_COLLIDER_HEIGHT);

        // Lock Y direction
        y_dir = Y_NONE;
    }
}

void AutoPlatform::resolveHGearRightCollision(const Collider& other_collider)
{
    if(collider.isCollision(other_collider))
    {
        // Clamp to track height
        setY(other_collider.y() + (TILE_HEIGHT / 2) - AUTO_PLATFORM_COLLIDER_HEIGHT);

        // Lock Y direction
        y_dir = Y_NONE;

        // Apply end cap
		if(rigidbody.normalized_dir.x() > 0 && x() > (other_collider.x() + (TILE_WIDTH / 2)))
		{
            // Update X direction
            x_dir = LEFT;
            changed_x_dir = true;

			setX(other_collider.x() + (TILE_WIDTH / 2));
			rigidbody.removeXForces();
		}
    }
}

void AutoPlatform::resolveVGearTopCollision(const Collider& other_collider)
{
    if(collider.isCollision(other_collider))
    {
        // Clamp to track
        setX(other_collider.x() + (TILE_WIDTH / 2));

        // Lock X direction
        x_dir = X_NONE;

        // Apply end cap
		if(y_dir == UP && y() < (other_collider.y()))
		{
            // Update Y direction
            y_dir = DOWN;

			setY(other_collider.y());
			rigidbody.removeYForces();
		}
    }
}

void AutoPlatform::resolveVGearMidCollision(const Collider& other_collider)
{
    if(collider.isCollision(other_collider))
    {
        // Clamp to track
        setX(other_collider.x() + (TILE_WIDTH / 2));

        // Lock Y direction
        x_dir = X_NONE;
    }
}

void AutoPlatform::resolveVGearBottomCollision(const Collider& other_collider)
{
    if(collider.isCollision(other_collider))
    {
        // Clamp to track
        setX(other_collider.x() + (TILE_WIDTH / 2));

        // Lock X direction
        x_dir = X_NONE;

        // Apply end cap
		if(y_dir == DOWN && y() > (other_collider.y()))
		{
            // Update Y direction
            y_dir = UP;

			setY(other_collider.y());
			rigidbody.removeYForces();
		}
    }
}

void AutoPlatform::resolveUpSpikeCollision(const Collider& other_collider) {}
void AutoPlatform::resolveDownSpikeCollision(const Collider& other_collider) {}
void AutoPlatform::resolveLeftSpikeCollision(const Collider& other_collider) {}
void AutoPlatform::resolveRightSpikeCollision(const Collider& other_collider) {}
void AutoPlatform::resolveLeftShallowSlope1Collision(const Collider& other_collider, int32 world_y) {}
void AutoPlatform::resolveLeftShallowSlope2Collision(const Collider& other_collider, int32 world_y) {}
void AutoPlatform::resolveLeftShallowSlope3Collision(const Collider& other_collider, int32 world_y) {}
void AutoPlatform::resolveLeftShallowSlope4Collision(const Collider& other_collider, int32 world_y) {}
void AutoPlatform::resolveLeftSteepSlope1Collision(const Collider& other_collider, int32 world_y) {}
void AutoPlatform::resolveLeftSteepSlope2Collision(const Collider& other_collider, int32 world_y) {}
void AutoPlatform::resolveRightShallowSlope1Collision(const Collider& other_collider, int32 world_y) {}
void AutoPlatform::resolveRightShallowSlope2Collision(const Collider& other_collider, int32 world_y) {}
void AutoPlatform::resolveRightShallowSlope3Collision(const Collider& other_collider, int32 world_y) {}
void AutoPlatform::resolveRightShallowSlope4Collision(const Collider& other_collider, int32 world_y) {}
void AutoPlatform::resolveRightSteepSlope1Collision(const Collider& other_collider, int32 world_y) {}
void AutoPlatform::resolveRightSteepSlope2Collision(const Collider& other_collider, int32 world_y) {}
void AutoPlatform::resolveOneWayBlockCollision(const Collider& other_collider) {}
