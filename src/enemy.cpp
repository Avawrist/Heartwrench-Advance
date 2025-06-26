#include "enemy.h"

//////////////////
// Struct Enemy //
//////////////////

Enemy::Enemy()
{
	damage = ENEMY_DAMAGE;
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
	   state == OBJECT_HITSTUN &&
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

// None..

//////////////////////////////
// State Function Overrides //
//////////////////////////////

// Get State From Objects

void Enemy::getStateFromTilePassage(GameObject& object)
{
	// Test for, and log grounded collision
	if(object.state == TILE_PASSAGE_SHUT &&
		test_collider.isCollision(object.collider) && 
		rigidbody.normalized_dir.y() >= 0)
	{grounded_detected = true;}
}

void Enemy::getStateFromFallingPlatformWide(GameObject& object)
{
	if(rigidbody.normalized_dir.y() >= 0 &&
		collider_y_axis.p4.y() <= object.collider.p1.y() + rigidbody.final_dir.y())
	{
		// Test for, and log grounded collision
		if(test_collider.isCollision(object.collider) &&
			rigidbody.normalized_dir.y() >= 0)
		{
			grounded_detected = true;
			rigidbody.removeYForces();
		}
	}
}

void Enemy::getStateFromFallingPlatformThin(GameObject& object)
{
	if(rigidbody.normalized_dir.y() >= 0 &&
		collider_y_axis.p4.y() <= object.collider.p1.y() + rigidbody.final_dir.y())
	{
		// Test for, and log grounded collision
		if(test_collider.isCollision(object.collider) &&
			rigidbody.normalized_dir.y() >= 0)
		{
			grounded_detected = true;
			rigidbody.removeYForces();
		}
	}
}

void Enemy::getStateFromPushBlock(GameObject& object)
{
	// Test for, and log grounded collision
	if(test_collider.isCollision(object.collider) && 
		rigidbody.normalized_dir.y() >= 0)
	{grounded_detected = true;}
}

void Enemy::getStateFromPushBlockMini(GameObject& object)
{
	// Test for, and log grounded collision
	if(test_collider.isCollision(object.collider) && 
		rigidbody.normalized_dir.y() >= 0)
	{grounded_detected = true;}
}

void Enemy::getStateFromAutoPlatform(GameObject& object)
{
	if(rigidbody.normalized_dir.y() >= 0 &&
	   collider_y_axis.p4.y() <= object.collider.p1.y() + rigidbody.final_dir.y())
	{
		// Test for, and log grounded collision
		if(test_collider.isCollision(object.collider) &&
			rigidbody.normalized_dir.y() >= 0)
		{
			grounded_detected = true;
			rigidbody.removeYForces();
		}
	}
}

// Get State From Tiles

void Enemy::getStateFromHardBlock(int32 world_x, int32 world_y)
{
	Collider other_collider = Collider(world_x,
								       world_y, 
									   TILE_WIDTH,
									   TILE_HEIGHT);

	// Test for, and log grounded collision
	if(test_collider.isCollision(other_collider) &&
		rigidbody.normalized_dir.y() >= 0)
	{
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}

void Enemy::getStateFromLeftShallowSlope1(int32 world_x, int32 world_y)
{
	Collider other_collider = Collider(world_x,
							           world_y + 3, 
									   TILE_WIDTH, 
									   TILE_HEIGHT / 4);

	int32 index = abs(other_collider.p1.x() - collider.p4.x()).integer();
	index = clamp(0, 7, index);

	int32 local_height  = left_shallow_slope_1_arr[index];
	int32 global_height = world_y + (TILE_HEIGHT / 2) - local_height;

	// Test for, and log grounded collision
	if(test_collider.isCollision(other_collider) &&
	   test_collider.p4.y() >= global_height)
	{grounded_detected = true;}
}

void Enemy::getStateFromLeftShallowSlope2(int32 world_x, int32 world_y)
{
	Collider other_collider = Collider(world_x,
								       world_y + 2, 
									   TILE_WIDTH, 
									   TILE_HEIGHT / 2);

	int32 index = abs(other_collider.p1.x() - collider.p4.x()).integer();
	index = clamp(0, 7, index);

	int32 local_height  = left_shallow_slope_2_arr[index];
	int32 global_height = world_y + (TILE_HEIGHT / 2) - local_height;

	// Test for, and log grounded collision
	if(test_collider.isCollision(other_collider) &&
		test_collider.p4.y() >= global_height)
	{grounded_detected = true;}
}

void Enemy::getStateFromLeftShallowSlope3(int32 world_x, int32 world_y)
{
	Collider other_collider = Collider(world_x,
								       world_y + 1, 
								       TILE_WIDTH, 
								       TILE_HEIGHT - 2);

	int32 index = abs(other_collider.p1.x() - collider.p4.x()).integer();
	index = clamp(0, 7, index);
	int32 local_height  = left_shallow_slope_3_arr[index];
	int32 global_height = world_y + (TILE_HEIGHT / 2) - local_height;

	// Test for, and log grounded collision
	if(test_collider.isCollision(other_collider) &&
		test_collider.p4.y() >= global_height)
	{grounded_detected = true;}
}

void Enemy::getStateFromLeftShallowSlope4(int32 world_x, int32 world_y)
{
	Collider other_collider = Collider(world_x,
									   world_y, 
									   TILE_WIDTH, 
									   TILE_HEIGHT);

	int32 index = abs(other_collider.p1.x() - collider.p4.x()).integer();
	index = clamp(0, 7, index);

	int32 local_height  = left_shallow_slope_4_arr[index];
	int32 global_height = world_y + (TILE_HEIGHT / 2) - local_height;

	// Test for, and log grounded collision
	if(test_collider.isCollision(other_collider) &&
	   test_collider.p4.y() >= global_height)
	{grounded_detected = true;}
}

void Enemy::getStateFromLeftSteepSlope1(int32 world_x, int32 world_y)
{
	Collider other_collider = Collider(world_x,
									   world_y + 2, 
									   TILE_WIDTH, 
									   TILE_HEIGHT / 2);

	int32 index = abs(other_collider.p1.x() - collider.p4.x()).integer();
	index = clamp(0, 7, index);

	int32 local_height  = left_steep_slope_1_arr[index];
	int32 global_height = world_y + (TILE_HEIGHT / 2) - local_height;

	// Test for, and log grounded collision
	if(test_collider.isCollision(other_collider) &&
		test_collider.p4.y() >= global_height)
	{grounded_detected = true;}
}

void Enemy::getStateFromLeftSteepSlope2(int32 world_x, int32 world_y)
{
	Collider other_collider = Collider(world_x,
								       world_y, 
								       TILE_WIDTH, 
								       TILE_HEIGHT);

	int32 index = abs(other_collider.p1.x() - collider.p4.x()).integer();
	index = clamp(0, 7, index);

	int32 local_height  = left_steep_slope_2_arr[index];
	int32 global_height = world_y + (TILE_HEIGHT / 2) - local_height;

	// Test for, and log grounded collision
	if(test_collider.isCollision(other_collider) &&
		test_collider.p4.y() >= global_height)
	{grounded_detected = true;}
}

void Enemy::getStateFromRightShallowSlope1(int32 world_x, int32 world_y)
{
	Collider other_collider = Collider(world_x,
								       world_y + 3, 
								       TILE_WIDTH, 
								       TILE_HEIGHT / 4);

	int32 index = (collider.p1.x() - other_collider.p1.x()).integer();
	index = clamp(0, 7, index);

	int32 local_height  = right_shallow_slope_1_arr[index];
	int32 global_height = world_y + (TILE_HEIGHT / 2) - local_height;

	// Test for, and log grounded collision
	if(test_collider.isCollision(other_collider) &&
		collider.p1.y() + collider.height > global_height)
	{grounded_detected = true;}
}

void Enemy::getStateFromRightShallowSlope2(int32 world_x, int32 world_y)
{
	Collider other_collider = Collider(world_x,
								       world_y + 2, 
									   TILE_WIDTH, 
									   TILE_HEIGHT / 2);

	int32 index = (collider.p1.x() - other_collider.p1.x()).integer();
	index = clamp(0, 7, index);

	int32 local_height  = right_shallow_slope_2_arr[index];
	int32 global_height = world_y + (TILE_HEIGHT / 2) - local_height;

	// Test for, and log grounded collision
	if(test_collider.isCollision(other_collider) &&
		collider.p1.y() + collider.height > global_height)
	{grounded_detected = true;}
}

void Enemy::getStateFromRightShallowSlope3(int32 world_x, int32 world_y)
{
	Collider other_collider = Collider(world_x,
									   world_y + 1, 
									   TILE_WIDTH, 
									   TILE_HEIGHT - 2);

	int32 index = (collider.p1.x() - other_collider.p1.x()).integer();
	index = clamp(0, 7, index);

	int32 local_height  = right_shallow_slope_3_arr[index];
	int32 global_height = world_y + (TILE_HEIGHT / 2) - local_height;

	// Test for, and log grounded collision
	if(test_collider.isCollision(other_collider) &&
		collider.p1.y() + collider.height > global_height)
	{grounded_detected = true;}
}

void Enemy::getStateFromRightShallowSlope4(int32 world_x, int32 world_y)
{
	Collider other_collider = Collider(world_x,
									   world_y, 
									   TILE_WIDTH, 
									   TILE_HEIGHT);

	int32 index = (collider.p1.x() - other_collider.p1.x()).integer();
	index = clamp(0, 7, index);

	int32 local_height  = right_shallow_slope_4_arr[index];
	int32 global_height = world_y + (TILE_HEIGHT / 2) - local_height;

	// Test for, and log grounded collision
	if(test_collider.isCollision(other_collider) &&
		collider.p1.y() + collider.height > global_height)
	{grounded_detected = true;}
}

void Enemy::getStateFromRightSteepSlope1(int32 world_x, int32 world_y)
{
	Collider other_collider = Collider(world_x,
									   world_y + 2, 
									   TILE_WIDTH, 
									   TILE_HEIGHT / 2);

	int32 index = (collider.p1.x() - other_collider.p1.x()).integer();
	index = clamp(0, 7, index);

	int32 local_height  = right_steep_slope_1_arr[index];
	int32 global_height = world_y + (TILE_HEIGHT / 2) - local_height;

	// Test for, and log grounded collision
	if(test_collider.isCollision(other_collider) &&
		collider.p1.y() + collider.height > global_height)
	{grounded_detected = true;}
}

void Enemy::getStateFromRightSteepSlope2(int32 world_x, int32 world_y)
{
	Collider other_collider = Collider(world_x,
									   world_y, 
									   TILE_WIDTH, 
									   TILE_HEIGHT);

	int32 index = (collider.p1.x() - other_collider.p1.x()).integer();
	index = clamp(0, 7, index);

	int32 local_height  = right_steep_slope_2_arr[index];
	int32 global_height = world_y + (TILE_HEIGHT / 2) - local_height;

	// Test for, and log grounded collision
	if(test_collider.isCollision(other_collider) &&
		collider.p1.y() + collider.height > global_height)
	{grounded_detected = true;}
}

void Enemy::getStateFromOneWayBlock(int32 world_x, int32 world_y)
{
	Collider other_collider = Collider(world_x, 
									   world_y + ONEWAYBLOCK_COLLIDER_Y_OFFSET, 
									   TILE_WIDTH, 
									   ONEWAYBLOCK_COLLIDER_HEIGHT);

	if(rigidbody.normalized_dir.y() >= 0 &&
		collider_y_axis.p4.y() <= other_collider.p1.y() + rigidbody.final_dir.y())
	{
		// Test for, and log grounded collision
		if(test_collider.isCollision(other_collider) &&
			rigidbody.normalized_dir.y() >= 0)
		{
			grounded_detected = true;
			rigidbody.removeYForces();
		}
	}
}

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
	}
}

