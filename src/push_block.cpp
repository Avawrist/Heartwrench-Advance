#include "push_block.h"

//////////////////////
// Struct PushBlock //
//////////////////////

PushBlock::PushBlock()
{
    // Init Assets //
    object_type = PUSH_BLOCK;
    sprite_ptr  = bn::sprite_items::push_block.create_sprite(0, 0);
    sprite_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                   0,
								                                   bn::sprite_items::push_block.tiles_item(),
								                                   0, 0);

    // Init Variables //
    collider_offset_x = PUSH_BLOCK_COLLIDER_OFFSET_X;
    collider_offset_y = PUSH_BLOCK_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        PUSH_BLOCK_COLLIDER_WIDTH, 
                        PUSH_BLOCK_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;

    state       = IDLE;
    object_type = PUSH_BLOCK;
    x_dir       = LEFT;
    y_dir       = UP;

    hitpoints = PUSH_BLOCK_HITPOINTS;
}

PushBlock::PushBlock(const PushBlock& other) : GameObject(other)
{

}

PushBlock::~PushBlock()
{

}

PushBlock& PushBlock::operator =(const PushBlock& other)
{
    return *this;
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

// None..

//////////////////////////////
// State Function Overrides //
//////////////////////////////

void PushBlock::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
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

        default:
        break;
    }
}

/////////////////////////
// Collision Overrides //
/////////////////////////

