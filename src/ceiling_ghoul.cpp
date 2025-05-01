#include "ceiling_ghoul.h"

CeilingGhoul::CeilingGhoul()
{

        // Reset Variables //
        sprite_ptr.reset();
        animate_action_ptr.reset();
    
        // Init Variables //
        object_type = CEILING_GHOUL;
        sprite_ptr  = bn::sprite_items::ceiling_ghoul.create_sprite(0, 0);
        sprite_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
        animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                      2,
                                                                      bn::sprite_items::ceiling_ghoul.tiles_item(),
                                                                      0,
                                                                      0);
    
        collider          = Collider(x(), y(), CEILING_GHOUL_COLLIDER_WIDTH, CEILING_GHOUL_COLLIDER_HEIGHT);
        collider_x_axis   = collider;
        collider_y_axis   = collider;
        collider_offset_x = CEILING_GHOUL_COLLIDER_OFFSET_X;
        collider_offset_y = CEILING_GHOUL_COLLIDER_OFFSET_Y;

        state = CEILING_GHOUL_CRAWL_STATE;
        dir   = RIGHT;
}

CeilingGhoul::CeilingGhoul(const CeilingGhoul& other) : GameObject(other)
{
    state = other.state;
}

CeilingGhoul::~CeilingGhoul()
{

}

CeilingGhoul& CeilingGhoul::operator =(const CeilingGhoul& other)
{
    state = other.state;

    return *this;
}

void CeilingGhoul::update(const RoomBounds&                              room_bounds,
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
        case CEILING_GHOUL_IDLE_STATE:
        break;

        case CEILING_GHOUL_CRAWL_STATE:

            rigidbody.addForce(CEILING_GHOUL_CRAWL_FORCE);

        break;

        case CEILING_GHOUL_HITSTUN_STATE:
        break;

        case CEILING_GHOUL_DEATH_STATE:

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

		}
	}

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

    //////////////////
    // Update State //
    //////////////////

    /////////////////////////////
	// Update Sprite Direction //
	/////////////////////////////
	
	if      (dir == LEFT)  {sprite_ptr->set_horizontal_flip(true);}
	else if (dir == RIGHT) {sprite_ptr->set_horizontal_flip(false);}

    //////////////////////
	// Update Hit Flash //
	//////////////////////

	updateHitFlash();

	//////////////////////////////
    // Monitor unloading bounds //
    //////////////////////////////
    
	updateInactiveState(camera);

}

void CeilingGhoul::setState(CeilingGhoulState new_state)
{
    state = new_state;

    switch(new_state)
    {
        case CEILING_GHOUL_IDLE_STATE:
        break;

        case CEILING_GHOUL_CRAWL_STATE:
        break;

        case CEILING_GHOUL_HITSTUN_STATE:
        break;

        case CEILING_GHOUL_DEATH_STATE:
        break;

        default:
        break;
    }
}