#include "enemy.h"

//////////////////
// Struct Enemy //
//////////////////

Enemy::Enemy()
{
	damage = ENEMY_DAMAGE;
	max_hp = ENEMY_MAX_HP;

	hp_sprite_ptr->set_visible(true);
}

Enemy::Enemy(const Enemy& other) : GameObject(other)
{}

Enemy::~Enemy()
{

}

Enemy& Enemy::operator =(const Enemy& other)
{return *this;}

void Enemy::wallSplatCheck()
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

void Enemy::updateHitFlash()
{
    if((hit_flash_frames || invulnerability_frames) || state == OBJECT_DEATH)
    {
        bn::sprite_palette_ptr new_palette = bn::sprite_palette_items::sprite_flash_palette.create_palette();

        int32 rotate_range = new_palette.rotate_range_size();
        int32 rotate_count = new_palette.rotate_count();
        
        if(global_timer % 2 == 0)
        {
            rotate_count++;
            if(rotate_count > rotate_range - 1) {rotate_count = 0;}
        }

        new_palette.set_rotate_count(rotate_count);

        if(sprite_ptr.has_value()) {sprite_ptr->set_palette(new_palette);}
    }
    else {if(sprite_ptr.has_value()) {sprite_ptr->set_palette(default_palette_ptr.value());}}

    hit_flash_frames--;
    if(hit_flash_frames < 0)
    {hit_flash_frames = 0;}
}

void Enemy::updateHPBar()
{
	/*
	if(hitpoints > ENEMY_MAX_HP) {return;}

	hp_sprite_ptr->set_position(x() + ENEMY_HP_BAR_X_OFFSET, y() + ENEMY_HP_BAR_Y_OFFSET);
	hp_animate_action_ptr = bn::create_sprite_animate_action_forever(hp_sprite_ptr.value(),
																	 0,
																	 bn::sprite_items::enemy_hp_bar.tiles_item(),
																	 hitpoints, hitpoints);
	*/
}

void Enemy::setHitFlash()
{
	hit_flash_frames = GAME_OBJECT_MAX_HIT_FLASH_FRAMES;

    bn::sprite_palette_ptr sprite_palette = bn::sprite_palette_items::sprite_flash_palette.create_palette();

    if(sprite_ptr.has_value())
    {sprite_ptr->set_palette(sprite_palette);}
}

void Enemy::setHitFlash(int32 frames)
{
    if(frames > GAME_OBJECT_MAX_HIT_FLASH_FRAMES) 
    {frames = GAME_OBJECT_MAX_HIT_FLASH_FRAMES;}

    hit_flash_frames = frames;

    bn::sprite_palette_ptr sprite_palette = bn::sprite_palette_items::sprite_flash_palette.create_palette();
    
    if(sprite_ptr.has_value())
    {sprite_ptr->set_palette(sprite_palette);}
}

void Enemy::updateDeathState()
{
    if(animate_action_ptr->done())
    {
		is_dead = true;
		
		//object_request = ObjectRequest(SKULL_DROP, bn::fixed_point(x(), y()));
	}
}

void Enemy::playEnemyDeathAnim()
{
	bn::optional<bn::sprite_ptr> temp_sprite_ptr = bn::sprite_items::enemy_death.create_sprite(sprite_ptr->x().integer(), 
																				 			   sprite_ptr->y().integer());
	temp_sprite_ptr->set_camera(sprite_ptr->camera());
	temp_sprite_ptr->set_z_order(sprite_ptr->z_order());

	sprite_ptr->swap(temp_sprite_ptr.value());

	temp_sprite_ptr.reset();

	animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
																1,
																bn::sprite_items::enemy_death.tiles_item(),
																0, 0, 0, 1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6);
}

//////////////////////////////
// State Function Overrides //
//////////////////////////////

// None..

/////////////////////////
// Collision Overrides //
/////////////////////////

// Level Objects
void Enemy::resolveTilePassageCollision(GameObject& object)
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

void Enemy::resolveFallingPlatformWideCollision(GameObject& object) 
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

void Enemy::resolveFallingPlatformThinCollision(GameObject& object)
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

void Enemy::resolvePushBlockCollision(GameObject& object)
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
        	resolveCornerCollision(object.collider);

			// Push the block
			if(state == OBJECT_HITSTUN || state == OBJECT_DEATH)
			{object.applyHit(damage, rigidbody.normalized_dir.x().integer(), 0);}
		}
    }

	updateTestColliders();

	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}

void Enemy::resolvePushBlockMiniCollision(GameObject& object)
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
        	resolveCornerCollision(object.collider);

			// Push the block
			if(state == OBJECT_HITSTUN || state == OBJECT_DEATH)
			{object.applyHit(damage, rigidbody.normalized_dir.x().integer(), 0);}
		}
    }

	updateTestColliders();

	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}

void Enemy::resolveAutoPlatformCollision(GameObject& object)
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

void Enemy::resolveSmashBlockLargeCollision(GameObject& object)
{
	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

        // If there is still collision somehow, must be corner case //
        resolveCornerCollision(object.collider);

		// Smash the block
		if(state == OBJECT_HITSTUN || state == OBJECT_DEATH)
		{object.applyHit(damage, 0, 0);}
    }

	updateTestColliders();

	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}

void Enemy::resolveSmashBlockMiniCollision(GameObject& object)
{
	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

        // If there is still collision somehow, must be corner case //
        resolveCornerCollision(object.collider);

		// Smash the block
		if(state == OBJECT_HITSTUN || state == OBJECT_DEATH)
		{object.applyHit(damage, 0, 0);}
    }

	updateTestColliders();

	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}

