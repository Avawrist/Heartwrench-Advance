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

void Enemy::getStateFromObjects(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects)
{
	////////////////////////////////////////
	// Initialize State Testing Variables //
	////////////////////////////////////////

	int32    ground_ray_length = 1;
	Collider test_collider     = Collider(collider.x(), 
	                                      collider.y() + ground_ray_length, 
										  collider.width, 
										  collider.height);

	// Placeholder for other objects
	Collider other_collider;

	/////////////////////////////////////
	// Get State Info from GameObjects //
	/////////////////////////////////////

	for(int32 i = 0; i < game_objects.size(); i++)
	{
		if(game_objects.at(i)->object_id != object_id)
		{
			other_collider = game_objects.at(i)->collider;

			switch(game_objects.at(i)->object_type)
			{
				// Level Objects
				case TILE_PASSAGE:

					// Test for, and log grounded collision
					if(game_objects.at(i)->state == TILE_PASSAGE_SHUT &&
						test_collider.isCollision(other_collider) && 
						rigidbody.normalized_dir.y() >= 0)
					{grounded_detected = true;}

				break;

				case PHASE_ORB_UP:
				break;

				case PHASE_ORB_DOWN:
				break;

				case PHASE_ORB_LEFT:
				break;

				case PHASE_ORB_RIGHT:
				break;

				case FALLING_PLATFORM_WIDE:
					
					if(rigidbody.normalized_dir.y() >= 0 &&
						collider_y_axis.p4.y() <= game_objects.at(i)->collider.p1.y() + rigidbody.final_dir.y())
					{
						// Test for, and log grounded collision
						if(test_collider.isCollision(game_objects.at(i)->collider) &&
							rigidbody.normalized_dir.y() >= 0)
						{
							grounded_detected     = true;
							rigidbody.removeYForces();
							
							// Trigger the falling platform
							/*
							if(game_objects.at(i)->state != FALLING_PLATFORM_WIDE_FALLING)
							{
								setY(this->y() - 1); // One pixel adjustment to deal with 
														// object slipping off platform on frame 1
								game_objects.at(i)->setState(FALLING_PLATFORM_WIDE_FALLING);
							}
							*/
						}
					}

				break;

				case FALLING_PLATFORM_THIN:
					
					if(rigidbody.normalized_dir.y() >= 0 &&
						collider_y_axis.p4.y() <= game_objects.at(i)->collider.p1.y() + rigidbody.final_dir.y())
					{
						// Test for, and log grounded collision
						if(test_collider.isCollision(game_objects.at(i)->collider) &&
							rigidbody.normalized_dir.y() >= 0)
						{
							grounded_detected = true;
							rigidbody.removeYForces();
							
							// Trigger the falling platform
							/*
							if(game_objects.at(i)->state != FALLING_PLATFORM_THIN_FALLING)
							{
								setY(this->y() - 1); // One pixel adjustment to deal with 
													// object slipping off platform on frame 1
								game_objects.at(i)->setState(FALLING_PLATFORM_THIN_FALLING);
							}
							*/
						}
					}

				break;

				case PUSH_BLOCK:

					// Test for, and log grounded collision
					if(test_collider.isCollision(other_collider) && 
						rigidbody.normalized_dir.y() >= 0)
					{grounded_detected = true;}

				break;

				case AUTO_PLATFORM:

					if(rigidbody.normalized_dir.y() >= 0 &&
					   collider_y_axis.p4.y() <= game_objects.at(i)->collider.p1.y() + rigidbody.final_dir.y())
					{
						// Test for, and log grounded collision
						if(test_collider.isCollision(game_objects.at(i)->collider) &&
							rigidbody.normalized_dir.y() >= 0)
						{
							grounded_detected     = true;
							rigidbody.removeYForces();
						}
					}

				break;

				// Level Enemies
				case THORN_COLUMN:
				case THORN_BAR:
				case GROUND_GHOUL:
				break;

				default:
				break;
			}
		}
	}
}

