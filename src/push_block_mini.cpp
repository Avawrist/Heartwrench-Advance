#include "push_block_mini.h"

//////////////////////
// Struct PushBlockMini //
//////////////////////

PushBlockMini::PushBlockMini()
{
    // Init Assets //
    object_type = PUSH_BLOCK_MINI;
    sprite_ptr  = bn::sprite_items::push_block_mini.create_sprite(0, 0);
    sprite_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                   0,
								                                   bn::sprite_items::push_block_mini.tiles_item(),
								                                   0, 0);

    // Init Variables //
    collider_offset_x = PUSH_BLOCK_MINI_COLLIDER_OFFSET_X;
    collider_offset_y = PUSH_BLOCK_MINI_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        PUSH_BLOCK_MINI_COLLIDER_WIDTH, 
                        PUSH_BLOCK_MINI_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;

    state       = IDLE;
    object_type = PUSH_BLOCK_MINI;
    x_dir       = LEFT;
    y_dir       = UP;

    hitpoints = PUSH_BLOCK_MINI_HITPOINTS;
	damage    = PUSH_BLOCK_MINI_DAMAGE;

	thirty_fps = PUSH_BLOCK_MINI_30_FPS;

	frame_start_pos = bn::fixed_point(0, 0);
	hit_h_wall 			  = 0;
	hit_v_wall 			  = 0;
	received_track_force  = false;
}

PushBlockMini::PushBlockMini(const PushBlockMini& other) : GameObject(other)
{
	frame_start_pos = other.frame_start_pos;
	hit_h_wall = other.hit_h_wall;
	hit_v_wall = other.hit_v_wall;
	received_track_force  = other.received_track_force;
}

PushBlockMini::~PushBlockMini()
{

}

PushBlockMini& PushBlockMini::operator =(const PushBlockMini& other)
{
	frame_start_pos = other.frame_start_pos;
	hit_h_wall = other.hit_h_wall;
	hit_v_wall = other.hit_v_wall;
	received_track_force  = other.received_track_force;

    return *this;
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

void PushBlockMini::updatePhysics()
{
	frame_start_pos = pos();

	GameObject::updatePhysics();
}

void PushBlockMini::checkIfDead()
{

}

void PushBlockMini::updateSpriteDirection()
{

}

void PushBlockMini::updateTimers()
{
	GameObject::updateTimers();

	hit_h_wall--;
	hit_h_wall = clamp(0, PUSH_BLOCK_MINI_HIT_H_WALL_FRAMES, hit_h_wall);

	hit_v_wall--;
	hit_v_wall = clamp(0, PUSH_BLOCK_MINI_HIT_V_WALL_FRAMES, hit_v_wall);
}

void PushBlockMini::updateHitFlash()
{
    if((hit_flash_frames || invulnerability_frames) || state == OBJECT_DEATH)
    {
        bn::sprite_palette_ptr new_palette = bn::sprite_palette_items::sprite_flash_palette.create_palette();

        int32 rotate_range = new_palette.rotate_range_size();
        int32 rotate_count = new_palette.rotate_count();
        
        if(global_timer % 4 == 0)
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

void PushBlockMini::setHitFlash()
{
	hit_flash_frames = GAME_OBJECT_MAX_HIT_FLASH_FRAMES;

    bn::sprite_palette_ptr sprite_palette = bn::sprite_palette_items::sprite_flash_palette.create_palette();

    if(sprite_ptr.has_value())
    {sprite_ptr->set_palette(sprite_palette);}
}

void PushBlockMini::setHitFlash(int32 frames)
{
    if(frames > GAME_OBJECT_MAX_HIT_FLASH_FRAMES) 
    {frames = GAME_OBJECT_MAX_HIT_FLASH_FRAMES;}

    hit_flash_frames = frames;

    bn::sprite_palette_ptr sprite_palette = bn::sprite_palette_items::sprite_flash_palette.create_palette();
    
    if(sprite_ptr.has_value())
    {sprite_ptr->set_palette(sprite_palette);}
}

//////////////////////////////
// State Function Overrides //
//////////////////////////////

void PushBlockMini::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
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

		case PUSH_BLOCK_ROLLING:

			// Gravity
            if(!grounded_detected)
            {rigidbody.addForce(GAME_OBJECT_GRAVITY_FORCE);}

		break;

        case OBJECT_HITSTUN:

            updateHitstunState();

        break;

        default:
        break;
    }
}

void PushBlockMini::updateState(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
							const bn::regular_bg_ptr&                      bg_ptr, 
							const bn::span<const bn::regular_bg_map_cell>& cells,
							const bn::regular_bg_item&                     bg_item)
{
	if(state == IDLE) {received_track_force = false;}

	ObjectState new_state = state;

	int32 pixels_moved_x = (frame_start_pos.x().integer() - pos().x().integer()) * -1;
	int32 pixels_moved_y = (frame_start_pos.y().integer() - pos().y().integer()) * -1;

	if(pixels_moved_x != 0 ||
       pixels_moved_y != 0) {new_state = PUSH_BLOCK_ROLLING;}
	else                    {new_state = IDLE;}

	if(new_state != state) {setState(new_state);}
}

void PushBlockMini::setState(ObjectState new_state)
{
	state = new_state;

	switch(new_state)
	{
		case IDLE:

			animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
																		  0,
																		  bn::sprite_items::push_block_mini.tiles_item(),
																		  0, 0);

		break;

		case PUSH_BLOCK_ROLLING:

			animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
																		  0,
																		  bn::sprite_items::push_block_mini.tiles_item(),
																		  0, 0);

		break;

		default:
		break;
	}
}