void Enemy::resolveSmashBlockZigguratLCollision(GameObject& object)
{
	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

        // If there is still collision somehow, must be corner case //
        resolveCornerCollision(object.collider);

		// Smash the block
		if(state == OBJECT_HITSTUN || state == OBJECT_DEATH)
		{object.applyHit(damage, 0, 0);}
    }

	updateTestColliders();

	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}

void Enemy::resolveSmashBlockZigguratCCollision(GameObject& object)
{
	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

        // If there is still collision somehow, must be corner case //
        resolveCornerCollision(object.collider);

		// Smash the block
		if(state == OBJECT_HITSTUN || state == OBJECT_DEATH)
		{object.applyHit(damage, 0, 0);}
    }

	updateTestColliders();

	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}

void Enemy::resolveSmashBlockZigguratRCollision(GameObject& object)
{
	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

        // If there is still collision somehow, must be corner case //
        resolveCornerCollision(object.collider);

		// Smash the block
		if(state == OBJECT_HITSTUN || state == OBJECT_DEATH)
		{object.applyHit(damage, 0, 0);}
    }

	updateTestColliders();

	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}

void Enemy::resolveLargeVaseCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
		// Smash the block
		if(state == OBJECT_HITSTUN || state == OBJECT_DEATH)
		{object.applyHit(damage, 0, 0);}
    }
}

void Enemy::resolveSmallVaseCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
		// Smash the block
		if(state == OBJECT_HITSTUN || state == OBJECT_DEATH)
		{object.applyHit(damage, 0, 0);}
    }
}

void Enemy::resolveHPTotemCollision(GameObject& object)
{
	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

        // If there is still collision somehow, must be corner case //
        resolveCornerCollision(object.collider);

		// Smash the block
		if(state == OBJECT_HITSTUN || state == OBJECT_DEATH)
		{object.applyHit(damage, 0, 0);}
    }

	updateTestColliders();

	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}

void Enemy::resolveFinishSealCollision(GameObject& object)
{
	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

        // If there is still collision somehow, must be corner case //
        resolveCornerCollision(object.collider);

		// Smash the block
		if(state == OBJECT_HITSTUN || state == OBJECT_DEATH)
		{object.applyHit(damage, 0, 0);}
    }

	updateTestColliders();

	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}

void Enemy::resolveBounceBellCollision(GameObject& object)
{
	if(collider.isCollision(object.collider))
    {
		if(state == OBJECT_HITSTUN)
		{
			object.applyHit(damage, 0, 0);
			state = IDLE;
		}
    }
}

void Enemy::resolveAutoBounceBellCollision(GameObject& object)
{
	if(collider.isCollision(object.collider))
    {
		if(state == OBJECT_HITSTUN)
		{
			object.applyHit(damage, 0, 0);
			state = IDLE;
		}
    }
}


// Level Enemies
void Enemy::resolveThornColumnCollision(GameObject& object)
{
	int32 thorn_collision_x_offset = collider.getCollisionXOffset(object.collider, rigidbody.normalized_dir.x()).integer();
	
	if(thorn_collision_x_offset != 0 &&
	   hitpoints > 0)
	{
		if(hitpoints > 0)
		{
			int32 knockback_x_dir = abs(thorn_collision_x_offset) / thorn_collision_x_offset;
			GameObject::applyHit(object.damage, knockback_x_dir, 0);
		}

		resolveXAxisCollision(object.collider);
		resolveYAxisCollision(object.collider);
	}
}

void Enemy::resolveThornBarCollision(GameObject& object)
{
	if(collider.isCollision(object.collider))
	{
		if(hitpoints > 0) {GameObject::applyHit(object.damage, 0, 0);}

		resolveXAxisCollision(object.collider);
		resolveYAxisCollision(object.collider);
	}

	updateTestColliders();

	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}

void Enemy::resolveGroundGhoulCollision(GameObject& object)
{	
	if(collider.isCollision(object.collider) &&
       (object.state == OBJECT_HITSTUN || object.state == OBJECT_DEATH))
	{
		applyHit(object.damage, object.rigidbody.normalized_dir.x().integer(), 0);
	}
}

void Enemy::resolveBellTrollCollision(GameObject& object)
{
	if(object.state == OBJECT_HITSTUN || object.state == OBJECT_DEATH) {return;}
	
	switch(object.state)
	{
		case BELL_TROLL_FROZEN:

			if(collider.isCollision(object.collider))
			{
				// Resolve X Axis Collision //
				resolveXAxisCollision(object.collider);

				// Resolve Y Axis Collision //
				resolveYAxisCollision(object.collider);

				// Resolve Corner Collision //
				if(col_y_offset == 0 && col_x_offset == 0)
				{resolveCornerCollision(object.collider);}

				// Smash the block
				if(state == OBJECT_HITSTUN || state == OBJECT_DEATH)
				{object.applyHit(damage, 0, 0);}
			}

			updateTestColliders();

			if(test_collider.isCollision(object.collider) && 
			rigidbody.normalized_dir.y() >= 0)
			{
				grounded_detected = true;
				rigidbody.removeYForces();
			}

		break;

		default:

			if(collider.isCollision(object.collider) &&
			(object.state == OBJECT_HITSTUN || object.state == OBJECT_DEATH))
			{
				applyHit(object.damage, object.rigidbody.normalized_dir.x().integer(), 0);
			}
			
		break;
	}
}

// Tiles
void Enemy::resolveXAxisCollision(const Collider& other_collider)
{
	GameObject::resolveXAxisCollision(other_collider);
	
	// Wall splat check
	if(state != OBJECT_DEATH)
	{wallSplatCheck();}
}
