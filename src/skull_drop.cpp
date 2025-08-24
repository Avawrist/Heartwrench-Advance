#include "skull_drop.h"

/////////////////////
// Struct SkullDrop //
/////////////////////

SkullDrop::SkullDrop()
{
    // Init Assets //
    object_type = SKULL_DROP;
    sprite_ptr  = bn::sprite_items::skull_drop.create_sprite(0, 0);
    sprite_ptr->set_z_order(ENEMY_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                   2,
								                                   bn::sprite_items::skull_drop.tiles_item(),
								                                   0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3);

    // Init Variables //
    collider_offset_x = SKULL_DROP_COLLIDER_OFFSET_X;
    collider_offset_y = SKULL_DROP_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y,
                        SKULL_DROP_COLLIDER_WIDTH, 
                        SKULL_DROP_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;

    setState(IDLE);

    hitpoints  = SKULL_DROP_HITPOINTS;
    thirty_fps = SKULL_DROP_30_FPS;
    damage     = SKULL_DROP_VALUE;

	invulnerability_frames = SKULL_DROP_STARTING_INVULN_FRAMES;

	rigidbody.addForce(SKULL_DROP_SPAWN_FORCE);
}

SkullDrop::SkullDrop(const SkullDrop& other) : GameObject(other)
{

}

SkullDrop::~SkullDrop()
{

}

SkullDrop& SkullDrop::operator =(const SkullDrop& other)
{
    return *this;
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

void SkullDrop::update(const RoomBounds& 							 room_bounds,
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

    updatePhysics();

    ///////////////////////
    // Resolve Collision //
    ///////////////////////

    if(!is_frozen)
    {resolveCollision(game_objects, bg_ptr, cells, bg_item);}

    //////////////////
    // Update State //
    //////////////////

    //updateState(game_objects, bg_ptr, cells, bg_item);

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

	//updateHitFlash();

    ////////////////////
    // Clamp Position //
    ////////////////////

    //clampPosition(room_bounds);

	//////////////////////////////
    // Monitor unloading bounds //
    //////////////////////////////
    
	updateInactiveState(camera);

}

void SkullDrop::setState(ObjectState new_state)
{
    state = new_state;

    switch(new_state)
    {
        case IDLE:

			animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
																		   2,
																		   bn::sprite_items::skull_drop.tiles_item(),
																		   0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3);

        break;

        case OBJECT_HITSTUN:
        break;

        case OBJECT_DEATH:

            animate_action_ptr  = bn::create_sprite_animate_action_once(sprite_ptr.value(),
                                                                        1,
                                                                        bn::sprite_items::skull_drop.tiles_item(),
                                                                        4, 4, 5, 5, 6, 6, 7, 7, 8, 8);

        break;

        default:
        break;
    }
}

//////////////////////////////
// State Function Overrides //
//////////////////////////////

void SkullDrop::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                                const bn::regular_bg_ptr&                      bg_ptr, 
                                const bn::span<const bn::regular_bg_map_cell>& cells,
                                const bn::regular_bg_item&                     bg_item,
                                const bn::camera_ptr&                          camera)
{
    switch(state)
    {
        case IDLE:

            // Gravity
            if(!grounded_detected)
            {rigidbody.addForce(GAME_OBJECT_GRAVITY_FORCE);}

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

/////////////////////////
// Collision Overrides //
/////////////////////////

// Level Objects
void SkullDrop::resolveTilePassageCollision(GameObject& object)
{
	if(object.state == TILE_PASSAGE_SHUT &&
	   collider.isCollision(object.collider))
	{
		// Resolve X Axis Collision //
		resolveXAxisCollision(object.collider);

		// Resolve Y Axis Collision //
		resolveYAxisCollision(object.collider);

		// Corner collision //
        if(col_x_offset == 0 && col_y_offset == 0)
		{resolveCornerCollision(object.collider);}
	}

	updateTestColliders();

	// Test for, and log grounded collision
	if(object.state == TILE_PASSAGE_SHUT &&
		test_collider.isCollision(object.collider) && 
		rigidbody.normalized_dir.y() >= 0)
	{grounded_detected = true;}
}

void SkullDrop::resolveFallingPlatformWideCollision(GameObject& object)
{
	if(rigidbody.normalized_dir.y() >= 0 &&
	   collider_y_axis.p4.y() <= object.collider.p1.y() + rigidbody.final_dir.y())
	{
		// Resolve Collision //
		while(collider_y_axis.isCollision(object.collider))
		{
			collider_y_axis.setY(collider_y_axis.y() - 1);
			setY(this->y() - 1);
		}

		updateTestColliders();

		// Test for, and log grounded collision
		if(test_collider.isCollision(object.collider) &&
			rigidbody.normalized_dir.y() >= 0)
		{
			grounded_detected = true;
			rigidbody.removeYForces();
		}
	}
}

void SkullDrop::resolveFallingPlatformThinCollision(GameObject& object)
{
	if(rigidbody.normalized_dir.y() >= 0 &&
	   collider_y_axis.p4.y() <= object.collider.p1.y() + rigidbody.final_dir.y())
	{
		// Resolve Collision //
		while(collider_y_axis.isCollision(object.collider))
		{
			collider_y_axis.setY(collider_y_axis.y() - 1);
			setY(this->y() - 1);
		}

		updateTestColliders();

		// Test for, and log grounded collision
		if(test_collider.isCollision(object.collider) &&
			rigidbody.normalized_dir.y() >= 0)
		{
			grounded_detected = true;
			rigidbody.removeYForces();
		}
	}
}

void SkullDrop::resolvePushBlockCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

		// Corner collision //
        if(col_x_offset == 0 && col_y_offset == 0)
		{resolveCornerCollision(object.collider);}
    }

	updateTestColliders();

	// Test for, and log grounded collision
	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{	
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}

void SkullDrop::resolvePushBlockMiniCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

		// Corner collision //
        if(col_x_offset == 0 && col_y_offset == 0)
		{resolveCornerCollision(object.collider);}
    }

	updateTestColliders();

	// Test for, and log grounded collision
	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{	
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}