/////////////////////////
// Collision Overrides //
/////////////////////////

// Level Objects
void PushBlockMini::resolveFallingPlatformWideCollision(GameObject& object)
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

void PushBlockMini::resolveFallingPlatformThinCollision(GameObject& object)
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

void PushBlockMini::resolveSmashBlockLargeCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
	{
		object.applyHit(damage, rigidbody.normalized_dir.x().integer(), 0);
	}
}

void PushBlockMini::resolveSmashBlockMiniCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
	{
		object.applyHit(damage, rigidbody.normalized_dir.x().integer(), 0);
	}
}

void PushBlockMini::resolveLargeVaseCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
	{
		object.applyHit(damage, rigidbody.normalized_dir.x().integer(), 0);
	}
}

void PushBlockMini::resolveSmallVaseCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
	{
		object.applyHit(damage, rigidbody.normalized_dir.x().integer(), 0);
	}
}

// Enemy Objects
void PushBlockMini::resolveGroundGhoulCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	// Now test for roof riding and resolve the PushBlock + Object.
	int32 pixels_moved_x = (frame_start_pos.x().integer() - pos().x().integer()) * -1;
	int32 pixels_moved_y = (frame_start_pos.y().integer() - pos().y().integer()) * -1;

	Collider roof_test_collider = Collider(collider.x() - pixels_moved_x,
										   collider.y() - pixels_moved_y + PUSH_BLOCK_MINI_ROOF_OFFSET,
										   collider.width,
										   PUSH_BLOCK_MINI_ROOF_COLLIDER_HEIGHT);

	if(roof_test_collider.isCollision(object.collider))
	{
		object.rigidbody.addForce(Force(bn::fixed_point_t<12>(pixels_moved_x, 0), 1));
		object.setY(object.y() + pixels_moved_y);
	}
}

