#include "small_vase.h"

//////////////////////
// Struct SmallVase //
//////////////////////

SmallVase::SmallVase()
{
    // Init Assets //
    object_type = SMALL_VASE;
    sprite_ptr  = bn::sprite_items::small_vase.create_sprite(0, 0);
    sprite_ptr->set_z_order(PROP_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                   0,
								                                   bn::sprite_items::small_vase.tiles_item(),
								                                   0, 0);

    // Init Variables //
    collider_offset_x = SMALL_VASE_COLLIDER_OFFSET_X;
    collider_offset_y = SMALL_VASE_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        SMALL_VASE_COLLIDER_WIDTH, 
                        SMALL_VASE_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;

    state = IDLE;

    hitpoints = SMALL_VASE_HITPOINTS;
    max_hp    = SMALL_VASE_HITPOINTS;

    thirty_fps = SMALL_VASE_30_FPS;
}

SmallVase::SmallVase(const SmallVase& other) : GameObject(other)
{}

SmallVase::~SmallVase()
{

}

SmallVase& SmallVase::operator =(const SmallVase& other)
{
    return *this;
}

void SmallVase::wallSplatCheck()
{
	if(col_x_offset != 0 &&
	   (state == OBJECT_HITSTUN) &&
	   abs(rigidbody.final_dir.x().integer()) >= GAME_OBJECT_REQUIRED_SPLAT_SPEED)
	{

		XDirection splat_x_dir;
		if(col_x_offset < 0) {splat_x_dir = LEFT;}
		else                 {splat_x_dir = RIGHT;}

		Hitbox temp_hitbox(bn::point(0, 0),
							WALL_SPLAT_HITSTOP_FRAMES,
							WALL_SPLAT_HITSTUN_FRAMES,
							WALL_SPLAT_SCREENSHAKE_FRAMES,
							WALL_SPLAT_HB_LIFESPAN_FRAMES,
							WALL_SPLAT_X_KNOCKBACK,
							WALL_SPLAT_Y_KNOCKBACK,	
							WALL_SPLAT_KNOCKBACK_DECAY,
							WALL_SPLAT_HB_WIDTH,
							WALL_SPLAT_HB_HEIGHT,
							WALL_SPLAT_DAMAGE,
							splat_x_dir,
							y_dir,
							HITBOX_WALL_SPLAT,
							WALL_SPLAT_SCREENSHAKE_SEVERITY);

		temp_hitbox.applyWallHit(*this);
	}
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

void SmallVase::update(const RoomBounds& 							   room_bounds,
                        bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                        const bn::regular_bg_ptr&                      bg_ptr, 
                        const bn::span<const bn::regular_bg_map_cell>& cells,
                        const bn::regular_bg_item&                     bg_item,
                        const bn::camera_ptr&                          camera)
{
    /////////////////////////////////
    // Early out for 30FPS Objects //
    /////////////////////////////////

    if(thirty_fps && (global_timer % 2 == 0)) {return;}

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

    //resolveCollision(game_objects, bg_ptr, cells, bg_item);

    ///////////////////
    // Update Timers //
    ///////////////////

    //updateTimers();

    ///////////////////
    // Check if dead //
    ///////////////////

    checkIfDead();

	//////////////////////
	// Update Hit Flash //
	//////////////////////

	//updateHitFlash();

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

void SmallVase::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                                   const bn::regular_bg_ptr&                      bg_ptr, 
                                   const bn::span<const bn::regular_bg_map_cell>& cells,
                                   const bn::regular_bg_item&                     bg_item,
                                   const bn::camera_ptr&                          camera)
{
    switch(state)
    {
        case IDLE:
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

void SmallVase::setState(ObjectState new_state)
{
	// Set State
	state = new_state;

	// Set animations & other state specific variables
	switch(new_state)
	{
		case OBJECT_DEATH:

            object_request = ObjectRequest(GEAR_DROP, bn::fixed_point(x(), y()));

            sprite_ptr->set_z_order(ENEMY_Z_ORDER);
			
			playVaseDeathAnim();

		break;

		case OBJECT_HITSTUN:

			// SFX
			bn::sound_items::vase_hit.play();
			
		break;

		default:
		break;
	}
}

/////////////////////////
// Collision Overrides //
/////////////////////////

// Level Objects
void SmallVase::resolveTilePassageCollision(GameObject& object)
{
	if(object.state == TILE_PASSAGE_SHUT &&
	   collider.isCollision(object.collider))
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
	if(object.state == TILE_PASSAGE_SHUT &&
		test_collider.isCollision(object.collider) && 
		rigidbody.normalized_dir.y() >= 0)
	{grounded_detected = true;}
}

void SmallVase::resolveFallingPlatformWideCollision(GameObject& object) 
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

void SmallVase::resolveFallingPlatformThinCollision(GameObject& object)
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

void SmallVase::resolvePushBlockCollision(GameObject& object)
{
	if(collider.isCollision(object.collider))
    {
		if(object.rigidbody.normalized_dir.x() != 0 || 
           object.rigidbody.normalized_dir.y() != 0)
        {
            GameObject::applyHit(object.damage, object.rigidbody.normalized_dir.x().integer(), 0); 
        }
		else
		{
			// Resolve X Axis Collision //
        	resolveXAxisCollision(object.collider);

        	// Resolve Y Axis Collision //
        	resolveYAxisCollision(object.collider);

        	// If there is still collision somehow, must be corner case //
        	//resolveCornerCollision(object.collider);

			// Push the block
			if(state == OBJECT_HITSTUN || state == OBJECT_DEATH)
			{object.applyHit(damage, rigidbody.normalized_dir.x().integer(), 0);}
		}
    }

	updateTestColliders();

	// Test for, and log grounded collision
	if(test_collider.isCollision(object.collider) && 
		rigidbody.normalized_dir.y() >= 0)
	{grounded_detected = true;}
}

void SmallVase::resolvePushBlockMiniCollision(GameObject& object)
{
	if(collider.isCollision(object.collider))
    {
		if(object.rigidbody.normalized_dir.x() != 0 || 
           object.rigidbody.normalized_dir.y() != 0)
        {
            GameObject::applyHit(object.damage, object.rigidbody.normalized_dir.x().integer(), 0);  
        }
		else
		{
			// Resolve X Axis Collision //
        	resolveXAxisCollision(object.collider);

        	// Resolve Y Axis Collision //
        	resolveYAxisCollision(object.collider);

        	// If there is still collision somehow, must be corner case //
        	//resolveCornerCollision(object.collider);

			// Push the block
			if(state == OBJECT_HITSTUN || state == OBJECT_DEATH)
			{object.applyHit(damage, rigidbody.normalized_dir.x().integer(), 0);}
		}
    }

	updateTestColliders();

	// Test for, and log grounded collision
	if(test_collider.isCollision(object.collider) && 
		rigidbody.normalized_dir.y() >= 0)
	{grounded_detected = true;}
}

void SmallVase::resolveAutoPlatformCollision(GameObject& object)
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

void SmallVase::resolveSmashBlockLargeCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

        // If there is still collision somehow, must be corner case //
        //resolveCornerCollision(object.collider);

		// Smash the block
		if(state == OBJECT_HITSTUN || state == OBJECT_DEATH)
		{object.applyHit(damage, 0, 0);}
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

void SmallVase::resolveSmashBlockMiniCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

        // If there is still collision somehow, must be corner case //
        //resolveCornerCollision(object.collider);

		// Smash the block
		if(state == OBJECT_HITSTUN || state == OBJECT_DEATH)
		{object.applyHit(damage, 0, 0);}
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

void SmallVase::resolveSmashBlockZigguratLCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

        // If there is still collision somehow, must be corner case //
        //resolveCornerCollision(object.collider);

		// Smash the block
		if(state == OBJECT_HITSTUN || state == OBJECT_DEATH)
		{object.applyHit(damage, 0, 0);}
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

void SmallVase::resolveSmashBlockZigguratCCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

        // If there is still collision somehow, must be corner case //
        //resolveCornerCollision(object.collider);

		// Smash the block
		if(state == OBJECT_HITSTUN || state == OBJECT_DEATH)
		{object.applyHit(damage, 0, 0);}
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

void SmallVase::resolveSmashBlockZigguratRCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

        // If there is still collision somehow, must be corner case //
        //resolveCornerCollision(object.collider);

		// Smash the block
		if(state == OBJECT_HITSTUN || state == OBJECT_DEATH)
		{object.applyHit(damage, 0, 0);}
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

void SmallVase::resolveLargeVaseCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
		// Smash the block
		if(state == OBJECT_HITSTUN || state == OBJECT_DEATH)
		{object.applyHit(damage, 0, 0);}
    }
}

void SmallVase::resolveSmallVaseCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
		// Smash the block
		if(state == OBJECT_HITSTUN || state == OBJECT_DEATH)
		{object.applyHit(damage, 0, 0);}
    }
}

