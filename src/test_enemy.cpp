#include "test_enemy.h"

TestEnemy::TestEnemy()
{

    // Reset Variables //
    sprite_ptr.reset();
    animate_action_ptr.reset();

    // Init Variables //
	object_type = TEST_ENEMY;
	dir         = RIGHT;
    sprite_ptr  = bn::sprite_items::test_enemy.create_sprite(0, 0);
    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  2,
								  								  bn::sprite_items::test_enemy.tiles_item(),
								  								  0,
								  								  0);

	collider          = Collider(x(), y(), TEST_ENEMY_COLLIDER_WIDTH, TEST_ENEMY_COLLIDER_HEIGHT);
	collider_x_axis   = collider;
	collider_y_axis   = collider;
	collider_offset_x = 0;
	collider_offset_y = 0;
    
}

TestEnemy::TestEnemy(const TestEnemy& other) : GameObject(other)
{

}

TestEnemy::~TestEnemy()
{

}

void TestEnemy::update(RoomBounds                      room_bounds,
			Collider**                                 tile_colliders,
            bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects,
            bn::regular_bg_ptr                         bg_ptr, 
            bn::span<const bn::regular_bg_map_cell>    cells,
            bn::regular_bg_item                        bg_item,
            bn::camera_ptr                             camera)
{
    ///////////////////
    // State Machine //
    ///////////////////

    rigidbody.addForce(TEST_ENEMY_GRAVITY_FORCE);

    ////////////////////
    // Update Physics //
    ////////////////////

    // Apply Decay to Forces
	rigidbody.applyDecay();

	// Apply forces to enemy
	applyForces();

    //////////////////////////////
	// Init Collision Variables //
	//////////////////////////////

	// Get current cell index that player resides in:
	int32 half_level_width_pixels  = bg_ptr.dimensions().width() / 2;
	int32 half_level_height_pixels = bg_ptr.dimensions().height() / 2;
	bn::fixed index_x = (x() + half_level_width_pixels)  / TILE_WIDTH;
	bn::fixed index_y = (y() + half_level_height_pixels) / TILE_HEIGHT;
	bn::point cell_index = bn::point(index_x.integer(), index_y.integer());

	// Create one temporary collider for each axis. If a collider finds a collision
	// in its axis, move the temp collider AND the Player back along the dir vector
	// in units of 1 until the collision is resolved on that axis.

	collider_x_axis.setPos(collider.x(), collider.y() - rigidbody.final_dir.y());
	collider_y_axis.setPos(collider.x() - rigidbody.final_dir.x(), collider.y());

	Collider other_collider;

    ////////////////////////////
    // Resolve Tile Collision //
    ////////////////////////////
	
	for(int32 y = -1; y < 2; y++)
	{
		for(int32 x = -1; x < 2; x++)
		{
			// 1. Get tile type at index //
			int32 check_index_x = cell_index.x() + x;
			int32 check_index_y = cell_index.y() + y;

			// Determine world coords in case we need to make a collider.
			int32 world_x = ((check_index_x * TILE_WIDTH)  - half_level_width_pixels)  + (TILE_WIDTH / 2);
			int32 world_y = ((check_index_y * TILE_HEIGHT) - half_level_height_pixels) + (TILE_HEIGHT / 2);

			uint32 tile_index = getTileAtBGIndex(check_index_x, check_index_y, 
			                                     bg_ptr, cells, bg_item);

			// Prepare offsets in case they are needed for Block collision.
			int32 block_w_offset = 0;
			int32 block_x_offset = 0;

			int32 index;
			int32 local_height;
			int32 global_height;

			// 2. If the tile is collidable make a temporary collider
			switch(tile_index)
			{
				case HARD_BLOCK_INDEX:
				case SOFT_BLOCK_INDEX:

					// If the neighbor to the right is also a BLOCK, smooth over the corner.
					// This is a hack to resolve collision since checks are always made from
					// left to right. 
					
					if(getTileAtBGIndex(check_index_x + 1, check_index_y, 
									    bg_ptr, cells, bg_item) == HARD_BLOCK_INDEX || 
					   getTileAtBGIndex(check_index_x + 1, check_index_y, 
									    bg_ptr, cells, bg_item) == SOFT_BLOCK_INDEX)
					{
						block_w_offset = TILE_WIDTH;
						block_x_offset = TILE_WIDTH / 2;
						x++; // Skip checking the next cell, since we already accounted for it here.
					}

					other_collider = Collider(world_x + block_x_offset, 
											  world_y, 
											  TILE_WIDTH + block_w_offset, 
											  TILE_HEIGHT);
			
					if(collider.isCollision(other_collider))
					{
						// Handle Default Collision Cases //
						while(collider_x_axis.isCollision(other_collider))
						{
							collider_x_axis.setX(collider_x_axis.x() - rigidbody.normalized_dir.x());
							setX(this->x() - rigidbody.normalized_dir.x());
						}

						while(collider_y_axis.isCollision(other_collider))
						{
							collider_y_axis.setY(collider_y_axis.y() - rigidbody.normalized_dir.y());
							setY(this->y() - rigidbody.normalized_dir.y());
						}

						// If there is still collision somehow, must be corner case //
						while(collider.isCollision(other_collider))
						{
							// We always resolve diagonal corner collisions with a horizontal shift. 
							setX(this->x() - rigidbody.normalized_dir.x());
						}
					}

				break;

				case LEFT_SHALLOW_SLOPE_1_INDEX:

					other_collider = Collider(world_x, 
											  world_y + 3, 
											  TILE_WIDTH, 
											  TILE_HEIGHT / 4);

					if(collider.isCollision(other_collider))
					{
						// Derive slope height at player position:
						index = abs(other_collider.p1.x() - collider.p4.x()).integer();
						index = clamp(0, 7, index);
						local_height  = left_shallow_slope_1_arr[index];
						global_height = world_y + (TILE_HEIGHT / 2) - local_height;

						// Manually set player position:
						setY(global_height - (TEST_ENEMY_COLLIDER_HEIGHT / 2));
					}

				break;
				
				case LEFT_SHALLOW_SLOPE_2_INDEX:

					other_collider = Collider(world_x, 
											  world_y + 2, 
											  TILE_WIDTH, 
											  TILE_HEIGHT / 2);

					if(collider.isCollision(other_collider))
					{
						// Derive slope height at player position:
						index = abs(other_collider.p1.x() - collider.p4.x()).integer();
						index = clamp(0, 7, index);
						local_height  = left_shallow_slope_2_arr[index];
						global_height = world_y + (TILE_HEIGHT / 2) - local_height;

						// Manually set player position:
						setY(global_height - (TEST_ENEMY_COLLIDER_HEIGHT / 2));
					}

				break;

				case LEFT_SHALLOW_SLOPE_3_INDEX:

					other_collider = Collider(world_x, 
											  world_y + 1, 
											  TILE_WIDTH, 
											  TILE_HEIGHT - 2);

					if(collider.isCollision(other_collider))
					{
						// Derive slope height at player position:
						index = abs(other_collider.p1.x() - collider.p4.x()).integer();
						index = clamp(0, 7, index);
						local_height  = left_shallow_slope_3_arr[index];
						global_height = world_y + (TILE_HEIGHT / 2) - local_height;

						// Manually set player position:
						setY(global_height - (TEST_ENEMY_COLLIDER_HEIGHT / 2));
					}

				break;

				case LEFT_SHALLOW_SLOPE_4_INDEX:

					other_collider = Collider(world_x, 
											  world_y, 
											  TILE_WIDTH, 
											  TILE_HEIGHT);

					if(collider.isCollision(other_collider))
					{
						// Derive slope height at player position:
						index = abs(other_collider.p1.x() - collider.p4.x()).integer();
						index = clamp(0, 7, index);
						local_height  = left_shallow_slope_4_arr[index];
						global_height = world_y + (TILE_HEIGHT / 2) - local_height;

						// Manually set player position:
						setY(global_height - (TEST_ENEMY_COLLIDER_HEIGHT / 2));
					}

				break;

				case LEFT_STEEP_SLOPE_1_INDEX:

					other_collider = Collider(world_x, 
											  world_y + 2, 
											  TILE_WIDTH, 
											  TILE_HEIGHT / 2);

					if(collider.isCollision(other_collider))
					{
						// Derive slope height at player position:
						index = abs(other_collider.p1.x() - collider.p4.x()).integer();
						index = clamp(0, 7, index);
						local_height  = left_steep_slope_1_arr[index];
						global_height = world_y + (TILE_HEIGHT / 2) - local_height;

						// Manually set player position:
						setY(global_height - (TEST_ENEMY_COLLIDER_HEIGHT / 2));
					}

				break;

				case LEFT_STEEP_SLOPE_2_INDEX:

					other_collider = Collider(world_x, 
											  world_y, 
											  TILE_WIDTH, 
											  TILE_HEIGHT);

					if(collider.isCollision(other_collider))
					{
						// Derive slope height at player position:
						index = abs(other_collider.p1.x() - collider.p4.x()).integer();
						index = clamp(0, 7, index);
						local_height  = left_steep_slope_2_arr[index];
						global_height = world_y + (TILE_HEIGHT / 2) - local_height;

						// Manually set player position:
						setY(global_height - (TEST_ENEMY_COLLIDER_HEIGHT / 2));
					}

				break;

				case RIGHT_SHALLOW_SLOPE_1_INDEX:

					other_collider = Collider(world_x, 
											  world_y + 3, 
											  TILE_WIDTH, 
											  TILE_HEIGHT / 4);

					if(collider.isCollision(other_collider))
					{
						// Derive slope height at player position:
						index = (collider.p1.x() - other_collider.p1.x()).integer();
						index = clamp(0, 7, index);
						local_height  = right_shallow_slope_1_arr[index];
						global_height = world_y + (TILE_HEIGHT / 2) - local_height;

						// Manually set player position:
						setY(global_height - (TEST_ENEMY_COLLIDER_HEIGHT / 2));
					}

				break;

				case RIGHT_SHALLOW_SLOPE_2_INDEX:

					other_collider = Collider(world_x, 
											  world_y + 2, 
											  TILE_WIDTH, 
											  TILE_HEIGHT / 2);

					if(collider.isCollision(other_collider))
					{
						// Derive slope height at player position:
						index = (collider.p1.x() - other_collider.p1.x()).integer();
						index = clamp(0, 7, index);
						local_height = right_shallow_slope_2_arr[index];
						global_height = world_y + (TILE_HEIGHT / 2) - local_height;

						// Manually set player position:
						setY(global_height - (TEST_ENEMY_COLLIDER_HEIGHT / 2));
					}

				break;

				case RIGHT_SHALLOW_SLOPE_3_INDEX:

					other_collider = Collider(world_x, 
											  world_y + 1,
											  TILE_WIDTH, 
											  TILE_HEIGHT - 2);

					if(collider.isCollision(other_collider))
					{
						// Derive slope height at player position:
						index = (collider.p1.x() - other_collider.p1.x()).integer();
						index = clamp(0, 7, index);
						local_height  = right_shallow_slope_3_arr[index];
						global_height = world_y + (TILE_HEIGHT / 2) - local_height;

						// Manually set player position:
						setY(global_height - (TEST_ENEMY_COLLIDER_HEIGHT / 2));
					}
				
				break;

				case RIGHT_SHALLOW_SLOPE_4_INDEX:

					other_collider = Collider(world_x, 
											  world_y,
											  TILE_WIDTH, 
											  TILE_HEIGHT);

					if(collider.isCollision(other_collider))
					{
						// Derive slope height at player position:
						index = (collider.p1.x() - other_collider.p1.x()).integer();
						index = clamp(0, 7, index);
						local_height  = right_shallow_slope_4_arr[index];
						global_height = world_y + (TILE_HEIGHT / 2) - local_height;

						// Manually set player position:
						setY(global_height - (TEST_ENEMY_COLLIDER_HEIGHT / 2));
					}
				
				break;

				case RIGHT_STEEP_SLOPE_1_INDEX:

					other_collider = Collider(world_x, 
											  world_y + 2,
											  TILE_WIDTH, 
											  TILE_HEIGHT / 2);

					if(collider.isCollision(other_collider))
					{
						// Derive slope height at player position:
						index = (collider.p1.x() - other_collider.p1.x()).integer();
						index = clamp(0, 7, index);
						local_height  = right_steep_slope_1_arr[index];
						global_height = world_y + (TILE_HEIGHT / 2) - local_height;

						// Manually set player position:
						setY(global_height - (TEST_ENEMY_COLLIDER_HEIGHT / 2));
					}
				
				break;

				case RIGHT_STEEP_SLOPE_2_INDEX:

					other_collider = Collider(world_x, 
											  world_y,
											  TILE_WIDTH, 
											  TILE_HEIGHT);

					if(collider.isCollision(other_collider))
					{
						// Derive slope height at player position:
						index = (collider.p1.x() - other_collider.p1.x()).integer();
						index = clamp(0, 7, index);
						local_height  = right_steep_slope_2_arr[index];
						global_height = world_y + (TILE_HEIGHT / 2) - local_height;

						// Manually set player position:
						setY(global_height - (TEST_ENEMY_COLLIDER_HEIGHT / 2));
					}
				
				break;

				case ONEWAY_BLOCK_INDEX:

					other_collider = Collider(world_x, 
											  world_y + ONEWAYBLOCK_COLLIDER_Y_OFFSET, 
											  TILE_WIDTH, 
											  ONEWAYBLOCK_COLLIDER_HEIGHT);

					if(rigidbody.normalized_dir.y() >= 0 &&
					   collider_y_axis.p4.y() <= other_collider.p1.y() + TEST_ENEMY_GRAVITY)
					{

						// Handle Corner Case //
						if(!collider_x_axis.isCollision(other_collider) &&
						   !collider_y_axis.isCollision(other_collider))
						{
							while(collider.isCollision(other_collider))
							{
								setY(this->y() - 1);
							}
						}
					
						// Handle Remaining Collision Cases //
						else
						{
							while(collider_y_axis.isCollision(other_collider))
							{
								collider_y_axis.setY(collider_y_axis.y() - 1);
								setY(this->y() - 1);
							}
						}
					}

				break;

				default:
				break;
			}
		}
	}

}