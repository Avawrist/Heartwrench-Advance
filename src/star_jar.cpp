#include "star_jar.h"

///////////////////
// Struct StarJar //
///////////////////

StarJar::StarJar()
{
    // Init Assets //
    object_type = STAR_JAR;
    sprite_ptr  = bn::sprite_items::star_jar.create_sprite(0, 0);
    sprite_ptr->set_z_order(ENEMY_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                   2,
								                                   bn::sprite_items::star_jar.tiles_item(),
								                                   0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3);

    // Init Variables //
    collider_offset_x = STAR_JAR_COLLIDER_OFFSET_X;
    collider_offset_y = STAR_JAR_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y,
                        STAR_JAR_COLLIDER_WIDTH, 
                        STAR_JAR_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;

    setState(IDLE);

    hitpoints  = STAR_JAR_HITPOINTS;
    thirty_fps = STAR_JAR_30_FPS;
    damage     = STAR_JAR_VALUE;

	invulnerability_frames = STAR_JAR_STARTING_INVULN_FRAMES;

	rigidbody.addForce(STAR_JAR_SPAWN_FORCE);
}

StarJar::StarJar(const StarJar& other) : GameObject(other)
{

}

StarJar::~StarJar()
{

}

StarJar& StarJar::operator =(const StarJar& other)
{
    return *this;
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

void StarJar::update(const RoomBounds& 							 room_bounds,
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

    if(!is_frozen) {resolveCollision(game_objects, bg_ptr, cells, bg_item);}

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

void StarJar::setState(ObjectState new_state)
{
    state = new_state;

    switch(new_state)
    {
        case IDLE:

			animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
																		3,
																		bn::sprite_items::star_jar.tiles_item(),
																		0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3);

        break;

        case OBJECT_HITSTUN:
        break;

        case OBJECT_DEATH:

			global_stars_collected++;
			playDropDeathAnim();

        break;

        default:
        break;
    }
}

//////////////////////////////
// State Function Overrides //
//////////////////////////////

void StarJar::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&   game_objects,
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
void StarJar::resolveTilePassageCollision(GameObject& object)
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

void StarJar::resolveFallingPlatformWideCollision(GameObject& object)
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

void StarJar::resolveFallingPlatformThinCollision(GameObject& object)
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

void StarJar::resolvePushBlockCollision(GameObject& object)
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

void StarJar::resolvePushBlockMiniCollision(GameObject& object)
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

void StarJar::resolveAutoPlatformCollision(GameObject& object)
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

void StarJar::resolveSmashBlockLargeCollision(GameObject& object)
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

void StarJar::resolveSmashBlockMiniCollision(GameObject& object)
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

void StarJar::resolveSmashBlockZigguratLCollision(GameObject& object)
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

void StarJar::resolveSmashBlockZigguratCCollision(GameObject& object)
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

void StarJar::resolveSmashBlockZigguratRCollision(GameObject& object)
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

void StarJar::resolveHPTotemCollision(GameObject& object)
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

void StarJar::resolveBellTrollCollision(GameObject& object)
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

void StarJar::resolveSpikeCollision(const Collider& other_collider)
{
    if(collider.isCollision(other_collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(other_collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(other_collider);

        // If there is still collision somehow, must be corner case //
        //resolveCornerCollision(other_collider);
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