void SkullDrop::resolveAutoPlatformCollision(GameObject& object)
{
	if(rigidbody.normalized_dir.y() >= 0 &&
	   collider_y_axis.p4.y() <= object.collider.p1.y() + rigidbody.final_dir.y())
	{
		// Resolve Collision //
		while(collider_y_axis.isCollision(object.collider))
		{
			collider_y_axis.setY(collider_y_axis.y() - 1);
			setY(this->y() - 1);
		}

		updateTestColliders();

		// Test for, and log grounded collision
		if(test_collider.isCollision(object.collider) &&
			rigidbody.normalized_dir.y() >= 0)
		{
			grounded_detected = true;
			rigidbody.removeYForces();
		}
	}
}

void SkullDrop::resolveSmashBlockLargeCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

		// Corner collision //
        if(col_x_offset == 0 && col_y_offset == 0)
		{resolveCornerCollision(object.collider);}
    }

	updateTestColliders();

	// Test for, and log grounded collision
	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{	
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}

void SkullDrop::resolveSmashBlockMiniCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

		// Corner collision //
        if(col_x_offset == 0 && col_y_offset == 0)
		{resolveCornerCollision(object.collider);}
    }

	updateTestColliders();

	// Test for, and log grounded collision
	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{	
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}

void SkullDrop::resolveSmashBlockZigguratLCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

		// Corner collision //
        if(col_x_offset == 0 && col_y_offset == 0)
		{resolveCornerCollision(object.collider);}
    }

	updateTestColliders();

	// Test for, and log grounded collision
	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{	
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}

void SkullDrop::resolveSmashBlockZigguratCCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

		// Corner collision //
        if(col_x_offset == 0 && col_y_offset == 0)
		{resolveCornerCollision(object.collider);}
    }

	updateTestColliders();

	// Test for, and log grounded collision
	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{	
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}

void SkullDrop::resolveSmashBlockZigguratRCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

		// Corner collision //
        if(col_x_offset == 0 && col_y_offset == 0)
		{resolveCornerCollision(object.collider);}
    }

	updateTestColliders();

	// Test for, and log grounded collision
	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{	
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}

void SkullDrop::resolveHPTotemCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

		// Corner collision //
        if(col_x_offset == 0 && col_y_offset == 0)
		{resolveCornerCollision(object.collider);}
    }

	updateTestColliders();

	// Test for, and log grounded collision
	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{	
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}

// Enemies

void SkullDrop::resolveBellTrollCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH || object.state == OBJECT_HITSTUN) {return;}

	switch(object.state)
	{
		case BELL_TROLL_FROZEN:

			if(collider.isCollision(object.collider))
			{
				// Resolve X Axis Collision //
				resolveXAxisCollision(object.collider);

				// Resolve Y Axis Collision //
				resolveYAxisCollision(object.collider);

				// If there is still collision somehow, must be corner case //
				//resolveCornerCollision(object.collider);
			}

			updateTestColliders();

			// Test for, and log grounded collision
			if(test_collider.isCollision(object.collider) && 
			rigidbody.normalized_dir.y() >= 0)
			{	
				grounded_detected = true;
				rigidbody.removeYForces();
			}

		break;

		default:
		break;
	}
}

// Tiles

void SkullDrop::resolveSpikeCollision(const Collider& other_collider)
{
    if(collider.isCollision(other_collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(other_collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(other_collider);

		// Corner collision //
        if(col_x_offset == 0 && col_y_offset == 0)
		{resolveCornerCollision(other_collider);}
    }

    updateTestColliders();

	// Test for, and log grounded collision
	if(test_collider.isCollision(other_collider) &&
		rigidbody.normalized_dir.y() >= 0)
	{
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}