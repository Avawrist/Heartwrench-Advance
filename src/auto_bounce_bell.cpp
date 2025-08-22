#include "auto_bounce_bell.h"

///////////////////////
// Struct AutoBounceBell //
///////////////////////

AutoBounceBell::AutoBounceBell()
{
    // Init Assets //
    object_type = AUTO_BOUNCE_BELL;
    sprite_ptr  = bn::sprite_items::auto_bounce_bell.create_sprite(0, 0);
    sprite_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                    2,
                                                                    bn::sprite_items::auto_bounce_bell.tiles_item(),
                                                                    3, 3, 12, 12);

    // Init Variables //
    collider_offset_x = AUTO_BOUNCE_BELL_COLLIDER_OFFSET_X;
    collider_offset_y = AUTO_BOUNCE_BELL_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        AUTO_BOUNCE_BELL_COLLIDER_WIDTH, 
                        AUTO_BOUNCE_BELL_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;

    state = IDLE;

    hitpoints = AUTO_BOUNCE_BELL_HITPOINTS;

    thirty_fps = AUTO_BOUNCE_BELL_30_FPS;

    roll_x_dir = RIGHT;
    roll_y_dir = Y_NONE;
}

AutoBounceBell::AutoBounceBell(const AutoBounceBell& other) : GameObject(other)
{
    roll_x_dir = other.roll_x_dir;
    roll_y_dir = other.roll_y_dir;
}

AutoBounceBell::~AutoBounceBell()
{

}

AutoBounceBell& AutoBounceBell::operator =(const AutoBounceBell& other)
{
    roll_x_dir = other.roll_x_dir;
    roll_y_dir = other.roll_y_dir;

    return *this;
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

void AutoBounceBell::update(const RoomBounds& 							   room_bounds,
                        bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                        const bn::regular_bg_ptr&                      bg_ptr, 
                        const bn::span<const bn::regular_bg_map_cell>& cells,
                        const bn::regular_bg_item&                     bg_item,
                        const bn::camera_ptr&                          camera)
{
    
    /////////////////////////////////
    // Early out for 30FPS Objects //
    /////////////////////////////////

    if((thirty_fps || is_frozen) && (global_timer % 2 == 0)) {return;}

    //////////////////
    // Update State //
    //////////////////

    updateStateMachine(game_objects, bg_ptr, cells, bg_item, camera);

    ////////////////////
    // Update Physics //
    ////////////////////

    //updatePhysics();

    ///////////////////////
    // Resolve Collision //
    ///////////////////////

    resolveCollision(game_objects, bg_ptr, cells, bg_item);

    //////////////////
    // Update State //
    //////////////////

    updateState(game_objects, bg_ptr, cells, bg_item);

    /////////////////////
    // Update Hitboxes //
    /////////////////////

    //updateHitboxes(room_bounds, game_objects, bg_ptr, cells, bg_item, camera);

    ///////////////////
    // Update Timers //
    ///////////////////

    updateTimers();

    ///////////////////
    // Check if dead //
    ///////////////////

    //checkIfDead();

    /////////////////////////////
	// Update Sprite Direction //
	/////////////////////////////
	
    //updateSpriteDirection();
    
    ////////////////////////////
    // Correct Sprite Offsets //
    ////////////////////////////
	
    //updateSpriteOffsets();

    //////////////////////////
    // Update HP Bar Visual //
    //////////////////////////

    //updateHPBar();

	//////////////////////
	// Update Hit Flash //
	//////////////////////

	updateHitFlash();

    ////////////////////
    // Clamp Position //
    ////////////////////

    clampPosition(room_bounds);

	//////////////////////////////
    // Monitor unloading bounds //
    //////////////////////////////
    
	updateInactiveState(camera);

}

//////////////////////////////
// State Function Overrides //
//////////////////////////////

void AutoBounceBell::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                                    const bn::regular_bg_ptr&                      bg_ptr, 
                                    const bn::span<const bn::regular_bg_map_cell>& cells,
                                    const bn::regular_bg_item&                     bg_item,
                                    const bn::camera_ptr&                          camera)
{

    // Rolling
    setX(x() + (AUTO_BOUNCE_BELL_SPEED * (int32)roll_x_dir));
    setY(y() + (AUTO_BOUNCE_BELL_SPEED * (int32)roll_y_dir));

    switch(state)
    {
        case IDLE:

            if(animate_action_ptr->done())
            {
                animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                                2,
                                                                                bn::sprite_items::auto_bounce_bell.tiles_item(),
                                                                                3, 3, 12, 12);
            }

        break;

        case OBJECT_HITSTUN:

            updateHitstunState();

        break;

		case OBJECT_DEATH:

			updateDeathState();

		break;
		
		default:
		break;
    }
}