// Special Objects
void PushBlockMini::resolvePlayerCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	// Now test for roof riding and resolve the PushBlockMini + Player.
	int32 pixels_moved_x = (frame_start_pos.x().integer() - pos().x().integer()) * -1;
	int32 pixels_moved_y = (frame_start_pos.y().integer() - pos().y().integer()) * -1;

	Collider roof_test_collider = Collider(collider.x() - pixels_moved_x,
										   collider.y() - pixels_moved_y + PUSH_BLOCK_MINI_ROOF_OFFSET,
										   collider.width,
										   PUSH_BLOCK_MINI_ROOF_COLLIDER_HEIGHT);

	if(roof_test_collider.isCollision(object.collider))
	{
		object.rigidbody.addForce(Force(bn::fixed_point_t<12>(pixels_moved_x, 0), 1));
		object.setY(object.y() + pixels_moved_y);

		if(hit_h_wall && (object.state == PLAYER_SPIN_ATTACK || object.state == PLAYER_AIR_NEUTRAL))
		{object.rigidbody.addForce(PUSH_BLOCK_MINI_MOMENTUM_TRANSFER_H_FORCE); hit_h_wall = 0;}

		if(hit_v_wall && object.state == PLAYER_AIR_NEUTRAL)
		{object.rigidbody.addForce(PUSH_BLOCK_MINI_MOMENTUM_TRANSFER_V_FORCE); hit_v_wall = 0;}
	}

	// Backwards collision resolution, let the player correct itself.
	// Expensive, but necessary.
	if(object.collider.isCollision(collider))
	{
		object.resolveXAxisCollision(collider);
		object.resolveYAxisCollision(collider);
		object.resolveCornerCollision(collider);
		object.updateTestColliders();
	}
}

