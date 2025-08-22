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

void AutoBounceBell::resolveTileCollision(const bn::regular_bg_ptr&                      bg_ptr, 
                                          const bn::span<const bn::regular_bg_map_cell>& cells,
                                          const bn::regular_bg_item&                     bg_item)
{

    //////////////////////////////
	// Init Collision Variables //
	//////////////////////////////

	// Get current cell index that object resides in:
	int32 half_level_width_pixels  = (bg_ptr.dimensions().width() / 2);
	int32 half_level_height_pixels = (bg_ptr.dimensions().height() / 2);
	bn::fixed index_x = (x() + half_level_width_pixels + collider_offset_x)  / TILE_WIDTH;
	bn::fixed index_y = (y() + half_level_height_pixels + collider_offset_y)  / TILE_HEIGHT;
	bn::point cell_index = bn::point(index_x.integer(), index_y.integer());

	// Update colliders for each axis. 
	collider_x_axis.setPos(collider.x(), collider.y() - rigidbody.final_dir.y());
	collider_y_axis.setPos(collider.x() - rigidbody.final_dir.x(), collider.y());

    // Placeholder for other objects
	Collider other_collider;

    for(int32 x = -1; x < 2; x++)
    {
        for(int32 y = -3; y < -1; y++)
        {
            // 1. Get tile type at index //
            int32 check_index_x = cell_index.x() + x;
            int32 check_index_y = cell_index.y() + y;

            // Determine world coords in case we need to make a collider.
            int32 world_x = ((check_index_x * TILE_WIDTH)  - half_level_width_pixels)  + (TILE_WIDTH  / 2);
            int32 world_y = ((check_index_y * TILE_HEIGHT) - half_level_height_pixels) + (TILE_HEIGHT / 2);

            uint32 tile_index = getTileAtBGIndex(check_index_x, check_index_y, 
                                                 bg_ptr, cells, bg_item);
            BN_LOG(tile_index);

            // 2. If the tile is collidable make a temporary collider based on type //
            if(tile_index == H_GEAR_LEFT)
            {
                other_collider = Collider(world_x,
                                            world_y, 
                                            TILE_WIDTH, 
                                            TILE_HEIGHT);
                
                resolveHGearLeftCollision(other_collider);
            }

            else if(tile_index >= H_GEAR_MID_MIN &&
                    tile_index <= H_GEAR_MID_MAX)
            {
                other_collider = Collider(world_x,
                                          world_y, 
                                          TILE_WIDTH, 
                                          TILE_HEIGHT);
                
                resolveHGearMidCollision(other_collider);
            }

            else if(tile_index == H_GEAR_RIGHT)
            {
                other_collider = Collider(world_x,
                                          world_y, 
                                          TILE_WIDTH, 
                                          TILE_HEIGHT);
                
                resolveHGearRightCollision(other_collider);
            }

            else if(tile_index == V_GEAR_TOP)
            {
                other_collider = Collider(world_x,
                                            world_y, 
                                            TILE_WIDTH, 
                                            TILE_HEIGHT);

                resolveVGearTopCollision(other_collider);
            }

            else if(tile_index == V_GEAR_MID_1 ||
                    tile_index == V_GEAR_MID_2 ||
                    tile_index == V_GEAR_MID_3)
            {
                other_collider = Collider(world_x,
                                            world_y, 
                                            TILE_WIDTH, 
                                            TILE_HEIGHT);

                resolveVGearMidCollision(other_collider);
            }

            else if(tile_index == V_GEAR_BOTTOM)
            {
                other_collider = Collider(world_x,
                                            world_y, 
                                            TILE_WIDTH, 
                                            TILE_HEIGHT);

                resolveVGearBottomCollision(other_collider);
            }
        }
    }

    /////////////////////////////
	// Test for Grounded State //
	/////////////////////////////

	updateTileGrounded(bg_ptr, cells, bg_item);
}

void AutoBounceBell::resolveHGearLeftCollision(const Collider& other_collider)
{
    if(collider.isCollision(other_collider))
    {
        // Clamp to track height
        setY(other_collider.y() + (3 * TILE_HEIGHT));

        // Lock Y direction
        roll_y_dir = Y_NONE;

        // Apply end cap
		if(roll_x_dir == LEFT && x() < (other_collider.x() - (TILE_WIDTH / 2)))
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
        setY(other_collider.y() + (3 * TILE_HEIGHT));

        // Lock Y direction
        roll_y_dir = Y_NONE;
    }
}

void AutoBounceBell::resolveHGearRightCollision(const Collider& other_collider)
{
    BN_LOG("test 0");
    if(collider.isCollision(other_collider))
    {
        // Clamp to track height
        setY(other_collider.y() + (3 * TILE_HEIGHT));

        // Lock Y direction
        roll_y_dir = Y_NONE;

        BN_LOG("test");

        // Apply end cap
		if(roll_x_dir == RIGHT && x() > (other_collider.x() + (TILE_WIDTH / 2)))
		{
            BN_LOG("test 2");
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