void Enemy::getStateFromTiles(const bn::regular_bg_ptr&                      bg_ptr,
                           	  const bn::span<const bn::regular_bg_map_cell>& cells,
                              const bn::regular_bg_item&                     bg_item)
{
	
	////////////////////////////////////////
	// Initialize State Testing Variables //
	////////////////////////////////////////

	int32 index;
	int32 local_height;
	int32 global_height;

	int32    ground_ray_length = 1;
	Collider test_collider     = Collider(collider.x(), 
	                                      collider.y() + ground_ray_length,
										  collider.width, 
										  collider.height);

	// Get current cell index that player resides in:
	int32 half_level_width_pixels  = bg_ptr.dimensions().width() / 2;
	int32 half_level_height_pixels = bg_ptr.dimensions().height() / 2;
	bn::fixed index_x = (x() + half_level_width_pixels)  / TILE_WIDTH;
	bn::fixed index_y = (y() + half_level_height_pixels) / TILE_HEIGHT;
	bn::point cell_index = bn::point(index_x.integer(), index_y.integer());

	// Placeholder for other objects
	Collider other_collider;

	///////////////////////////////
    // Get State Info from Tiles //
    ///////////////////////////////	

	for(int32 y = -1; y < 2; y++)
	{
		for(int32 x = -1; x < 2; x++)
		{
			// 1. Get tile type at index //
			int32 check_index_x = cell_index.x() + x;
			int32 check_index_y = cell_index.y() + y;

			// Clamp index values so we don't crash by going out of bounds.
			while(check_index_x < 0) {check_index_x++;}
			while(check_index_x > (bg_ptr.dimensions().width() / 8) - 1)  {check_index_x--;}

			while(check_index_y < 0) {check_index_y++;}
			while(check_index_y > (bg_ptr.dimensions().height() / 8) - 1) {check_index_y--;}

			// Determine world coords in case we need to make a collider.
			int32 world_x = ((check_index_x * TILE_WIDTH)  - half_level_width_pixels)  + (TILE_WIDTH / 2);
			int32 world_y = ((check_index_y * TILE_HEIGHT) - half_level_height_pixels) + (TILE_HEIGHT / 2);

			uint32 tile_index = getTileAtBGIndex(check_index_x, check_index_y, 
													bg_ptr, cells, bg_item);

			// 2. Check Tile Type and update state accordingly //
			if(tile_index >= HARD_BLOCK_MIN_INDEX &&
			   tile_index <= HARD_BLOCK_MAX_INDEX)
			{

				other_collider = Collider(world_x,
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
					
			else if(tile_index == UP_SPIKE_BLOCK_1_INDEX ||
					tile_index == UP_SPIKE_BLOCK_2_INDEX)
			{
				other_collider = Collider(world_x,
											world_y, 
											TILE_WIDTH, 
											TILE_HEIGHT);
				
				resolveUpSpikeCollision(other_collider);
			}

			else if(tile_index == DOWN_SPIKE_BLOCK_1_INDEX ||
					tile_index == DOWN_SPIKE_BLOCK_2_INDEX)
			{
				other_collider = Collider(world_x,
										world_y, 
										TILE_WIDTH, 
										TILE_HEIGHT);
				
				resolveDownSpikeCollision(other_collider);
			}

			else if(tile_index == LEFT_SPIKE_BLOCK_1_INDEX ||
					tile_index == LEFT_SPIKE_BLOCK_2_INDEX)
			{
				other_collider = Collider(world_x,
											world_y, 
											TILE_WIDTH, 
											TILE_HEIGHT);
				
				resolveLeftSpikeCollision(other_collider);
			}

			else if(tile_index == RIGHT_SPIKE_BLOCK_1_INDEX || 
					tile_index == RIGHT_SPIKE_BLOCK_2_INDEX)
			{
				other_collider = Collider(world_x,
											world_y, 
											TILE_WIDTH,
											TILE_HEIGHT);
				
				resolveRightSpikeCollision(other_collider);
			}

			else if(tile_index == LEFT_SHALLOW_SLOPE_1_INDEX)
			{
				other_collider = Collider(world_x,
										world_y + 3, 
										TILE_WIDTH, 
										TILE_HEIGHT / 4);

				index = abs(other_collider.p1.x() - collider.p4.x()).integer();
				index = clamp(0, 7, index);
				local_height  = left_shallow_slope_1_arr[index];
				global_height = world_y + (TILE_HEIGHT / 2) - local_height;

				// Test for, and log grounded collision
				if(test_collider.isCollision(other_collider) &&
					test_collider.p4.y() >= global_height)
				{
					grounded_detected = true;
				}
			}

			else if(tile_index == LEFT_SHALLOW_SLOPE_2_INDEX)
			{					
				other_collider = Collider(world_x,
											world_y + 2, 
											TILE_WIDTH, 
											TILE_HEIGHT / 2);

				index = abs(other_collider.p1.x() - collider.p4.x()).integer();
				index = clamp(0, 7, index);
				local_height  = left_shallow_slope_2_arr[index];
				global_height = world_y + (TILE_HEIGHT / 2) - local_height;

				// Test for, and log grounded collision
				if(test_collider.isCollision(other_collider) &&
					test_collider.p4.y() >= global_height)
				{
					grounded_detected = true;
				}
			}

			else if(tile_index == LEFT_SHALLOW_SLOPE_3_INDEX)
			{
				other_collider = Collider(world_x,
											world_y + 1, 
											TILE_WIDTH, 
											TILE_HEIGHT - 2);

				index = abs(other_collider.p1.x() - collider.p4.x()).integer();
				index = clamp(0, 7, index);
				local_height  = left_shallow_slope_3_arr[index];
				global_height = world_y + (TILE_HEIGHT / 2) - local_height;

				// Test for, and log grounded collision
				if(test_collider.isCollision(other_collider) &&
					test_collider.p4.y() >= global_height)
				{
					grounded_detected = true;
				}
			}

			else if(tile_index == LEFT_SHALLOW_SLOPE_4_INDEX)
			{					
				other_collider = Collider(world_x,
											world_y, 
											TILE_WIDTH, 
											TILE_HEIGHT);

				index = abs(other_collider.p1.x() - collider.p4.x()).integer();
				index = clamp(0, 7, index);
				local_height  = left_shallow_slope_4_arr[index];
				global_height = world_y + (TILE_HEIGHT / 2) - local_height;

				// Test for, and log grounded collision
				if(test_collider.isCollision(other_collider) &&
					test_collider.p4.y() >= global_height)
				{
					grounded_detected = true;
				}
			}
			
			else if(tile_index == LEFT_STEEP_SLOPE_1_INDEX)
			{
				other_collider = Collider(world_x,
											world_y + 2, 
											TILE_WIDTH, 
											TILE_HEIGHT / 2);

				index = abs(other_collider.p1.x() - collider.p4.x()).integer();
				index = clamp(0, 7, index);
				local_height  = left_steep_slope_1_arr[index];
				global_height = world_y + (TILE_HEIGHT / 2) - local_height;

				// Test for, and log grounded collision
				if(test_collider.isCollision(other_collider) &&
					test_collider.p4.y() >= global_height)
				{
					grounded_detected = true;
				}
			}
			
			else if(tile_index == LEFT_STEEP_SLOPE_2_INDEX)
			{	
				other_collider = Collider(world_x,
											world_y, 
											TILE_WIDTH, 
											TILE_HEIGHT);

				index = abs(other_collider.p1.x() - collider.p4.x()).integer();
				index = clamp(0, 7, index);
				local_height  = left_steep_slope_2_arr[index];
				global_height = world_y + (TILE_HEIGHT / 2) - local_height;

				// Test for, and log grounded collision
				if(test_collider.isCollision(other_collider) &&
					test_collider.p4.y() >= global_height)
				{
					grounded_detected = true;
				}
			}				

			else if(tile_index == RIGHT_SHALLOW_SLOPE_1_INDEX)
			{
				other_collider = Collider(world_x,
											world_y + 3, 
											TILE_WIDTH, 
											TILE_HEIGHT / 4);

				index = (collider.p1.x() - other_collider.p1.x()).integer();
				index = clamp(0, 7, index);
				local_height  = right_shallow_slope_1_arr[index];
				global_height = world_y + (TILE_HEIGHT / 2) - local_height;

				// Test for, and log grounded collision
				if(test_collider.isCollision(other_collider) &&
					collider.p1.y() + collider.height > global_height)
				{
					grounded_detected = true;
				}
			}

			else if(tile_index == RIGHT_SHALLOW_SLOPE_2_INDEX)
			{
				other_collider = Collider(world_x,
											world_y + 2, 
											TILE_WIDTH, 
											TILE_HEIGHT / 2);

				index = (collider.p1.x() - other_collider.p1.x()).integer();
				index = clamp(0, 7, index);
				local_height  = right_shallow_slope_2_arr[index];
				global_height = world_y + (TILE_HEIGHT / 2) - local_height;

				// Test for, and log grounded collision
				if(test_collider.isCollision(other_collider) &&
					collider.p1.y() + collider.height > global_height)
				{
					grounded_detected = true;
				}
			}

			else if(tile_index == RIGHT_SHALLOW_SLOPE_3_INDEX)
			{
				other_collider = Collider(world_x,
											world_y + 1, 
											TILE_WIDTH, 
											TILE_HEIGHT - 2);

				index = (collider.p1.x() - other_collider.p1.x()).integer();
				index = clamp(0, 7, index);
				local_height  = right_shallow_slope_3_arr[index];
				global_height = world_y + (TILE_HEIGHT / 2) - local_height;

				// Test for, and log grounded collision
				if(test_collider.isCollision(other_collider) &&
					collider.p1.y() + collider.height > global_height)
				{
					grounded_detected = true;
				}
			}

			else if(tile_index == RIGHT_SHALLOW_SLOPE_4_INDEX)
			{
				other_collider = Collider(world_x,
											world_y, 
											TILE_WIDTH, 
											TILE_HEIGHT);

				index = (collider.p1.x() - other_collider.p1.x()).integer();
				index = clamp(0, 7, index);
				local_height  = right_shallow_slope_4_arr[index];
				global_height = world_y + (TILE_HEIGHT / 2) - local_height;

				// Test for, and log grounded collision
				if(test_collider.isCollision(other_collider) &&
					collider.p1.y() + collider.height > global_height)
				{
					grounded_detected = true;
				}
			}

			else if(tile_index == RIGHT_STEEP_SLOPE_1_INDEX)
			{
				other_collider = Collider(world_x,
											world_y + 2, 
											TILE_WIDTH, 
											TILE_HEIGHT / 2);

				index = (collider.p1.x() - other_collider.p1.x()).integer();
				index = clamp(0, 7, index);
				local_height  = right_steep_slope_1_arr[index];
				global_height = world_y + (TILE_HEIGHT / 2) - local_height;

				// Test for, and log grounded collision
				if(test_collider.isCollision(other_collider) &&
					collider.p1.y() + collider.height > global_height)
				{
					grounded_detected = true;
				}
			}

			else if(tile_index == RIGHT_STEEP_SLOPE_2_INDEX)
			{
				other_collider = Collider(world_x,
											world_y, 
											TILE_WIDTH, 
											TILE_HEIGHT);

				index = (collider.p1.x() - other_collider.p1.x()).integer();
				index = clamp(0, 7, index);
				local_height  = right_steep_slope_2_arr[index];
				global_height = world_y + (TILE_HEIGHT / 2) - local_height;

				// Test for, and log grounded collision
				if(test_collider.isCollision(other_collider) &&
					collider.p1.y() + collider.height > global_height)
				{
					grounded_detected = true;
				}
			}	

			else if(tile_index >= ONEWAY_BLOCK_MIN_INDEX &&
					tile_index <= ONEWAY_BLOCK_MAX_INDEX)
			{
			
				other_collider = Collider(world_x, 
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
