#include "scythe_platform.h"

ScythePlatform::ScythePlatform(Direction _dir, bn::fixed_point _p)
{
    // Reset Variables //
    sprite_ptr.reset();
    animate_action_ptr.reset();
    delete rigidbody_ptr;
    delete collider_ptr;

    // Init Variables //
    object_type = SCYTHE_PLATFORM;
    sprite_ptr  = bn::sprite_items::scythe_platform.create_sprite(0, 0);
    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                  2,
								                                  bn::sprite_items::scythe_platform.tiles_item(),
								                                  0,
								                                  0);

    collider_offset_x = 0;
    collider_offset_y = 0;

    rigidbody_ptr = new RigidBody();
	collider_ptr  = new Collider(x() + collider_offset_x, 
                                 y() + collider_offset_y, 
                                 SCYTHE_PLATFORM_COLLIDER_WIDTH, 
                                 SCYTHE_PLATFORM_COLLIDER_HEIGHT);

    state = STATE_THROWN;
    speed = SCYTHE_PLATFORM_SPEED;
    dir   = _dir;

    setPos(_p);
}

ScythePlatform::~ScythePlatform()
{

}

void ScythePlatform::update(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects,
                            const Room& room,
                            const bn::camera_ptr& camera)
{
    bool is_stuck = false;

	// Player made the scythe hidden on creation, undo now:
	sprite_ptr->set_visible(true);

    ///////////////////
    // State Machine //
    ///////////////////

    switch(state)
    {
        case STATE_THROWN:
            rigidbody_ptr->addForce(SCYTHE_PLATFORM_X_FORCE);

        break;

        case STATE_STUCK:
        break;

        default:
        break;
    }

    ///////////////////
    // Apply Physics //
    ///////////////////
    
    // Apply Decay to Forces
    rigidbody_ptr->applyDecay();

    // Apply forces to scythe platform
    bn::fixed_point final_dir = applyForces();

    ////////////////////////////
    // Resolve Tile Collision //
    ////////////////////////////

    // Get a normalized direction vector, to be used for collision correction if
	// there is a collision.
	bn::fixed normalized_dir_x = 0;
	bn::fixed normalized_dir_y = 0;
	if(final_dir.x() != 0) {normalized_dir_x = final_dir.x() / abs(final_dir.x());}
	if(final_dir.y() != 0) {normalized_dir_y = final_dir.y() / abs(final_dir.y());}
	bn::fixed_point normalized_dir = bn::fixed_point(normalized_dir_x, normalized_dir_y);

	// Create one temporary collider for each axis. If a collider finds a collision
	// in its axis, move the temp collider AND the Scythe back along the dir vector
	// in units of 1 until the collision is resolved on that axis.
	Collider* temp_collider_x_ptr = new Collider(collider_ptr->x(),
											     collider_ptr->y() - final_dir.y(),
											     collider_ptr->width,
												 collider_ptr->height);
	Collider* temp_collider_y_ptr = new Collider(collider_ptr->x() - final_dir.x(),
											     collider_ptr->y(),
											     collider_ptr->width,
												 collider_ptr->height);
	Collider* other_collider_ptr = NULL;

	// Get current cell index that Scythe resides in:
	int32 half_room_width_pixels  = room.bg_ptr->dimensions().width() / 2;
	int32 half_room_height_pixels = room.bg_ptr->dimensions().height() / 2;
	bn::fixed index_x = (x() + half_room_width_pixels)  / TILE_WIDTH;
	bn::fixed index_y = (y() + half_room_height_pixels) / TILE_HEIGHT;
	bn::point cell_index = bn::point(index_x.integer(), index_y.integer());

	for(int32 y = -2; y < 3; y++)
	{
		for(int32 x = -2; x < 3; x++)
		{
			// 1. Get tile type at index //
			int32 check_index_x = cell_index.x() + x;
			int32 check_index_y = cell_index.y() + y;

			// Determine world coords in case we need to make a collider.
			int32 world_x = ((check_index_x * TILE_WIDTH)  - half_room_width_pixels)  + (TILE_WIDTH / 2);
			int32 world_y = ((check_index_y * TILE_HEIGHT) - half_room_height_pixels) + (TILE_HEIGHT / 2);

			uint32 tile_index = room.getTileAtIndex(check_index_x, check_index_y);

			// Prepare offsets in case they are needed for Block collision.
			int32 block_w_offset = 0;
			int32 block_x_offset = 0;

			// 2. If the tile is collidable make a temporary collider //
			switch(tile_index)
			{
				case BLOCK_INDEX:

					// If the neighbor to the right is also a BLOCK, smooth over the corner.
					// This is a hack to resolve collision since checks are always made from
					// left to right. 
					if(room.getTileAtIndex(check_index_x + 1,
										   check_index_y) == BLOCK_INDEX)
					{
						block_w_offset = TILE_WIDTH;
						block_x_offset = TILE_WIDTH / 2;
						x++; // Skip checking the next cell, since we already accounted for it here.
					} 

					other_collider_ptr = new Collider(world_x + block_x_offset,
													  world_y, 
													  TILE_WIDTH + block_w_offset, 
													  TILE_HEIGHT);

					if(collider_ptr->isCollision(*(other_collider_ptr)))
					{
                        // Update state (deferred)
                        is_stuck = true;    

						// Handle Default Collision Cases //
						while(temp_collider_x_ptr->isCollision(*other_collider_ptr))
						{
							temp_collider_x_ptr->setX(temp_collider_x_ptr->x() - dir);
							setX(this->x() - dir);
						}

						while(temp_collider_y_ptr->isCollision(*other_collider_ptr))
						{
							if(normalized_dir.y() == 0) {break;}
							temp_collider_y_ptr->setY(temp_collider_y_ptr->y() - normalized_dir.y());
							setY(this->y() - normalized_dir.y());
						}

						// If there is still collision somehow, must be corner case //
						while(collider_ptr->isCollision(*(other_collider_ptr)))
						{
							// We always resolve diagonal corner collisions with a horizontal shift. 
							setX(this->x() - dir);
						}
					}

					delete other_collider_ptr;

				break;

				default:
				break;
			}
		}
	}

    ///////////////////////////////////
    // Resolve Game Object Collision //
    ///////////////////////////////////

    // Create test collider for roof collision checks
    #define SCYTHE_PLATFORM_ROOF_OFFSET           -6
    #define SCYTHE_PLATFORM_ROOF_COLLIDER_HEIGHT   1

    Collider* test_collider_roof_ptr = new Collider(collider_ptr->x(),
                                                    collider_ptr->y() + SCYTHE_PLATFORM_ROOF_OFFSET,
                                                    collider_ptr->width,
                                                    SCYTHE_PLATFORM_ROOF_COLLIDER_HEIGHT);

    for(int32 i = 0; i < game_objects.size(); i++)
    {

		GameObject* object_ptr = game_objects.at(i);
		other_collider_ptr     = object_ptr->collider_ptr;

		switch(object_ptr->object_type)
		{
			case PLAYER:
				
				// If player is riding the platform:
				if(test_collider_roof_ptr->isCollision(*other_collider_ptr) &&
					other_collider_ptr->p4.y() < collider_ptr->p1.y())
				{
					if(final_dir.y() <= 0)
					{
						// If descending, applying force to the x axis is all that's needed.
						// The player gravity will take care of the rest. 
						object_ptr->rigidbody_ptr->addForce(new Force(bn::fixed_point_t<12>(final_dir.x(), 0),
																	SCYTHE_PLATFORM_DECAY));
					}
					else
					{
						// If ascending, apply force to BOTH axes and offset y by 1 
						// so the player hugs the platform tight.
						object_ptr->rigidbody_ptr->addForce(new Force(bn::fixed_point_t<12>(final_dir.x(), final_dir.y() + 1),
																	SCYTHE_PLATFORM_DECAY));
					}
				}

			break;

			case DEVIL_PLATFORM:

				if(collider_ptr->isCollision(*other_collider_ptr))
				{
					// Update state (deferred)
					is_stuck = true;

					// Handle Default Collision Cases //
					while(temp_collider_x_ptr->isCollision(*other_collider_ptr))
					{
						if(normalized_dir.x() == 0) {break;}
						temp_collider_x_ptr->setX(temp_collider_x_ptr->x() - normalized_dir.x());
						setX(this->x() - normalized_dir.x());
					}

					/*
					while(temp_collider_y_ptr->isCollision(*other_collider_ptr))
					{
						if(normalized_dir.y() == 0) {break;}
						temp_collider_y_ptr->setY(temp_collider_y_ptr->y() - normalized_dir.y());
						setY(this->y() - normalized_dir.y());
					} */

					// If there is still collision somehow, must be corner case //
					while(collider_ptr->isCollision(*(other_collider_ptr)))
					{
						if(normalized_dir.x() == 0) {break;}
						// We always resolve diagonal corner collisions with a horizontal shift. 
						setX(this->x() - normalized_dir.x());
					}	
				}

			break;

			default:
			break;
		}

		object_ptr         = NULL;
		other_collider_ptr = NULL;

    }

    // Clean up temp colliders
    delete test_collider_roof_ptr;
	delete temp_collider_x_ptr;
	delete temp_collider_y_ptr;

    ///////////////////
    // Update States //
    ///////////////////

    if(is_stuck) {state = STATE_STUCK;}

    //////////////////////
	// Update Direction //
	//////////////////////

    if(dir == LEFT)        {sprite_ptr->set_horizontal_flip(true);}
	else if (dir == RIGHT) {sprite_ptr->set_horizontal_flip(false);}
}