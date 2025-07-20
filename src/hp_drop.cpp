#include "hp_drop.h"

///////////////////
// Struct HPDrop //
///////////////////

HPDrop::HPDrop()
{
    // Init Assets //
    object_type = HP_DROP;
    sprite_ptr  = bn::sprite_items::hp_drop.create_sprite(0, 0);
    sprite_ptr->set_z_order(ENEMY_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                   2,
								                                   bn::sprite_items::hp_drop.tiles_item(),
								                                   0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3);

    // Init Variables //
    collider_offset_x = HP_DROP_COLLIDER_OFFSET_X;
    collider_offset_y = HP_DROP_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        HP_DROP_COLLIDER_WIDTH, 
                        HP_DROP_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;

    setState(IDLE);

    hitpoints  = HP_DROP_HITPOINTS;
    thirty_fps = HP_DROP_30_FPS;
    damage     = HP_DROP_VALUE;

	invulnerability_frames = HP_DROP_STARTING_INVULN_FRAMES;

	rigidbody.addForce(HP_DROP_SPAWN_FORCE);
}

HPDrop::HPDrop(const HPDrop& other) : GameObject(other)
{

}

HPDrop::~HPDrop()
{

}

HPDrop& HPDrop::operator =(const HPDrop& other)
{
    return *this;
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

void HPDrop::setState(ObjectState new_state)
{
    state = new_state;

    switch(new_state)
    {
        case IDLE:

            animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                            2,
                                                                            bn::sprite_items::hp_drop.tiles_item(),
                                                                            0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3);

        break;

        case OBJECT_HITSTUN:
        break;

        case OBJECT_DEATH:

            animate_action_ptr  = bn::create_sprite_animate_action_once(sprite_ptr.value(),
                                                                        0,
                                                                        bn::sprite_items::hp_drop.tiles_item(),
                                                                        4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9);
        break;

        default:
        break;
    }
}

//////////////////////////////
// State Function Overrides //
//////////////////////////////

void HPDrop::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
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
void HPDrop::resolveTilePassageCollision(GameObject& object)
{
	if(object.state == TILE_PASSAGE_SHUT &&
	   collider.isCollision(object.collider))
	{
		// Resolve X Axis Collision //
		resolveXAxisCollision(object.collider);

		// Resolve Y Axis Collision //
		resolveYAxisCollision(object.collider);

		// If there is still collision somehow, must be corner case //
		resolveCornerCollision(object.collider);
	}

	updateTestColliders();

	// Test for, and log grounded collision
	if(object.state == TILE_PASSAGE_SHUT &&
		test_collider.isCollision(object.collider) && 
		rigidbody.normalized_dir.y() >= 0)
	{grounded_detected = true;}
}

void HPDrop::resolveFallingPlatformWideCollision(GameObject& object)
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

void HPDrop::resolveFallingPlatformThinCollision(GameObject& object)
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

void HPDrop::resolvePushBlockCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

        // If there is still collision somehow, must be corner case //
        resolveCornerCollision(object.collider);

		// Smash the block
		if(abs(rigidbody.final_dir.x()) >= HP_DROP_MIN_HIT_SPEED)
		{
			object.applyHit(damage, rigidbody.normalized_dir.x().integer(), 0);
		}
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

void HPDrop::resolvePushBlockMiniCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

        // If there is still collision somehow, must be corner case //
        resolveCornerCollision(object.collider);

		// Smash the block
		if(abs(rigidbody.final_dir.x()) >= HP_DROP_MIN_HIT_SPEED)
		{
			object.applyHit(damage, rigidbody.normalized_dir.x().integer(), 0);
		}
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

void HPDrop::resolveAutoPlatformCollision(GameObject& object)
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

void HPDrop::resolveSmashBlockLargeCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

        // If there is still collision somehow, must be corner case //
        resolveCornerCollision(object.collider);

		// Smash the block
		if(abs(rigidbody.final_dir.x()) >= HP_DROP_MIN_HIT_SPEED)
		{
			object.applyHit(damage, rigidbody.normalized_dir.x().integer(), 0);
		}
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

void HPDrop::resolveSmashBlockMiniCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

        // If there is still collision somehow, must be corner case //
        resolveCornerCollision(object.collider);

		// Smash the block
		if(abs(rigidbody.final_dir.x()) >= HP_DROP_MIN_HIT_SPEED)
		{
			object.applyHit(damage, rigidbody.normalized_dir.x().integer(), 0);
		}
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

void HPDrop::resolveLargeVaseCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(abs(rigidbody.final_dir.x()) >= HP_DROP_MIN_HIT_SPEED && collider.isCollision(object.collider))
	{
		object.applyHit(damage, 0, 0);
	}
}

void HPDrop::resolveSmallVaseCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(abs(rigidbody.final_dir.x()) >= HP_DROP_MIN_HIT_SPEED && collider.isCollision(object.collider))
	{
		object.applyHit(damage, 0, 0);
	}
}

void HPDrop::resolveHPTotemCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

        // If there is still collision somehow, must be corner case //
        resolveCornerCollision(object.collider);

		// Smash the block
		if(abs(rigidbody.final_dir.x()) >= HP_DROP_MIN_HIT_SPEED)
		{
			object.applyHit(damage, rigidbody.normalized_dir.x().integer(), 0);
		}
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
void HPDrop::resolveThornColumnCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(abs(rigidbody.final_dir.x()) >= HP_DROP_MIN_HIT_SPEED && collider.isCollision(object.collider))
	{
		object.applyHit(damage, rigidbody.normalized_dir.x().integer(), 0);
	}
}

void HPDrop::resolveThornBarCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(abs(rigidbody.final_dir.x()) >= HP_DROP_MIN_HIT_SPEED && collider.isCollision(object.collider))
	{
		object.applyHit(damage, rigidbody.normalized_dir.x().integer(), 0);
	}
}

void HPDrop::resolveGroundGhoulCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(abs(rigidbody.final_dir.x()) >= HP_DROP_MIN_HIT_SPEED && collider.isCollision(object.collider))
	{
		object.applyHit(damage, rigidbody.normalized_dir.x().integer(), 0);
	}
}