// Tiles
void PushBlockMini::resolveTileCollision(const bn::regular_bg_ptr&                      bg_ptr, 
                                     	 const bn::span<const bn::regular_bg_map_cell>& cells,
                                     	 const bn::regular_bg_item&                     bg_item)
{
	////////////////////////////////////////
    // Update Variables for state testing //
	////////////////////////////////////////
	grounded_detected = false;

	// Grounded test collider
	test_collider = Collider(collider.x(), 
	                         collider.y() + GAME_OBJECT_GROUND_RAY_LENGTH,
							 collider.width, 
							 collider.height);

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

			// 2. If the tile is collidable make a temporary collider based on type //

			if((tile_index >= HARD_BLOCK_MIN_INDEX && 
			    tile_index <= HARD_BLOCK_MAX_INDEX) || 
               (tile_index >= SPIKE_MIN_INDEX && 
                tile_index <= SPIKE_MAX_INDEX))
			{
				// Prepare offsets in case they are needed for Block collision.
				int32 block_w_offset = 0;
				int32 block_x_offset = 0;

				// If the neighbor to the right is also a HARD BLOCK or SPIKE, smooth over the corner.
				// This is a hack to resolve collision since checks are always made from
				// left to right. 
				if((getTileAtBGIndex(check_index_x + 1, check_index_y, 
									 bg_ptr, cells, bg_item) >= HARD_BLOCK_MIN_INDEX && 
					getTileAtBGIndex(check_index_x + 1, check_index_y, 
									 bg_ptr, cells, bg_item) <= HARD_BLOCK_MAX_INDEX) ||
                    (getTileAtBGIndex(check_index_x + 1, check_index_y, 
                                      bg_ptr, cells, bg_item) >= SPIKE_MIN_INDEX && 
                    getTileAtBGIndex(check_index_x + 1, check_index_y, 
                                     bg_ptr, cells, bg_item) <= SPIKE_MAX_INDEX))
				{
					block_w_offset = TILE_WIDTH;
					block_x_offset = TILE_WIDTH / 2;
					x++;
				}

				other_collider = Collider(world_x + block_x_offset, 
										  world_y, 
										  TILE_WIDTH + block_w_offset,
										  TILE_HEIGHT);

				resolveHardBlockCollision(other_collider);
			}

            else if(tile_index == H_GEAR_LEFT)
            {
                other_collider = Collider(world_x,
                                          world_y, 
                                          TILE_WIDTH, 
                                          TILE_HEIGHT);
                
                resolveHGearLeftCollision(other_collider);
            }

            else if(tile_index >= H_GEAR_MID_MIN &&
                    tile_index <= H_GEAR_MID_MAX)
            {
                other_collider = Collider(world_x,
                                          world_y, 
                                          TILE_WIDTH, 
                                          TILE_HEIGHT);
                
                resolveHGearMidCollision(other_collider);
            }

            else if(tile_index == H_GEAR_RIGHT)
            {
                other_collider = Collider(world_x,
                                          world_y, 
                                          TILE_WIDTH, 
                                          TILE_HEIGHT);
                
                resolveHGearRightCollision(other_collider);
            }

            else if(tile_index == V_GEAR_TOP)
			{
				other_collider = Collider(world_x,
										  world_y, 
										  TILE_WIDTH, 
										  TILE_HEIGHT);

				resolveVGearTopCollision(other_collider);
			}

			else if(tile_index == V_GEAR_MID_1 ||
					tile_index == V_GEAR_MID_2 ||
					tile_index == V_GEAR_MID_3)
			{
				other_collider = Collider(world_x,
										  world_y, 
										  TILE_WIDTH, 
										  TILE_HEIGHT);

				resolveVGearMidCollision(other_collider);
			}

			else if(tile_index == V_GEAR_BOTTOM)
			{
				other_collider = Collider(world_x,
										  world_y, 
										  TILE_WIDTH, 
										  TILE_HEIGHT);

				resolveVGearBottomCollision(other_collider);
			}
    
            else if(tile_index == SHALLOW_SLOPE_1_INDEX)
            {
                other_collider = Collider(world_x, 
                                          world_y + 3, 
                                          TILE_WIDTH, 
                                          TILE_HEIGHT / 4);

                resolveShallowSlope1Collision(other_collider, world_y);
            }
                
            else if(tile_index == SHALLOW_SLOPE_2_INDEX)
            {
                other_collider = Collider(world_x, 
                                          world_y + 2, 
                                          TILE_WIDTH, 
                                          TILE_HEIGHT / 2);

                resolveShallowSlope2Collision(other_collider, world_y);
            }

            else if(tile_index == SHALLOW_SLOPE_3_INDEX)
            {
                other_collider = Collider(world_x, 
                                          world_y + 1, 
                                          TILE_WIDTH, 
                                          TILE_HEIGHT - 2);

                resolveShallowSlope3Collision(other_collider, world_y);
            }

            else if(tile_index == SHALLOW_SLOPE_4_INDEX)
            {
                other_collider = Collider(world_x, 
                                          world_y, 
                                          TILE_WIDTH, 
                                          TILE_HEIGHT);

                resolveShallowSlope4Collision(other_collider, world_y);
            }

            else if(tile_index == STEEP_SLOPE_1_INDEX)
            {
                
                other_collider = Collider(world_x, 
                                          world_y + 2, 
                                          TILE_WIDTH, 
                                          TILE_HEIGHT / 2);

                resolveSteepSlope1Collision(other_collider, world_y);
            }

            else if(tile_index == STEEP_SLOPE_2_INDEX)
            {
                other_collider = Collider(world_x, 
                                          world_y, 
                                          TILE_WIDTH, 
                                          TILE_HEIGHT);

                resolveSteepSlope2Collision(other_collider, world_y);
            }
            
			else if(tile_index >= CLIMBABLE_MIN_INDEX &&
					tile_index <= CLIMBABLE_MAX_INDEX)
			{
				other_collider = Collider(world_x,
										  world_y, 
										  TILE_WIDTH, 
										  TILE_HEIGHT);
				
				resolveClimbableCollision(other_collider);
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

void PushBlockMini::resolveXAxisCollision(const Collider& other_collider)
{
	GameObject::resolveXAxisCollision(other_collider);

	if(col_x_offset != 0 && pos().x() != frame_start_pos.x())
	{
		hit_h_wall = PUSH_BLOCK_MINI_HIT_H_WALL_FRAMES;
		rigidbody.removeXForces();
	}
}

void PushBlockMini::resolveYAxisCollision(const Collider& other_collider)
{
	GameObject::resolveYAxisCollision(other_collider);

	if(col_y_offset != 0 && pos().y() != frame_start_pos.y()) 
	{
		hit_v_wall = PUSH_BLOCK_MINI_HIT_V_WALL_FRAMES;
		rigidbody.removeYForces();
	}
}