void PushBlock::resolveTileCollision(const bn::regular_bg_ptr&                      bg_ptr, 
                                        const bn::span<const bn::regular_bg_map_cell>& cells,
                                        const bn::regular_bg_item&                     bg_item)
{
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

    for(int32 y = -2; y < 3; y++)
	{
		for(int32 x = -2; x < 3; x++)
		{
			
			// 1. Get tile type at index //
			int32 check_index_x = cell_index.x() + x;
			int32 check_index_y = cell_index.y() + y;

			// Determine world coords in case we need to make a collider.
			int32 world_x = ((check_index_x * TILE_WIDTH)  - half_level_width_pixels)  + (TILE_WIDTH / 2);
			int32 world_y = ((check_index_y * TILE_HEIGHT) - half_level_height_pixels) + (TILE_HEIGHT / 2);

			uint32 tile_index = getTileAtBGIndex(check_index_x, check_index_y, 
			                                     bg_ptr, cells, bg_item);

			// 2. If the tile is collidable make a temporary collider based on type //

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
					// Resolve Axis Collision
					resolveXAxisCollision(other_collider);
					resolveYAxisCollision(other_collider);

					// If there is still collision somehow, must be corner case
					resolveCornerCollision(other_collider);
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

                resolveLeftShallowSlope1Collision(other_collider, world_y);
            }
                
            else if(tile_index == LEFT_SHALLOW_SLOPE_2_INDEX)
            {
                other_collider = Collider(world_x, 
                                          world_y + 2, 
                                          TILE_WIDTH, 
                                          TILE_HEIGHT / 2);

                resolveLeftShallowSlope2Collision(other_collider, world_y);
            }

            else if(tile_index == LEFT_SHALLOW_SLOPE_3_INDEX)
            {
                other_collider = Collider(world_x, 
                                          world_y + 1, 
                                          TILE_WIDTH, 
                                          TILE_HEIGHT - 2);

                resolveLeftShallowSlope3Collision(other_collider, world_y);
            }

            else if(tile_index == LEFT_SHALLOW_SLOPE_4_INDEX)
            {
                other_collider = Collider(world_x, 
                                          world_y, 
                                          TILE_WIDTH, 
                                          TILE_HEIGHT);

                resolveLeftShallowSlope4Collision(other_collider, world_y);
            }

            else if(tile_index == LEFT_STEEP_SLOPE_1_INDEX)
            {
                
                other_collider = Collider(world_x, 
                                          world_y + 2, 
                                          TILE_WIDTH, 
                                          TILE_HEIGHT / 2);

                resolveLeftSteepSlope1Collision(other_collider, world_y);
            }

            else if(tile_index == LEFT_STEEP_SLOPE_2_INDEX)
            {
                other_collider = Collider(world_x, 
                                          world_y, 
                                          TILE_WIDTH, 
                                          TILE_HEIGHT);

                resolveLeftSteepSlope2Collision(other_collider, world_y);
            }

            else if(tile_index == RIGHT_SHALLOW_SLOPE_1_INDEX)
            {
                other_collider = Collider(world_x, 
                                          world_y + 3, 
                                          TILE_WIDTH, 
                                          TILE_HEIGHT / 4);

                resolveRightShallowSlope1Collision(other_collider, world_y);
            }

            else if(tile_index == RIGHT_SHALLOW_SLOPE_2_INDEX)
            {
                other_collider = Collider(world_x, 
                                          world_y + 2, 
                                          TILE_WIDTH, 
                                          TILE_HEIGHT / 2);

                resolveRightShallowSlope2Collision(other_collider, world_y);
            }

            else if(tile_index == RIGHT_SHALLOW_SLOPE_3_INDEX)
            {
                other_collider = Collider(world_x, 
                                            world_y + 1,
                                            TILE_WIDTH, 
                                            TILE_HEIGHT - 2);

                resolveRightShallowSlope3Collision(other_collider, world_y);
            }
            
            else if(tile_index == RIGHT_SHALLOW_SLOPE_4_INDEX)
            {
                other_collider = Collider(world_x, 
                                            world_y,
                                            TILE_WIDTH, 
                                            TILE_HEIGHT);

                resolveRightShallowSlope4Collision(other_collider, world_y);
            }
            
            else if(tile_index == RIGHT_STEEP_SLOPE_1_INDEX)
            {
                other_collider = Collider(world_x, 
                                            world_y + 2,
                                            TILE_WIDTH, 
                                            TILE_HEIGHT / 2);

                resolveRightSteepSlope1Collision(other_collider, world_y);
            }
            
            else if(tile_index == RIGHT_STEEP_SLOPE_2_INDEX)
            {
                other_collider = Collider(world_x, 
                                            world_y,
                                            TILE_WIDTH, 
                                            TILE_HEIGHT);

                resolveRightSteepSlope2Collision(other_collider, world_y);
            }
            
            else if(tile_index >= ONEWAY_BLOCK_MIN_INDEX &&
                    tile_index <= ONEWAY_BLOCK_MAX_INDEX)
            {
                other_collider = Collider(world_x, 
                                            world_y + ONEWAYBLOCK_COLLIDER_Y_OFFSET, 
                                            TILE_WIDTH, 
                                            ONEWAYBLOCK_COLLIDER_HEIGHT);

                resolveOneWayBlockCollision(other_collider);
            }

		}
	}
}

void PushBlock::resolveUpSpikeCollision(const Collider& other_collider)
{
    if(collider.isCollision(other_collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(other_collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(other_collider);

        // If there is still collision somehow, must be corner case //
        resolveCornerCollision(other_collider);
    }
}

void PushBlock::resolveDownSpikeCollision(const Collider& other_collider)
{
    if(collider.isCollision(other_collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(other_collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(other_collider);

        // If there is still collision somehow, must be corner case //
        resolveCornerCollision(other_collider);
    }
}

void PushBlock::resolveLeftSpikeCollision(const Collider& other_collider)
{
    if(collider.isCollision(other_collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(other_collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(other_collider);

        // If there is still collision somehow, must be corner case //
        resolveCornerCollision(other_collider);
    }
}

void PushBlock::resolveRightSpikeCollision(const Collider& other_collider)
{
    if(collider.isCollision(other_collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(other_collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(other_collider);

        // If there is still collision somehow, must be corner case //
        resolveCornerCollision(other_collider);
    }
}
