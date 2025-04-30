#include "ground_ghoul.h"

GroundGhoul::GroundGhoul()
{

        // Reset Variables //
        sprite_ptr.reset();
        animate_action_ptr.reset();
    
        // Init Variables //
        object_type = GROUND_GHOUL;
        sprite_ptr  = bn::sprite_items::ground_ghoul.create_sprite(0, 0);
        sprite_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
        animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                      2,
                                                                      bn::sprite_items::ground_ghoul.tiles_item(),
                                                                      0,
                                                                      0);
    
        collider          = Collider(x(), y(), GROUND_GHOUL_COLLIDER_WIDTH, GROUND_GHOUL_COLLIDER_HEIGHT);
        collider_x_axis   = collider;
        collider_y_axis   = collider;
        collider_offset_x = GROUND_GHOUL_COLLIDER_OFFSET_X;
        collider_offset_y = GROUND_GHOUL_COLLIDER_OFFSET_Y;

        state = GROUND_GHOUL_CRAWL_STATE;
        dir   = RIGHT;

        grounded_detected = false;

        test_collider       = collider;
        test_collider_right = collider;
        test_collider_left  = collider;

}

GroundGhoul::GroundGhoul(const GroundGhoul& other) : GameObject(other)
{
    state = other.state;
    grounded_detected = other.grounded_detected;

    test_collider       = other.test_collider;
    test_collider_right = other.test_collider_right;
    test_collider_left  = other.test_collider_left;
}

GroundGhoul::~GroundGhoul()
{

}

GroundGhoul& GroundGhoul::operator =(const GroundGhoul& other)
{
    state = other.state;
    grounded_detected = other.grounded_detected;

    test_collider       = other.test_collider;
    test_collider_right = other.test_collider_right;
    test_collider_left  = other.test_collider_left;

    return *this;
}