void Enemy::resolvePushBlockCollision(GameObject& object)
{
	if(collider.isCollision(object.collider))
    {
		if(object.rigidbody.normalized_dir.x() != 0 || 
           object.rigidbody.normalized_dir.y() != 0)
        {
            hitpoints = 0;   
        }
		else
		{
			// Resolve X Axis Collision //
        	resolveXAxisCollision(object.collider);

        	// Resolve Y Axis Collision //
        	resolveYAxisCollision(object.collider);

        	// If there is still collision somehow, must be corner case //
        	resolveCornerCollision(object.collider);
		}
    }
}

void Enemy::resolvePushBlockMiniCollision(GameObject& object)
{
	if(collider.isCollision(object.collider))
    {
		if(object.rigidbody.normalized_dir.x() != 0 || 
           object.rigidbody.normalized_dir.y() != 0)
        {
            hitpoints = 0;   
        }
		else
		{
			// Resolve X Axis Collision //
        	resolveXAxisCollision(object.collider);

        	// Resolve Y Axis Collision //
        	resolveYAxisCollision(object.collider);

        	// If there is still collision somehow, must be corner case //
        	resolveCornerCollision(object.collider);
		}
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
		applyHit(knockback_x_dir, 0, object.damage);
	}
}

void Enemy::resolveThornBarCollision(GameObject& object)
{
	resolveXAxisCollision(object.collider);
	resolveYAxisCollision(object.collider);

	if(collider.isCollision(object.collider) &&
	   hitpoints > 0)
	{
		applyHit(0, rigidbody.normalized_dir.y().integer() * -1, object.damage);
	}
}

// Tiles
void Enemy::resolveXAxisCollision(const Collider& other_collider)
{
	GameObject::resolveXAxisCollision(other_collider);
	
	// Wall splat check
	wallSplatCheck();
}
