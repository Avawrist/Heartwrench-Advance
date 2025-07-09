#include "enemy.h"

//////////////////
// Struct Enemy //
//////////////////

Enemy::Enemy()
{
	damage = ENEMY_DAMAGE;

	hp_sprite_ptr->set_visible(true);
}

Enemy::Enemy(const Enemy& other) : GameObject(other)
{

}

Enemy::~Enemy()
{

}

void Enemy::wallSplatCheck()
{
	if(col_x_offset != 0 &&
	   (state == OBJECT_HITSTUN || state == OBJECT_DEATH) &&
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

void Enemy::updateHPBar()
{
	if(hitpoints > ENEMY_MAX_HP) {return;}

	hp_sprite_ptr->set_position(x() + ENEMY_HP_BAR_X_OFFSET, y() + ENEMY_HP_BAR_Y_OFFSET);
	hp_animate_action_ptr = bn::create_sprite_animate_action_forever(hp_sprite_ptr.value(),
																	 0,
																	 bn::sprite_items::enemy_hp_bar.tiles_item(),
																	 hitpoints, hitpoints);
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

	// Test for, and log grounded collision
	if(test_collider.isCollision(object.collider) && 
		rigidbody.normalized_dir.y() >= 0)
	{grounded_detected = true;}
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

	// Test for, and log grounded collision
	if(test_collider.isCollision(object.collider) && 
		rigidbody.normalized_dir.y() >= 0)
	{grounded_detected = true;}
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

void Enemy::resolveSmashBlockMiniCollision(GameObject& object)
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
void Enemy::resolveThornColumnCollision(GameObject& object)
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

void Enemy::resolveThornBarCollision(GameObject& object)
{
	resolveXAxisCollision(object.collider);
	resolveYAxisCollision(object.collider);

	if(collider.isCollision(object.collider) &&
	   hitpoints > 0)
	{
		GameObject::applyHit(object.damage, 0, 0);
	}
}

// Tiles
void Enemy::resolveXAxisCollision(const Collider& other_collider)
{
	GameObject::resolveXAxisCollision(other_collider);
	
	// Wall splat check
	wallSplatCheck();
}
