#include "test_enemy.h"

TestEnemy::TestEnemy()
{

    // Reset Variables //
    sprite_ptr.reset();
    animate_action_ptr.reset();

    // Init Variables //
	object_type = TEST_ENEMY;
    sprite_ptr  = bn::sprite_items::test_enemy.create_sprite(0, 0);
	sprite_ptr->set_z_order(TEST_ENEMY_Z_ORDER);
    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  2,
								  								  bn::sprite_items::test_enemy.tiles_item(),
								  								  0,
								  								  0);

	collider          = Collider(x(), y(), TEST_ENEMY_COLLIDER_WIDTH, TEST_ENEMY_COLLIDER_HEIGHT);
	collider_x_axis   = collider;
	collider_y_axis   = collider;
	collider_offset_x = TEST_ENEMY_COLLIDER_OFFSET_X;
	collider_offset_y = TEST_ENEMY_COLLIDER_OFFSET_Y;
    
}

TestEnemy::TestEnemy(const TestEnemy& other) : GameObject(other)
{

}

TestEnemy::~TestEnemy()
{

}

TestEnemy& TestEnemy::operator =(const TestEnemy& other)
{
	return *this;
}

void TestEnemy::update(const RoomBounds&                              room_bounds,
            		   bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
            		   const bn::regular_bg_ptr&                      bg_ptr, 
            		   const bn::span<const bn::regular_bg_map_cell>& cells,
            		   const bn::regular_bg_item&                     bg_item,
            		   const bn::camera_ptr&                          camera)
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

	// Get current cell index that enemy resides in:
	int32 half_level_width_pixels  = (bg_ptr.dimensions().width() / 2);
	int32 half_level_height_pixels = (bg_ptr.dimensions().height() / 2);
	bn::fixed index_x = (x() + half_level_width_pixels + TEST_ENEMY_COLLIDER_OFFSET_X)  / TILE_WIDTH;
	bn::fixed index_y = (y() + half_level_height_pixels + TEST_ENEMY_COLLIDER_OFFSET_Y) / TILE_HEIGHT;
	bn::point cell_index = bn::point(index_x.integer(), index_y.integer());

	// Update colliders for each axis. 
	collider_x_axis.setPos(collider.x(), collider.y() - rigidbody.final_dir.y());
	collider_y_axis.setPos(collider.x() - rigidbody.final_dir.x(), collider.y());

	// Placeholder for other objects
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

			bn::fixed col_x_offset;
			bn::fixed col_y_offset;

			int32 index;
			int32 local_height;
			int32 global_height;

			// 2. If the tile is collidable make a temporary collider based on type//

			if(tile_index >= HARD_BLOCK_MIN_INDEX && 
			   tile_index <= HARD_BLOCK_MAX_INDEX)
			{
				// Prepare offsets in case they are needed for Block collision.
				int32 block_w_offset = 0;
				int32 block_x_offset = 0;

				// If the neighbor to the right is also a BLOCK, smooth over the corner.
				// This is a hack to resolve collision since checks are always made from
				// left to right. 
				if(getTileAtBGIndex(check_index_x + 1, check_index_y, 
									 bg_ptr, cells, bg_item) >= HARD_BLOCK_MIN_INDEX && 
					getTileAtBGIndex(check_index_x + 1, check_index_y, 
									 bg_ptr, cells, bg_item) <= HARD_BLOCK_MAX_INDEX)
				{
					block_w_offset = TILE_WIDTH;
					block_x_offset = TILE_WIDTH / 2;
					x++;
				}

				other_collider = Collider(world_x + block_x_offset, 
										  world_y, 
										  TILE_WIDTH + block_w_offset,
										  TILE_HEIGHT);

				if(collider.isCollision(other_collider))
				{
					// Resolve X Axis Collision //
					col_x_offset = collider_x_axis.getCollisionXOffset(other_collider, rigidbody.normalized_dir.x());
					collider_x_axis.setX(collider_x_axis.x() + col_x_offset);
					setX(this->x() + col_x_offset);

					// Resolve Y Axis Collision //
					col_y_offset = collider_y_axis.getCollisionYOffset(other_collider, rigidbody.normalized_dir.y());
					collider_y_axis.setY(collider_y_axis.y() + col_y_offset);
					setY(this->y() + col_y_offset);

					// If there is still collision somehow, must be corner case //
					while(collider.isCollision(other_collider))
					{
						// We always resolve diagonal corner collisions with a horizontal shift. 
						setX(this->x() - rigidbody.normalized_dir.x());
					}
				}
			}	
			
			else if(tile_index == LEFT_SHALLOW_SLOPE_1_INDEX)
			{
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
					setY(global_height - (TEST_ENEMY_COLLIDER_HEIGHT / 2) - TEST_ENEMY_COLLIDER_OFFSET_Y);
				}
			}
				
			else if(tile_index == LEFT_SHALLOW_SLOPE_2_INDEX)
			{
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
					setY(global_height - (TEST_ENEMY_COLLIDER_HEIGHT / 2) - TEST_ENEMY_COLLIDER_OFFSET_Y);
				}
			}

			else if(tile_index == LEFT_SHALLOW_SLOPE_3_INDEX)
			{
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
					setY(global_height - (TEST_ENEMY_COLLIDER_HEIGHT / 2) - TEST_ENEMY_COLLIDER_OFFSET_Y);
				}
			}

			else if(tile_index == LEFT_SHALLOW_SLOPE_4_INDEX)
			{
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
					setY(global_height - (TEST_ENEMY_COLLIDER_HEIGHT / 2) - TEST_ENEMY_COLLIDER_OFFSET_Y);
				}
			}

			else if(tile_index == LEFT_STEEP_SLOPE_1_INDEX)
			{
				
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
					setY(global_height - (TEST_ENEMY_COLLIDER_HEIGHT / 2) - TEST_ENEMY_COLLIDER_OFFSET_Y);
				}
			}

			else if(tile_index == LEFT_STEEP_SLOPE_2_INDEX)
			{
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
					setY(global_height - (TEST_ENEMY_COLLIDER_HEIGHT / 2) - TEST_ENEMY_COLLIDER_OFFSET_Y);
				}
			}

			else if(tile_index == RIGHT_SHALLOW_SLOPE_1_INDEX)
			{
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
					setY(global_height - (TEST_ENEMY_COLLIDER_HEIGHT / 2) - TEST_ENEMY_COLLIDER_OFFSET_Y);
				}
			}

			else if(tile_index == RIGHT_SHALLOW_SLOPE_2_INDEX)
			{
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
					setY(global_height - (TEST_ENEMY_COLLIDER_HEIGHT / 2) - TEST_ENEMY_COLLIDER_OFFSET_Y);
				}
			}

			else if(tile_index == RIGHT_SHALLOW_SLOPE_3_INDEX)
			{
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
					setY(global_height - (TEST_ENEMY_COLLIDER_HEIGHT / 2) - TEST_ENEMY_COLLIDER_OFFSET_Y);
				}
			}
			
			else if(tile_index == RIGHT_SHALLOW_SLOPE_4_INDEX)
			{
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
					setY(global_height - (TEST_ENEMY_COLLIDER_HEIGHT / 2) - TEST_ENEMY_COLLIDER_OFFSET_Y);
				}
			}
			
			else if(tile_index == RIGHT_STEEP_SLOPE_1_INDEX)
			{
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
					setY(global_height - (TEST_ENEMY_COLLIDER_HEIGHT / 2) - TEST_ENEMY_COLLIDER_OFFSET_Y);
				}
			}
			
			else if(tile_index == RIGHT_STEEP_SLOPE_2_INDEX)
			{
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
					setY(global_height - (TEST_ENEMY_COLLIDER_HEIGHT / 2) - TEST_ENEMY_COLLIDER_OFFSET_Y);
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
					collider_y_axis.p4.y() <= other_collider.p1.y() + TEST_ENEMY_GRAVITY)
				{
					// Handle Remaining Collision Cases //
					while(collider_y_axis.isCollision(other_collider))
					{
						collider_y_axis.setY(collider_y_axis.y() - 1);
						setY(this->y() - 1);
					}
				}
			}
		}
	}

	/////////////////////////////////
	// Generic Object Update stuff //
	/////////////////////////////////
	
	GameObject::update(room_bounds,
					   game_objects,
					   bg_ptr,
					   cells,
					   bg_item,
					   camera);

}