void SmallVase::resolveHPTotemCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

        // If there is still collision somehow, must be corner case //
        //resolveCornerCollision(object.collider);

		// Smash the block
		if(state == OBJECT_HITSTUN || state == OBJECT_DEATH)
		{object.applyHit(damage, 0, 0);}
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


// Level Enemies
void SmallVase::resolveThornColumnCollision(GameObject& object)
{
	int32 thorn_collision_x_offset = collider.getCollisionXOffset(object.collider, rigidbody.normalized_dir.x()).integer();
	
	resolveXAxisCollision(object.collider);
	resolveYAxisCollision(object.collider);

	if(thorn_collision_x_offset != 0 &&
	   hitpoints > 0)
	{
		int32 knockback_x_dir = abs(thorn_collision_x_offset) / thorn_collision_x_offset;
		GameObject::applyHit(object.damage, knockback_x_dir, 0);
	}
}

void SmallVase::resolveThornBarCollision(GameObject& object)
{
	resolveXAxisCollision(object.collider);
	resolveYAxisCollision(object.collider);

	if(collider.isCollision(object.collider) &&
	   hitpoints > 0)
	{
		GameObject::applyHit(object.damage, 0, 0);
	}
}

void SmallVase::resolveGroundGhoulCollision(GameObject& object)
{	
	if(collider.isCollision(object.collider) &&
       (object.state == OBJECT_HITSTUN || object.state == OBJECT_DEATH))
	{
		applyHit(object.damage, object.rigidbody.normalized_dir.x().integer(), 0);
	}
}

// Tiles

void SmallVase::resolveSpikeCollision(const Collider& other_collider)
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

void SmallVase::resolveXAxisCollision(const Collider& other_collider)
{
	GameObject::resolveXAxisCollision(other_collider);
	
	// Wall splat check
	wallSplatCheck();
}