void GroundGhoul::update(const RoomBounds&                              room_bounds,
                         bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                         const bn::regular_bg_ptr&                      bg_ptr, 
                         const bn::span<const bn::regular_bg_map_cell>& cells,
                         const bn::regular_bg_item&                     bg_item,
                         const bn::camera_ptr&                          camera)
{

    ///////////////////
    // State Machine //
    ///////////////////

    switch(state)
    {
        case GROUND_GHOUL_IDLE_STATE:
        break;

        case GROUND_GHOUL_CRAWL_STATE:

            rigidbody.addForce(GROUND_GHOUL_CRAWL_FORCE);

        break;

        case GROUND_GHOUL_AIR_STATE:

            rigidbody.addForce(GROUND_GHOUL_CRAWL_FORCE);
            rigidbody.addForce(GROUND_GHOUL_GRAVITY_FORCE);

        break;

        case GROUND_GHOUL_HITSTUN_STATE:
        break;

        case GROUND_GHOUL_DEATH_STATE:

            is_dead = true;

        break;

        default:
        break;
    }

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
	bn::fixed index_x = (x() + half_level_width_pixels  + collider_offset_x)  / TILE_WIDTH;
	bn::fixed index_y = (y() + half_level_height_pixels + collider_offset_y)  / TILE_HEIGHT;
	bn::point cell_index = bn::point(index_x.integer(), index_y.integer());

	// Update colliders for each axis. 
	collider_x_axis.setPos(collider.x(), collider.y() - rigidbody.final_dir.y());
	collider_y_axis.setPos(collider.x() - rigidbody.final_dir.x(), collider.y());

	// Placeholder for other objects
	Collider other_collider;

    //////////////////////////////////
    // Resolve GameObject Collision //
    //////////////////////////////////

    for(int32 i = 0; i < game_objects.size(); i++)
    {
        other_collider = game_objects.at(i)->collider;
        
        bn::fixed col_x_offset;
        bn::fixed col_y_offset;

        switch(game_objects.at(i)->object_type)
        {
            case TILE_PASSAGE:
                
                if(((TilePassage*)(game_objects.at(i)))->state == TILE_PASSAGE_STATE_SHUT &&
                    collider.isCollision(other_collider))
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

            break;

            default:
            break;
        }
    }

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
                    if(col_x_offset < 0)      {dir = LEFT;}
                    else if(col_x_offset > 0) {dir = RIGHT;}

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
					setY(global_height - (collider.height / 2) - collider_offset_y);
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
					setY(global_height - (collider.height / 2) - collider_offset_y);
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
					setY(global_height - (collider.height / 2) - collider_offset_y);
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
					setY(global_height - (collider.height / 2) - collider_offset_y);
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
					setY(global_height - (collider.height / 2) - collider_offset_y);
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
					setY(global_height - (collider.height / 2) - collider_offset_y);
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
					setY(global_height - (collider.height / 2) - collider_offset_y);
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
					setY(global_height - (collider.height / 2) - collider_offset_y);
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
					setY(global_height - (GROUND_GHOUL_COLLIDER_HEIGHT / 2) - GROUND_GHOUL_COLLIDER_OFFSET_Y);
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
					setY(global_height - (collider.height / 2) - collider_offset_y);
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
					setY(global_height - (collider.height / 2) - collider_offset_y);
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
					setY(global_height - (collider.height / 2) - collider_offset_y);
				}
			}
			
			else if(tile_index >= ONEWAY_BLOCK_MIN_INDEX &&
			        tile_index <= ONEWAY_BLOCK_MAX_INDEX)
			{

				other_collider = Collider(world_x, 
										  world_y + ONEWAYBLOCK_COLLIDER_Y_OFFSET, 
										  TILE_WIDTH, 
										  ONEWAYBLOCK_COLLIDER_HEIGHT);

                // Handle Remaining Collision Cases //
                while(collider_y_axis.isCollision(other_collider))
                {
                    collider_y_axis.setY(collider_y_axis.y() - 1);
                    setY(this->y() - 1);
                }

			}
		}
	}

    ////////////////////////////////////////
	// Initialize State Testing Variables //
	////////////////////////////////////////
    
    grounded_detected = false;

    // Update test colliders for grounded collision checks
	const uint32 ground_ray_length = 1;
	test_collider.setPos(collider.x(),
                         collider.y() + ground_ray_length);

	// Create test colliders for wall collision checks
	const uint32 wall_ray_length = 1;
	test_collider_right.setPos(collider.x() + wall_ray_length,
                               collider.y());
	test_collider_left.setPos(collider.x() - wall_ray_length,
                              collider.y());

	int32 index;
	int32 local_height;
	int32 global_height;

    ////////////////////////////////
    // Get State from GameObjects //
    ////////////////////////////////

    for(int32 i = 0; i < game_objects.size(); i++)
    {
        if(i != object_id)
        {
            other_collider = game_objects.at(i)->collider;

            switch(game_objects.at(i)->object_type)
            {
                case TILE_PASSAGE:

                    // Test for, and log grounded collision
					if(((TilePassage*)(game_objects.at(i)))->state == TILE_PASSAGE_STATE_SHUT &&
					    test_collider.isCollision(other_collider))
					{grounded_detected = true;}

                break;

                case PHASE_ORB_UP:
                case PHASE_ORB_DOWN:
                case PHASE_ORB_LEFT:
                case PHASE_ORB_RIGHT:
                case DEVIL_PLATFORM:
                case ANGEL_PLATFORM:
                case SCYTHE_PLATFORM:
                break;
    
                case GROUND_GHOUL:
                case CEILING_GHOUL:
                case WALL_LEFT_GHOUL:
                case WALL_RIGHT_GHOUL:
                    
                    if(collider.isCollision(other_collider))
                    {
                        if(dir == RIGHT) {dir = LEFT;}
                        else             {dir = RIGHT;}
                    }
                    
                break;
    
                default:
                break;
            }
        }
	}

    //////////////////////////
    // Get State from Tiles //
    //////////////////////////

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
                {grounded_detected = true;}

            }
                    
            else if(tile_index >= ONEWAY_BLOCK_MIN_INDEX &&
                    tile_index <= ONEWAY_BLOCK_MAX_INDEX)
            {
            
                other_collider = Collider(world_x, 
                                          world_y + ONEWAYBLOCK_COLLIDER_Y_OFFSET, 
                                          TILE_WIDTH, 
                                          ONEWAYBLOCK_COLLIDER_HEIGHT);

                if(collider_y_axis.p4.y() <= other_collider.p1.y() + GROUND_GHOUL_GRAVITY)
                {
                    // Test for, and log grounded collision
                    if(test_collider.isCollision(other_collider) && 
                        rigidbody.normalized_dir.y() >= 0)
                    {grounded_detected = true;}
                }
            }

            else if(tile_index == UP_SPIKE_BLOCK_1_INDEX ||
                    tile_index == UP_SPIKE_BLOCK_2_INDEX)
            {

                other_collider = Collider(world_x,
                                          world_y, 
                                          TILE_WIDTH, 
                                          TILE_HEIGHT);
                
                if(test_collider.isCollision(other_collider) && 
                   rigidbody.normalized_dir.y() >= 0)
                {grounded_detected = true;}

            }

            else if(tile_index == DOWN_SPIKE_BLOCK_1_INDEX ||
                    tile_index == DOWN_SPIKE_BLOCK_2_INDEX)
            {

                other_collider = Collider(world_x,
                                          world_y, 
                                          TILE_WIDTH, 
                                          TILE_HEIGHT);
                
                if(test_collider.isCollision(other_collider) && 
                   rigidbody.normalized_dir.y() >= 0)
                {grounded_detected = true;}

            }

            else if(tile_index == LEFT_SPIKE_BLOCK_1_INDEX ||
                    tile_index == LEFT_SPIKE_BLOCK_2_INDEX)
            {
                other_collider = Collider(world_x,
                                            world_y, 
                                            TILE_WIDTH, 
                                            TILE_HEIGHT);
                
                if(test_collider.isCollision(other_collider) && 
                   rigidbody.normalized_dir.y() >= 0)
                {grounded_detected = true;}

            }

            else if(tile_index == RIGHT_SPIKE_BLOCK_1_INDEX || 
                    tile_index == RIGHT_SPIKE_BLOCK_2_INDEX)
            {

                other_collider = Collider(world_x,
                                            world_y, 
                                            TILE_WIDTH,
                                            TILE_HEIGHT);
                
                if(test_collider.isCollision(other_collider) && 
                   rigidbody.normalized_dir.y() >= 0)
                {grounded_detected = true;}

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

                    // Offset the decline of the slope if ghoul is moving with it.
                    // This avoids a frame in air state when descending slopes.
                    if(rigidbody.normalized_dir.x() == -1)
                    {rigidbody.addForce(GROUND_GHOUL_GRAVITY_FORCE);}
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

                    // Offset the decline of the slope if player is moving with it.
                    // This avoids a frame in air state when descending slopes.
                    if(rigidbody.normalized_dir.x() == -1)
                    {rigidbody.addForce(GROUND_GHOUL_GRAVITY_FORCE);}
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

                    // Offset the decline of the slope if player is moving with it.
                    // This avoids a frame in air state when descending slopes.
                    if(rigidbody.normalized_dir.x() == -1)
                    {rigidbody.addForce(GROUND_GHOUL_GRAVITY_FORCE);}
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

                    // Offset the decline of the slope if player is moving with it.
                    // This avoids a frame in air state when descending slopes.
                    if(rigidbody.normalized_dir.x() == -1)
                    {rigidbody.addForce(GROUND_GHOUL_GRAVITY_FORCE);}
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

                    // Offset the decline of the slope if player is moving with it.
                    // This avoids a frame in air state when descending slopes.
                    if(rigidbody.normalized_dir.x() == -1)
                    {rigidbody.addForce(GROUND_GHOUL_GRAVITY_FORCE);}
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

                    // Offset the decline of the slope if player is moving with it.
                    // This avoids a frame in air state when descending slopes.
                    if(rigidbody.normalized_dir.x() == -1)
                    {rigidbody.addForce(GROUND_GHOUL_GRAVITY_FORCE);}
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

                    if(rigidbody.normalized_dir.x() == 1)
                    {rigidbody.addForce(GROUND_GHOUL_GRAVITY_FORCE);}
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

                    if(rigidbody.normalized_dir.x() == 1)
                    {rigidbody.addForce(GROUND_GHOUL_GRAVITY_FORCE);}
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

                    if(rigidbody.normalized_dir.x() == 1)
                    {rigidbody.addForce(GROUND_GHOUL_GRAVITY_FORCE);}
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

                    if(rigidbody.normalized_dir.x() == 1)
                    {rigidbody.addForce(GROUND_GHOUL_GRAVITY_FORCE);}
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

                    if(rigidbody.normalized_dir.x() == 1)
                    {rigidbody.addForce(GROUND_GHOUL_GRAVITY_FORCE);}
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

                    // Offset the decline of the slope if player is moving with it.
                    // This avoids a frame in air state when descending slopes.
                    if(rigidbody.normalized_dir.x() == 1)
                    {rigidbody.addForce(GROUND_GHOUL_GRAVITY_FORCE);}
                }
            }	
        }
    }

    //////////////////
    // Update State //
    //////////////////

    if(grounded_detected) {setState(GROUND_GHOUL_CRAWL_STATE);}
    else                  {setState(GROUND_GHOUL_AIR_STATE);}

    /////////////////////////////
	// Update Sprite Direction //
	/////////////////////////////
	
	if      (dir == LEFT)  {sprite_ptr->set_horizontal_flip(true);}
	else if (dir == RIGHT) {sprite_ptr->set_horizontal_flip(false);}

	//////////////////////////////
    // Monitor unloading bounds //
    //////////////////////////////
    
	updateInactiveState(camera);

}

void GroundGhoul::setState(GroundGhoulState new_state)
{
    state = new_state;

    switch(new_state)
    {
        case GROUND_GHOUL_IDLE_STATE:
        break;

        case GROUND_GHOUL_CRAWL_STATE:
        break;

        case GROUND_GHOUL_AIR_STATE:
        break;

        case GROUND_GHOUL_HITSTUN_STATE:
        break;

        case GROUND_GHOUL_DEATH_STATE:
        break;

        default:
        break;
    }
}