void AutoBounceBell::setState(ObjectState new_state)
{
	// Set State
	state = new_state;

	// Set animations & other state specific variables
	switch(new_state)
	{
        case OBJECT_HITSTUN:

            animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
                                                                        4,
                                                                        bn::sprite_items::auto_bounce_bell.tiles_item(),
                                                                        7, 8, 9, 10, 11, 6,
                                                                        5, 4, 3, 2, 1, 
                                                                        2, 2, 3, 3, 4, 4, 5, 5,
                                                                        4, 4, 4, 4, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2);


        break;

		case OBJECT_DEATH:
		break;

		default:
		break;
	}
}

/////////////////////////
// Collision Overrides //
/////////////////////////

void AutoBounceBell::resolveObjectCollision(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects) {}

void AutoBounceBell::resolveHardBlockCollision(const Collider& other_collider) {}

void AutoBounceBell::resolveHGearLeftCollision(const Collider& other_collider)
{
    if(collider.isCollision(other_collider))
    {
        // Clamp to track height
        //setY(other_collider.y());

        // Lock Y direction
        roll_y_dir = Y_NONE;

        // Apply end cap
		if(rigidbody.normalized_dir.x() < 0 && x() < (other_collider.x() - (TILE_WIDTH / 2)))
		{
            // Update X direction
            roll_x_dir = RIGHT;

			setX(other_collider.x() - (TILE_WIDTH / 2));
			rigidbody.removeXForces();
		}
    }
}

void AutoBounceBell::resolveHGearMidCollision(const Collider& other_collider)
{
    if(collider.isCollision(other_collider))
    {
        // Clamp to track height
        //setY(other_collider.y());

        // Lock Y direction
        roll_y_dir = Y_NONE;
    }
}

void AutoBounceBell::resolveHGearRightCollision(const Collider& other_collider)
{
    if(collider.isCollision(other_collider))
    {
        // Clamp to track height
        //setY(other_collider.y());

        // Lock Y direction
        roll_y_dir = Y_NONE;

        // Apply end cap
		if(rigidbody.normalized_dir.x() > 0 && x() > (other_collider.x() + (TILE_WIDTH / 2)))
		{
            // Update X direction
            roll_x_dir = LEFT;

			setX(other_collider.x() + (TILE_WIDTH / 2));
			rigidbody.removeXForces();
		}
    }
}

void AutoBounceBell::resolveVGearTopCollision(const Collider& other_collider)
{
    if(collider.isCollision(other_collider))
    {
        // Clamp to track
        setX(other_collider.x() + (TILE_WIDTH / 2));

        // Lock X direction
        roll_x_dir = X_NONE;

        // Apply end cap
		if(roll_y_dir == UP && y() < (other_collider.y()))
		{
            // Update Y direction
            roll_y_dir = DOWN;

			setY(other_collider.y());
			rigidbody.removeYForces();
		}
    }
}

void AutoBounceBell::resolveVGearMidCollision(const Collider& other_collider)
{
    if(collider.isCollision(other_collider))
    {
        // Clamp to track
        setX(other_collider.x() + (TILE_WIDTH / 2));

        // Lock Y direction
        roll_x_dir = X_NONE;
    }
}

void AutoBounceBell::resolveVGearBottomCollision(const Collider& other_collider)
{
    if(collider.isCollision(other_collider))
    {
        // Clamp to track
        setX(other_collider.x() + (TILE_WIDTH / 2));

        // Lock X direction
        roll_x_dir = X_NONE;

        // Apply end cap
		if(roll_y_dir == DOWN && y() > (other_collider.y()))
		{
            // Update Y direction
            roll_y_dir = UP;

			setY(other_collider.y());
			rigidbody.removeYForces();
		}
    }
}

void AutoBounceBell::resolveSpikeCollision(const Collider& other_collider)       {}

void AutoBounceBell::resolveClimbableCollision(const Collider& other_collider)   {}

void AutoBounceBell::resolveOneWayBlockCollision(const Collider& other_collider) {}
