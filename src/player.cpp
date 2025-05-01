#include "player.h"

///////////////////
// Struct Player //
///////////////////

Player::Player()
{

    // Reset Variables //
    sprite_ptr.reset();
    animate_action_ptr.reset();

    // Init Variables //
	object_type = PLAYER;
	dir         = RIGHT;
    sprite_ptr  = bn::sprite_items::player.create_sprite(0, 0);
	sprite_ptr->set_z_order(PLAYER_Z_ORDER);
	animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  1,
								  								  bn::sprite_items::player.tiles_item(),
								  								  0,
								  								  0);

	collider            = Collider(x(), y(), PLAYER_COLLIDER_WIDTH, PLAYER_COLLIDER_HEIGHT);
	collider_x_axis     = collider;
	collider_y_axis     = collider;
	test_collider       = collider;
	test_collider_right = collider;
	test_collider_left  = collider;
	collider_offset_x   = PLAYER_COLLIDER_OFFSET_X;
	collider_offset_y   = PLAYER_COLLIDER_OFFSET_Y;
    
	state             	 = STATE_AIR_NEUTRAL;
    x_speed        	  	 = PLAYER_MIN_X_SPEED;
    jump_force           = PLAYER_BASE_JUMP_FORCE;
	secondary_jump_force = PLAYER_SECOND_JUMP_FORCE;
    wall_jump_force   	 = bn::fixed_point(PLAYER_WALL_JUMP_X_FORCE,
										   PLAYER_WALL_JUMP_Y_FORCE);
	gravity           	 = PLAYER_GRAVITY;
	wall_ride_gravity 	 = PLAYER_WALL_RIDE_GRAVITY;
	phase_destination    = bn::fixed_point(0, 0);
	
	remaining_jump_input_frames      = 0;
	remaining_x_drift_lockout_frames = 0;
	air_frames_elapsed               = 0;
	v_collision_grace_frames         = 0;
	late_jump_grace_frames           = 0;
	current_scythe_frame             = 0;
	current_death_frame              = 0;
	current_phase_frame              = 0;
	
	wall_right_detected      = false;
    wall_left_detected       = false;
    grounded_detected        = false;
	grounded_owp_detected    = false;
	left_wj_eligible         = false;
	right_wj_eligible        = false;
	scythe_ground_2_buffered = false;
	scythe_ground_3_buffered = false;
	kill_player              = false;
	is_dead                  = false;

	hitbox_1_ptr = NULL;
	hitbox_2_ptr = NULL;
	hitbox_3_ptr = NULL;

	phase_dir = PHASE_RIGHT;
	pm_sprite_ptr = bn::sprite_items::phase_marker.create_sprite(0, 0);
	pm_sprite_ptr->set_z_order(PLAYER_Z_ORDER);
	pm_sprite_ptr->set_visible(false);
	
}

Player::Player(const Player& other) : GameObject(other)
{

	state             	 = other.state;
	dir                  = other.dir;
    x_speed        	  	 = other.x_speed;
    jump_force           = other.jump_force;
	secondary_jump_force = other.secondary_jump_force;
    wall_jump_force   	 = other.wall_jump_force;

    gravity           	 = other.gravity;
	wall_ride_gravity 	 = other.wall_ride_gravity;

	phase_destination    = other.phase_destination;
	
	remaining_jump_input_frames      = other.remaining_jump_input_frames;
	remaining_x_drift_lockout_frames = other.remaining_x_drift_lockout_frames;
	air_frames_elapsed               = other.air_frames_elapsed;
	v_collision_grace_frames         = other.v_collision_grace_frames;
	late_jump_grace_frames           = other.late_jump_grace_frames;
	current_scythe_frame             = other.current_scythe_frame;
	current_phase_frame              = other.current_phase_frame;
	current_death_frame              = other.current_death_frame;

	wall_right_detected      = other.wall_right_detected;
    wall_left_detected       = other.wall_left_detected;
    grounded_detected        = other.grounded_detected;
	grounded_owp_detected    = other.grounded_owp_detected;
	left_wj_eligible         = other.left_wj_eligible;
	right_wj_eligible        = other.right_wj_eligible;
	scythe_ground_2_buffered = other.scythe_ground_2_buffered;
	scythe_ground_3_buffered = other.scythe_ground_3_buffered;
	kill_player              = other.kill_player;
	is_dead                  = other.is_dead;

	test_collider         = other.test_collider;
	test_collider_right   = other.test_collider_right;
	test_collider_left    = other.test_collider_left;

	if(other.hitbox_1_ptr == NULL) {hitbox_1_ptr = NULL;}
	else {hitbox_1_ptr = new Hitbox(*(other.hitbox_1_ptr));}

	if(other.hitbox_2_ptr == NULL) {hitbox_2_ptr = NULL;}
	else {hitbox_2_ptr = new Hitbox(*(other.hitbox_2_ptr));}	
	
	if(other.hitbox_3_ptr == NULL) {hitbox_3_ptr = NULL;}
	else {hitbox_3_ptr = new Hitbox(*(other.hitbox_3_ptr));}

	phase_dir     = other.phase_dir;
	pm_sprite_ptr = other.pm_sprite_ptr;
	
}

Player::~Player()
{
	delete hitbox_1_ptr;
	delete hitbox_2_ptr;
	delete hitbox_3_ptr;	

	// Free the phase marker sprite
	pm_sprite_ptr.reset();
}

Player& Player::operator =(const Player& other)
{
	state             	 = other.state;
	dir                  = other.dir;
    x_speed        	  	 = other.x_speed;
    jump_force           = other.jump_force;
	secondary_jump_force = other.secondary_jump_force;
    wall_jump_force   	 = other.wall_jump_force;

    gravity           	 = other.gravity;
	wall_ride_gravity 	 = other.wall_ride_gravity;

	phase_destination    = other.phase_destination;
	
	remaining_jump_input_frames      = other.remaining_jump_input_frames;
	remaining_x_drift_lockout_frames = other.remaining_x_drift_lockout_frames;
	air_frames_elapsed               = other.air_frames_elapsed;
	v_collision_grace_frames         = other.v_collision_grace_frames;
	late_jump_grace_frames           = other.late_jump_grace_frames;
	current_scythe_frame             = other.current_scythe_frame;
	current_phase_frame              = other.current_phase_frame;
	current_death_frame              = other.current_death_frame;

	wall_right_detected      = other.wall_right_detected;
    wall_left_detected       = other.wall_left_detected;
    grounded_detected        = other.grounded_detected;
	grounded_owp_detected    = other.grounded_owp_detected;
	left_wj_eligible         = other.left_wj_eligible;
	right_wj_eligible        = other.right_wj_eligible;
	scythe_ground_2_buffered = other.scythe_ground_2_buffered;
	scythe_ground_3_buffered = other.scythe_ground_3_buffered;
	kill_player              = other.kill_player;
	is_dead                  = other.is_dead;

	test_collider         = other.test_collider;
	test_collider_right   = other.test_collider_right;
	test_collider_left    = other.test_collider_left;

	if(other.hitbox_1_ptr == NULL) {hitbox_1_ptr = NULL;}
	else {hitbox_1_ptr = new Hitbox(*(other.hitbox_1_ptr));}

	if(other.hitbox_2_ptr == NULL) {hitbox_2_ptr = NULL;}
	else {hitbox_2_ptr = new Hitbox(*(other.hitbox_2_ptr));}	
	
	if(other.hitbox_3_ptr == NULL) {hitbox_3_ptr = NULL;}
	else {hitbox_3_ptr = new Hitbox(*(other.hitbox_3_ptr));}

	phase_dir     = other.phase_dir;
	pm_sprite_ptr = other.pm_sprite_ptr;

	return *this;
}

void Player::update(const RoomBounds& 								  room_bounds,
					bn::vector<GameObject*, MAX_GAME_OBJECTS>&        game_objects,
					const bn::regular_bg_ptr&                         bg_ptr, 
                    const bn::span<const bn::regular_bg_map_cell>&    cells,
                    const bn::regular_bg_item&                        bg_item,
					const bn::camera_ptr&                             camera)
{
    
	bool clear_to_jump = true;

	//////////////////////////
    // Player State Machine //
    //////////////////////////
	
    switch(state)
    {
		case STATE_GROUNDED_NEUTRAL:

			///////////////////////////////////
			// Player Grounded Neutral State //
			///////////////////////////////////

			// Update walk speed //
			if(bn::keypad::left_released())        
			{
				rigidbody.addForce(PLAYER_X_LEFT_DECAY_FORCE);
				x_speed = PLAYER_MIN_X_SPEED;
			}
			else if (bn::keypad::right_released()) 
			{
				rigidbody.addForce(PLAYER_X_RIGHT_DECAY_FORCE);
				x_speed = PLAYER_MIN_X_SPEED;
			}

			// Simulate friction/momentum
			if((bn::keypad::left_held() || bn::keypad::right_held()))  
			{
				x_speed += X_SPEED_ACC_RATE;
				x_speed = clamp(PLAYER_MIN_X_SPEED, PLAYER_MAX_X_SPEED, x_speed);
			}

			// Get Input //
			
			// Walk
			if(bn::keypad::left_held())       
			{rigidbody.addForce(PLAYER_X_LEFT_FORCE); dir = LEFT;}

			else if(bn::keypad::right_held()) 
			{rigidbody.addForce(PLAYER_X_RIGHT_FORCE); dir = RIGHT;}

			// Scythe Ground 1
			if(bn::keypad::b_pressed())
			{setState(STATE_SCYTHE_GROUND_1);}

			// Jump
			else if(bn::keypad::a_pressed()) {jump();}

			// Add Gravity if Grounded on OWP
			if(grounded_owp_detected) {rigidbody.addForce(PLAYER_GRAVITY_FORCE);}

		break;
	
		case STATE_AIR_NEUTRAL:

			//////////////////////////////
			// Player Air Neutral State //
			//////////////////////////////

			// Update drift speed //

			// Simulate friction/momentum
			if((bn::keypad::left_held() || bn::keypad::right_held()) && !remaining_x_drift_lockout_frames)  
			{
				x_speed += X_SPEED_ACC_RATE;
				x_speed = clamp(PLAYER_MIN_X_SPEED, PLAYER_MAX_X_SPEED, x_speed);
			}

			// Simulate momentum
			if(bn::keypad::left_released())        
			{
				rigidbody.addForce(PLAYER_X_LEFT_DECAY_FORCE);
				x_speed = PLAYER_MIN_X_SPEED;
			}
			else if (bn::keypad::right_released()) 
			{
				rigidbody.addForce(PLAYER_X_RIGHT_DECAY_FORCE);
				x_speed = PLAYER_MIN_X_SPEED;
			}

			// Get Input //

			// Drift
			if(bn::keypad::left_held() && !remaining_x_drift_lockout_frames)       
			{rigidbody.addForce(PLAYER_X_LEFT_FORCE); dir = LEFT;}

			else if(bn::keypad::right_held() && !remaining_x_drift_lockout_frames)
			{rigidbody.addForce(PLAYER_X_RIGHT_FORCE); dir = RIGHT;}

			// Scythe Air 1
			if(bn::keypad::b_pressed())
			{setState(STATE_SCYTHE_AIR_1);}

			// Fast Fall
			if(bn::keypad::down_held() && 
			   rigidbody.normalized_dir.y() > 0 && 
			   air_frames_elapsed >= PLAYER_MIN_FAST_FALL_FRAMES)
			{fastFall();}
			
			// Wall Jump
			if(bn::keypad::a_pressed())
			{
				if(right_wj_eligible)
				{
					//if(bn::keypad::r_held())
					//{rigidbody.addForce(PLAYER_P_WALL_JUMP_LEFT_FORCE);}
					//else
					//{
					rigidbody.addForce(PLAYER_WALL_JUMP_LEFT_FORCE);
					//}
					
					sprite_ptr->set_vertical_scale(PLAYER_MAX_STRETCH_V);
					sprite_ptr->set_horizontal_scale(PLAYER_MIN_STRETCH_H);
					remaining_x_drift_lockout_frames = PLAYER_X_DRIFT_LOCKOUT_FRAMES;
					dir = LEFT;
				}
				else if(left_wj_eligible)
				{
					//if(bn::keypad::r_held())
					//{rigidbody.addForce(PLAYER_P_WALL_JUMP_RIGHT_FORCE);}
					//else
					//{
					rigidbody.addForce(PLAYER_WALL_JUMP_RIGHT_FORCE);
					//}
					
					sprite_ptr->set_vertical_scale(PLAYER_MAX_STRETCH_V);
					sprite_ptr->set_horizontal_scale(PLAYER_MIN_STRETCH_H);
					remaining_x_drift_lockout_frames = PLAYER_X_DRIFT_LOCKOUT_FRAMES;
					dir = RIGHT;
				}
			}

			// Late Jump
			if(bn::keypad::a_pressed() && late_jump_grace_frames) 
			{jump();}

			// High Jump
			if(bn::keypad::a_held() && remaining_jump_input_frames > 0)
			{rigidbody.addForce(PLAYER_SECONDARY_JUMP_FORCE);}

			else if(bn::keypad::a_released()) 
			{remaining_jump_input_frames = 0;}

			// Add Gravity //
			if(!remaining_x_drift_lockout_frames)
			{
				rigidbody.addForce(PLAYER_GRAVITY_FORCE);
				if(air_frames_elapsed >= PLAYER_PROLONGED_AIR_FRAMES_REQUIRED)
				{rigidbody.addForce(PLAYER_PROLONGED_GRAVITY_FORCE);}
			}

			// Update Remaining Jump Input Frames //
			remaining_jump_input_frames--;
			remaining_jump_input_frames = clamp(0, 
												PLAYER_MAX_JUMP_INPUT_FRAMES, 
												remaining_jump_input_frames);

			// Update X Drift Lockout Frames //
			remaining_x_drift_lockout_frames--;
			remaining_x_drift_lockout_frames = clamp(0, 
													PLAYER_X_DRIFT_LOCKOUT_FRAMES, 
													remaining_x_drift_lockout_frames);

			// Update Squish frames for squish eligibility // 
			air_frames_elapsed++;
			air_frames_elapsed = clamp(0, 
									   PLAYER_MAX_AIR_FRAMES, 
									   air_frames_elapsed);

			// Update grace frames for a late jump //
			late_jump_grace_frames--;
			late_jump_grace_frames = clamp(0, 
										PLAYER_LATE_JUMP_GRACE_FRAMES, 
										late_jump_grace_frames);

		break;

		case STATE_WALL_SLIDE_RIGHT:

			///////////////////////////////////
			// Player Wall Slide Right State //
			///////////////////////////////////

			// Simulate friction/momentum
			if(bn::keypad::left_held())  
			{
				x_speed += X_SPEED_ACC_RATE;
				x_speed = clamp(PLAYER_MIN_X_SPEED, PLAYER_MAX_X_SPEED, x_speed);
			}
	
			// Drift
			if(bn::keypad::left_held() && !remaining_x_drift_lockout_frames) 
			{rigidbody.addForce(PLAYER_X_LEFT_FORCE);}

			// Wall Jump
			if(bn::keypad::a_pressed())
			{
				rigidbody.removeForces();
				rigidbody.addForce(PLAYER_WALL_JUMP_LEFT_FORCE);

				sprite_ptr->set_vertical_scale(PLAYER_MAX_STRETCH_V);
				sprite_ptr->set_horizontal_scale(PLAYER_MIN_STRETCH_H);
				remaining_x_drift_lockout_frames = PLAYER_X_DRIFT_LOCKOUT_FRAMES;
				dir = LEFT;
			}
			
			// Add Gravity //
			rigidbody.addForce(PLAYER_WALL_GRAVITY_FORCE);

			// Update Squish frames for squish eligibility // 
			air_frames_elapsed++;
			air_frames_elapsed = clamp(0, 
									   PLAYER_MAX_AIR_FRAMES, 
									   air_frames_elapsed);
			
		break;
		
		case STATE_WALL_SLIDE_LEFT:

			//////////////////////////////////
			// Player Wall Slide Left State //
			//////////////////////////////////

			// Simulate friction/momentum
			if(bn::keypad::right_held())  
			{
				x_speed += X_SPEED_ACC_RATE;
				x_speed = clamp(PLAYER_MIN_X_SPEED, PLAYER_MAX_X_SPEED, x_speed);
			}

			// Drift
			if(bn::keypad::right_held() && !remaining_x_drift_lockout_frames)
			{rigidbody.addForce(PLAYER_X_RIGHT_FORCE);}

			// Wall Jump
			if(bn::keypad::a_pressed())
			{
				rigidbody.removeForces();
				rigidbody.addForce(PLAYER_WALL_JUMP_RIGHT_FORCE);

				sprite_ptr->set_vertical_scale(PLAYER_MAX_STRETCH_V);
				sprite_ptr->set_horizontal_scale(PLAYER_MIN_STRETCH_H);
				remaining_x_drift_lockout_frames = PLAYER_X_DRIFT_LOCKOUT_FRAMES;
				dir = RIGHT;
			}
			
			// Add Gravity //
			rigidbody.addForce(PLAYER_WALL_GRAVITY_FORCE);
			
			// Update Squish frames for squish eligibility // 
			air_frames_elapsed++;
			air_frames_elapsed = clamp(0, 
									   PLAYER_MAX_AIR_FRAMES, 
									   air_frames_elapsed);
		
		break;

		case STATE_PHASE_STEP:
			
			{
				// Get current cell index that player resides in:
				int32 half_level_width_pixels  = bg_ptr.dimensions().width()  / 2;
				int32 half_level_height_pixels = bg_ptr.dimensions().height() / 2;
				bn::fixed index_x = (x() + half_level_width_pixels  + collider_offset_x) / TILE_WIDTH;
				bn::fixed index_y = (y() + half_level_height_pixels + collider_offset_y) / TILE_HEIGHT;
				bn::point cell_index = bn::point(index_x.integer(), index_y.integer());

				Collider other_collider;

				// Check if player can cancel the phase:
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

						// 2. If the tile is collidable make a temporary collider based on type//

						if(tile_index >= HARD_BLOCK_MIN_INDEX && 
						   tile_index <= HARD_BLOCK_MAX_INDEX)
						{
							
							other_collider = Collider(world_x, 
														world_y, 
														TILE_WIDTH,
														TILE_HEIGHT);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}		
						
						else if(tile_index == LEFT_SHALLOW_SLOPE_1_INDEX)
						{

							other_collider = Collider(world_x, 
														world_y + 3, 
														TILE_WIDTH, 
														TILE_HEIGHT / 4);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}
							
						else if(tile_index == LEFT_SHALLOW_SLOPE_2_INDEX)
						{
							other_collider = Collider(world_x, 
														world_y + 2, 
														TILE_WIDTH, 
														TILE_HEIGHT / 2);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}

						else if(tile_index == LEFT_SHALLOW_SLOPE_3_INDEX)
						{

							other_collider = Collider(world_x, 
														world_y + 1, 
														TILE_WIDTH, 
														TILE_HEIGHT - 2);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}

						else if(tile_index == LEFT_SHALLOW_SLOPE_4_INDEX)
						{

							other_collider = Collider(world_x, 
														world_y, 
														TILE_WIDTH, 
														TILE_HEIGHT);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}

						else if(tile_index == LEFT_STEEP_SLOPE_1_INDEX)
						{
							
							other_collider = Collider(world_x, 
														world_y + 2, 
														TILE_WIDTH, 
														TILE_HEIGHT / 2);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}

						else if(tile_index == LEFT_STEEP_SLOPE_2_INDEX)
						{

							other_collider = Collider(world_x, 
														world_y, 
														TILE_WIDTH, 
														TILE_HEIGHT);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}

						else if(tile_index == RIGHT_SHALLOW_SLOPE_1_INDEX)
						{

							other_collider = Collider(world_x, 
														world_y + 3, 
														TILE_WIDTH, 
														TILE_HEIGHT / 4);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}

						else if(tile_index == RIGHT_SHALLOW_SLOPE_2_INDEX)
						{

							other_collider = Collider(world_x, 
														world_y + 2, 
														TILE_WIDTH, 
														TILE_HEIGHT / 2);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}

						else if(tile_index == RIGHT_SHALLOW_SLOPE_3_INDEX)
						{

							other_collider = Collider(world_x, 
														world_y + 1,
														TILE_WIDTH, 
														TILE_HEIGHT - 2);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}
						
						else if(tile_index == RIGHT_SHALLOW_SLOPE_4_INDEX)
						{

							other_collider = Collider(world_x, 
														world_y,
														TILE_WIDTH, 
														TILE_HEIGHT);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}
						
						else if(tile_index == RIGHT_STEEP_SLOPE_1_INDEX)
						{
						
							other_collider = Collider(world_x, 
														world_y + 2,
														TILE_WIDTH, 
														TILE_HEIGHT / 2);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}
						
						else if(tile_index == RIGHT_STEEP_SLOPE_2_INDEX)
						{

							other_collider = Collider(world_x, 
														world_y,
														TILE_WIDTH, 
														TILE_HEIGHT);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}
						
						/*
						else if(tile_index >= ONEWAY_BLOCK_MIN_INDEX &&
								tile_index <= ONEWAY_BLOCK_MAX_INDEX)
						{
							other_collider = Collider(world_x, 
														world_y + ONEWAYBLOCK_COLLIDER_Y_OFFSET, 
														TILE_WIDTH, 
														ONEWAYBLOCK_COLLIDER_HEIGHT);

							if(rigidbody.normalized_dir.y() >= 0 &&
								collider_y_axis.p4.y() <= other_collider.p1.y() + PLAYER_GRAVITY)
							{

								if(bn::keypad::down_held() && 
								(state == STATE_AIR_NEUTRAL || state == STATE_GROUNDED_NEUTRAL)) 
								{
									rigidbody.addForce(PLAYER_GRAVITY_FORCE);
								}
								else
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
						*/

					}
				}
			}

			// Early jump breakout
			if(bn::keypad::a_pressed() &&
			   current_phase_frame >= PLAYER_PHASE_JUMP_LOCKOUT_FRAMES && 
			   clear_to_jump)
			{
				// Jump
				jump();

				// Add force
				if(phase_dir == PHASE_LEFT)
				{rigidbody.addForce(PLAYER_PHASE_STEP_EXIT_FORCE_LEFT);}
				else if(phase_dir == PHASE_RIGHT)
				{rigidbody.addForce(PLAYER_PHASE_STEP_EXIT_FORCE_RIGHT);}

				// Set State
				setState(STATE_NO_STATE);

				// Hide Marker
				pm_sprite_ptr->set_visible(false);
			}

			// Keep moving towards phase destination until reached
			else
			{
				// Phasing Up
				if(phase_dir == PHASE_UP && y() >= phase_destination.y())
				{
					setY(y() - PLAYER_PHASE_STEP_SPEED);
					if(y() <= phase_destination.y()) 
					{
						setY(phase_destination.y());
						setState(STATE_NO_STATE);
						rigidbody.addForce(PLAYER_PHASE_STEP_EXIT_FORCE_UP);
						late_jump_grace_frames = 0;

						// Stretch
						sprite_ptr->set_horizontal_scale(PLAYER_MIN_STRETCH_H); 				
						sprite_ptr->set_vertical_scale(PLAYER_MAX_STRETCH_V);

						// Hide Marker
						pm_sprite_ptr->set_visible(false);
					}
				}

				// Phasing Down
				else if(phase_dir == PHASE_DOWN && y() <= phase_destination.y())
				{
					setY(y() + PLAYER_PHASE_STEP_SPEED);
					if(y() >= phase_destination.y()) 
					{
						setY(phase_destination.y());
						setState(STATE_NO_STATE);
						rigidbody.addForce(PLAYER_PHASE_STEP_EXIT_FORCE_DOWN);

						// Stretch
						sprite_ptr->set_horizontal_scale(PLAYER_MIN_STRETCH_H); 				
						sprite_ptr->set_vertical_scale(PLAYER_MAX_STRETCH_V);

						// Hide Marker
						pm_sprite_ptr->set_visible(false);
					}
				}

				// Phasing Left
				else if(phase_dir == PHASE_LEFT && x() >= phase_destination.x())
				{
					setX(x() - PLAYER_PHASE_STEP_SPEED);
					if(x() <= phase_destination.x()) 
					{
						setX(phase_destination.x());
						setState(STATE_NO_STATE);
						rigidbody.addForce(PLAYER_PHASE_STEP_EXIT_FORCE_LEFT);
						dir = LEFT;

						// Stretch
						sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H); 				
						sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);

						// Hide Marker
						pm_sprite_ptr->set_visible(false);
					}
				}

				// Phasing Right
				else if(phase_dir == PHASE_RIGHT && x() <= phase_destination.x())
				{
					setX(x() + PLAYER_PHASE_STEP_SPEED);
					if(x() >= phase_destination.x()) 
					{
						setX(phase_destination.x());
						setState(STATE_NO_STATE);
						rigidbody.addForce(PLAYER_PHASE_STEP_EXIT_FORCE_RIGHT);
						dir = RIGHT;

						// Stretch
						sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H); 				
						sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);

						// Hide Marker
						pm_sprite_ptr->set_visible(false);
					}
				}
			}

			// Increment phase frame counter
			current_phase_frame++;
			current_phase_frame = clamp(0, 
				                        PLAYER_PHASE_JUMP_LOCKOUT_FRAMES, 
										current_phase_frame);
			
		break;

		case STATE_SCYTHE_GROUND_1:

			// Increment Frame Counter
			current_scythe_frame++;
			current_scythe_frame = clamp(0, 
										 PLAYER_SCYTHE_GROUND_1_TOTAL_FRAMES,
										 current_scythe_frame);

			// Create Hitboxes
			if(current_scythe_frame == PLAYER_SCYTHE_GROUND_1_CREATE_HB_FRAME)
			{
				createGroundedScythe1Hitboxes(game_objects, camera);
			}

			// Take Buffer Input
			/*
			if(bn::keypad::b_pressed() && 
			   current_scythe_frame >= PLAYER_MIN_SCYTHE_GROUND_1_BUFFER_FRAMES)
			{scythe_ground_2_buffered = true;} 
			*/

			if(current_scythe_frame >= PLAYER_SCYTHE_GROUND_1_TOTAL_FRAMES)
			{
				/*
				if(scythe_ground_2_buffered) 
				{
					scythe_ground_2_buffered = false;
					setState(STATE_SCYTHE_GROUND_2);
				}
				else {setState(STATE_NO_STATE);}
				*/
				setState(STATE_NO_STATE);
			}

		break;

		case STATE_SCYTHE_GROUND_2:

			// Increment Frame Counter
			current_scythe_frame++;
			current_scythe_frame = clamp(0, 
										PLAYER_SCYTHE_GROUND_2_TOTAL_FRAMES,
										current_scythe_frame);

			// Create Hitboxes
			if(current_scythe_frame == PLAYER_SCYTHE_GROUND_2_CREATE_HB_FRAME &&
			game_objects.size() < MAX_GAME_OBJECTS)
			{
				//Create hitboxes here
			}

			// Take Buffer Input
			if(bn::keypad::b_pressed() && 
			current_scythe_frame >= PLAYER_MIN_SCYTHE_GROUND_2_BUFFER_FRAMES)
			{scythe_ground_3_buffered = true;} 

			if(current_scythe_frame >= PLAYER_SCYTHE_GROUND_2_TOTAL_FRAMES)
			{
				if(scythe_ground_3_buffered) 
				{
					scythe_ground_3_buffered = false;
					setState(STATE_SCYTHE_GROUND_3);
				}
				else
				{setState(STATE_NO_STATE);}
			}

		break;

		case STATE_SCYTHE_GROUND_3:

			// Increment Frame Counter
			current_scythe_frame++;
			current_scythe_frame = clamp(0, 
										PLAYER_SCYTHE_GROUND_3_TOTAL_FRAMES,
										current_scythe_frame);

			// Create Hitboxes
			if(current_scythe_frame == PLAYER_SCYTHE_GROUND_3_CREATE_HB_FRAME &&
			game_objects.size() < MAX_GAME_OBJECTS)
			{
				//Create hitboxes here
			}

			if(current_scythe_frame >= PLAYER_SCYTHE_GROUND_3_TOTAL_FRAMES)
			{
				setState(STATE_NO_STATE);
			}

		break;

		case STATE_SCYTHE_AIR_1:

			///////////////////////////////
			// Player Scythe Air 1 State //
			///////////////////////////////

			//////////////////
			// Scythe stuff //
			//////////////////

			// Create Hitboxes
			if(current_scythe_frame == PLAYER_SCYTHE_AIR_1_CREATE_HB_FRAME)
			{
				createAirScythe1Hitboxes(game_objects, camera);
			}

			if(current_scythe_frame >= PLAYER_SCYTHE_AIR_1_TOTAL_FRAMES)
			{setState(STATE_NO_STATE);}

			///////////////
			// Air stuff //
			///////////////

			// Update drift speed //

			// Simulate friction/momentum
			if((bn::keypad::left_held() || bn::keypad::right_held()) && !remaining_x_drift_lockout_frames)  
			{
				x_speed += X_SPEED_ACC_RATE;
				x_speed = clamp(PLAYER_MIN_X_SPEED, PLAYER_MAX_X_SPEED, x_speed);
			}

			// Simulate momentum
			if(bn::keypad::left_released())        
			{
				rigidbody.addForce(PLAYER_X_LEFT_DECAY_FORCE);
				x_speed = PLAYER_MIN_X_SPEED;
			}
			else if (bn::keypad::right_released()) 
			{
				rigidbody.addForce(PLAYER_X_RIGHT_DECAY_FORCE);
				x_speed = PLAYER_MIN_X_SPEED;
			}

			// Get Input //

			// Drift
			if(bn::keypad::left_held() && !remaining_x_drift_lockout_frames)       
			{rigidbody.addForce(PLAYER_X_LEFT_FORCE);}

			else if(bn::keypad::right_held() && !remaining_x_drift_lockout_frames)
			{rigidbody.addForce(PLAYER_X_RIGHT_FORCE);}
			
			// High Jump
			if(bn::keypad::a_held() && remaining_jump_input_frames > 0)
			{rigidbody.addForce(PLAYER_SECONDARY_JUMP_FORCE);}

			else if(bn::keypad::a_released()) 
			{remaining_jump_input_frames = 0;}

			// Add Gravity //
			if(!remaining_x_drift_lockout_frames)
			{
				rigidbody.addForce(PLAYER_GRAVITY_FORCE);
				if(air_frames_elapsed >= PLAYER_PROLONGED_AIR_FRAMES_REQUIRED)
				{rigidbody.addForce(PLAYER_PROLONGED_GRAVITY_FORCE);}
			}

			// Update Remaining Jump Input Frames //
			remaining_jump_input_frames--;
			remaining_jump_input_frames = clamp(0, 
												PLAYER_MAX_JUMP_INPUT_FRAMES, 
												remaining_jump_input_frames);

			// Update X Drift Lockout Frames //
			remaining_x_drift_lockout_frames--;
			remaining_x_drift_lockout_frames = clamp(0, 
													PLAYER_X_DRIFT_LOCKOUT_FRAMES, 
													remaining_x_drift_lockout_frames);

			// Update Squish frames for squish eligibility // 
			air_frames_elapsed++;
			air_frames_elapsed = clamp(0, 
									   PLAYER_MAX_AIR_FRAMES, 
									   air_frames_elapsed);

			// Update grace frames for a late jump //
			late_jump_grace_frames--;
			late_jump_grace_frames = clamp(0, 
										PLAYER_LATE_JUMP_GRACE_FRAMES, 
										late_jump_grace_frames);

			// Increment Frame Counter
			current_scythe_frame++;
			current_scythe_frame = clamp(0, 
											PLAYER_SCYTHE_AIR_1_TOTAL_FRAMES,
											current_scythe_frame);

		break;

		case STATE_DYING:

			current_death_frame++;
			current_death_frame = clamp(0, 
										PLAYER_DEATH_FRAMES,
										current_death_frame);

			if(current_death_frame >= PLAYER_DEATH_FRAMES)
			{is_dead = true;}

		break;
		
		default:
		break;
    }
	
	// Reset walljump variables
	left_wj_eligible  = false;
	right_wj_eligible = false;
	
    ///////////////////
    // Apply Physics //
    ///////////////////
    
	// Apply Decay to Forces
	rigidbody.applyDecay();

	// Apply forces to player
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

	// Update colliders for each axis. 
	collider_x_axis.setPos(collider.x(), collider.y() - rigidbody.final_dir.y());
	collider_y_axis.setPos(collider.x() - rigidbody.final_dir.x(), collider.y());

	// Placeholder for other objects
	Collider other_collider;

	//////////////////////////////////
	// Resolve GameObject Collision //
	//////////////////////////////////

	if(state != STATE_PHASE_STEP)
	{
		for(int32 i = 0; i < game_objects.size(); i++)
		{
			other_collider = game_objects.at(i)->collider;
			
			bn::fixed col_x_offset;
			bn::fixed col_y_offset;

			switch(game_objects.at(i)->object_type)
			{
				case DEVIL_PLATFORM:
					
					if(collider.isCollision(other_collider))
					{
						// Handle Default Collision Cases //
						while(collider_x_axis.isCollision(other_collider))
						{
							if(rigidbody.normalized_dir.x() == 0) {kill_player = true; break;}
							collider_x_axis.setX(collider_x_axis.x() - rigidbody.normalized_dir.x());
							setX(this->x() - rigidbody.normalized_dir.x());
						}

						while(collider_y_axis.isCollision(other_collider))
						{
							if(rigidbody.normalized_dir.y() == 0) {kill_player = true; break;}
							collider_y_axis.setY(collider_y_axis.y() - rigidbody.normalized_dir.y());
							setY(this->y() - rigidbody.normalized_dir.y());
						}

						// If there is still collision somehow, must be corner case //
						while(collider.isCollision(other_collider))
						{
							if(rigidbody.normalized_dir.x() == 0) {kill_player = true; break;}
							// We always resolve diagonal corner collisions with a horizontal shift. 
							setX(this->x() - rigidbody.normalized_dir.x());
						}	
					}

				break;

				case ANGEL_PLATFORM:

					if(collider_y_axis.p4.y() <= other_collider.p1.y() + PLAYER_GRAVITY)
					{

						// Handle Corner Case //
						if(!collider_x_axis.isCollision(other_collider) &&
						!collider_y_axis.isCollision(other_collider))
						{
							while(collider.isCollision(other_collider))
							{setY(this->y() - 1);}
						}
					
						// Handle Remaining Collision Cases //
						else
						{

							while(collider_y_axis.isCollision(other_collider))
							{collider_y_axis.setY(collider_y_axis.y() - 1);
								setY(this->y() - 1);}
						}
					}
					
				break;

				case SCYTHE_PLATFORM:
					
					if(collider_y_axis.p4.y() <= other_collider.p1.y() + PLAYER_GRAVITY)
					{
						if(bn::keypad::down_held())       {break;}
						else if(v_collision_grace_frames) {break;}

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
						v_collision_grace_frames = PLAYER_V_COLLISION_MAX_GRACE_FRAMES * col_y_offset.integer();

						// If there is still collision somehow, must be corner case //
						while(collider.isCollision(other_collider))
						{
							if(rigidbody.normalized_dir.x() == 0) {kill_player = true; break;}
							// We always resolve diagonal corner collisions with a horizontal shift. 
							setX(this->x() - rigidbody.normalized_dir.x());
						}
					}

				break;

				default:
				break;
			}
		}
	}

	////////////////////////////
    // Resolve Tile Collision //
    ////////////////////////////
	
	if(state != STATE_PHASE_STEP)
	{
		// Check cells for collision in the direction the player is facing
		int32 x_check_dir; 
		if(dir == LEFT) {x_check_dir = -1;}
		else {x_check_dir = 1;}

		for(int32 y = -2; y < 3; y++)
		{
			for(int32 x = 1 * (x_check_dir * -1); x != 2 * x_check_dir; x += x_check_dir)
			{
				
				// 1. Get tile type at index //
				int32 check_index_x = cell_index.x() + x;
				int32 check_index_y = cell_index.y() + y;

				// Determine world coords in case we need to make a collider.
				int32 world_x = ((check_index_x * TILE_WIDTH)  - half_level_width_pixels)  + (TILE_WIDTH / 2);
				int32 world_y = ((check_index_y * TILE_HEIGHT) - half_level_height_pixels) + (TILE_HEIGHT / 2);

				uint32 tile_index = getTileAtBGIndex(check_index_x, 
													 check_index_y, 
													 bg_ptr, 
													 cells, 
													 bg_item);

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
					if(getTileAtBGIndex(check_index_x + x_check_dir, check_index_y, 
										bg_ptr, cells, bg_item) >= HARD_BLOCK_MIN_INDEX && 
						getTileAtBGIndex(check_index_x + x_check_dir, check_index_y, 
										bg_ptr, cells, bg_item) <= HARD_BLOCK_MAX_INDEX)
					{
						block_w_offset = TILE_WIDTH;
						block_x_offset = (TILE_WIDTH / 2) * x_check_dir;
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
						v_collision_grace_frames = PLAYER_V_COLLISION_MAX_GRACE_FRAMES * col_y_offset.integer();

						// If there is still collision somehow, must be corner case //
						while(collider.isCollision(other_collider))
						{
							if(rigidbody.normalized_dir.x() == 0) {kill_player = true; break;}
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
						setY(global_height - (PLAYER_COLLIDER_HEIGHT / 2) - PLAYER_COLLIDER_OFFSET_Y);
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
						setY(global_height - (PLAYER_COLLIDER_HEIGHT / 2) - PLAYER_COLLIDER_OFFSET_Y);
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
						setY(global_height - (PLAYER_COLLIDER_HEIGHT / 2) - PLAYER_COLLIDER_OFFSET_Y);
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
						setY(global_height - (PLAYER_COLLIDER_HEIGHT / 2) - PLAYER_COLLIDER_OFFSET_Y);
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
						setY(global_height - (PLAYER_COLLIDER_HEIGHT / 2) - PLAYER_COLLIDER_OFFSET_Y);
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
						setY(global_height - (PLAYER_COLLIDER_HEIGHT / 2) - PLAYER_COLLIDER_OFFSET_Y);
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
						setY(global_height - (PLAYER_COLLIDER_HEIGHT / 2) - PLAYER_COLLIDER_OFFSET_Y);
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
						setY(global_height - (PLAYER_COLLIDER_HEIGHT / 2) - PLAYER_COLLIDER_OFFSET_Y);
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
						setY(global_height - (PLAYER_COLLIDER_HEIGHT / 2) - PLAYER_COLLIDER_OFFSET_Y);
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
						setY(global_height - (PLAYER_COLLIDER_HEIGHT / 2) - PLAYER_COLLIDER_OFFSET_Y);
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
						setY(global_height - (PLAYER_COLLIDER_HEIGHT / 2) - PLAYER_COLLIDER_OFFSET_Y);
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
						setY(global_height - (PLAYER_COLLIDER_HEIGHT / 2) - PLAYER_COLLIDER_OFFSET_Y);
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
						collider_y_axis.p4.y() <= other_collider.p1.y() + PLAYER_GRAVITY)
					{

						if(bn::keypad::down_held() && 
						(state == STATE_AIR_NEUTRAL || state == STATE_GROUNDED_NEUTRAL)) 
						{
							rigidbody.addForce(PLAYER_GRAVITY_FORCE);
						}
						else
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
		}
	}
	
	////////////////////////////////////////
	// Initialize State Testing Variables //
	////////////////////////////////////////

	// Initialize state variables, to be updated on collision.
    wall_right_detected   = false;
    wall_left_detected    = false;
    grounded_detected     = false;
	grounded_owp_detected = false;

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

	/////////////////////////////////////
	// Get State Info from GameObjects //
	/////////////////////////////////////

	if(state != STATE_PHASE_STEP)
	{
		for(int32 i = 0; i < game_objects.size(); i++)
		{
			other_collider = game_objects.at(i)->collider;

			switch(game_objects.at(i)->object_type)
			{
				case DEVIL_PLATFORM:

					// Test for, and log grounded collision
					if(test_collider.isCollision(other_collider) && 
					rigidbody.normalized_dir.y() >= 0)
					{
						if(air_frames_elapsed >= PLAYER_SQUISH_FRAMES_REQUIRED) 
						{sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H); 				
						sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);}
						grounded_detected = true;
					}

					// Test for wall riding on right side
					if(test_collider_right.isCollision(other_collider) && 
					rigidbody.final_dir.y() >= 0)
					{wall_right_detected = true;}
					
					// Test for wall riding on left side
					if(test_collider_left.isCollision(other_collider) && 
					rigidbody.final_dir.y() >= 0)
					{wall_left_detected = true;}

				break;

				case ANGEL_PLATFORM:
				case SCYTHE_PLATFORM:
					
					if(collider_y_axis.p4.y() <= other_collider.p1.y() + PLAYER_GRAVITY)
					{
						// Test for, and log grounded collision
						if(test_collider.isCollision(other_collider) && 
						rigidbody.normalized_dir.y() >= 0)
						{ 
							if(!bn::keypad::down_held()) 
							{
								grounded_detected     = true;
								grounded_owp_detected = true;
								if(air_frames_elapsed >= PLAYER_SQUISH_FRAMES_REQUIRED)
								{sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);
								sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);}
							}
						}
					}
					
				break;

				case PHASE_ORB_UP:
				
					if(state != STATE_PHASE_STEP && collider.isCollision(other_collider))
					{
						phase_destination = ((PhaseOrb*)(game_objects.at(i)))->phase_destination;
						phase_dir = PHASE_UP;
						setPos(game_objects.at(i)->pos());
						setState(STATE_PHASE_STEP);
					}

				break;

				case PHASE_ORB_DOWN:
				
					if(state != STATE_PHASE_STEP && collider.isCollision(other_collider))
					{
						phase_destination = ((PhaseOrb*)(game_objects.at(i)))->phase_destination;
						phase_dir = PHASE_DOWN;
						setPos(game_objects.at(i)->pos());
						setState(STATE_PHASE_STEP);
					}

				break;

				case PHASE_ORB_LEFT:

					if(state != STATE_PHASE_STEP && collider.isCollision(other_collider))
					{
						phase_destination = ((PhaseOrb*)(game_objects.at(i)))->phase_destination;
						phase_dir = PHASE_LEFT;
						setPos(game_objects.at(i)->pos());
						setState(STATE_PHASE_STEP);
					}

				break;

				case PHASE_ORB_RIGHT:

					if(state != STATE_PHASE_STEP && collider.isCollision(other_collider))
					{
						phase_destination = ((PhaseOrb*)(game_objects.at(i)))->phase_destination;
						phase_dir = PHASE_RIGHT;
						setPos(game_objects.at(i)->pos());
						setState(STATE_PHASE_STEP);
					}

				break;

				case GROUND_GHOUL:
				case CEILING_GHOUL:
				case WALL_LEFT_GHOUL:
				case WALL_RIGHT_GHOUL:
				
					if(collider.isCollision(other_collider))	
					{
						rigidbody.removeForces();
						kill_player = true;
					}

				break;

				case TILE_PASSAGE:

					// Test for, and log grounded collision
					if(((TilePassage*)(game_objects.at(i)))->state == TILE_PASSAGE_STATE_SHUT &&
					test_collider.isCollision(other_collider) && 
					rigidbody.normalized_dir.y() >= 0)
					{
						if(rigidbody.final_dir.y() >= PLAYER_MIN_PASSAGE_SPEED)
						{((TilePassage*)(game_objects.at(i)))->setState(TILE_PASSAGE_STATE_OPEN);}
						else 
						{
							grounded_detected = true;
							if(air_frames_elapsed >= PLAYER_SQUISH_FRAMES_REQUIRED)
							{
								sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H); 				
								sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);
							}
						}
					}

				break;

				default:
				break;
			}
		}
	}

	///////////////////////////////
    // Get State Info from Tiles //
    ///////////////////////////////	

	if(state != STATE_PHASE_STEP)
	{
		for(int32 y = -2; y < 3; y++)
		{
			for(int32 x = -2; x < 3; x++)
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
						if(air_frames_elapsed >= PLAYER_SQUISH_FRAMES_REQUIRED &&
						rigidbody.final_dir.y() >= PLAYER_SQUISH_SPEED_REQUIRED)
						{
							sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H); 				
							sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);
						}

						grounded_detected = true;
					}

					// Test for wall riding on right side
					if(test_collider_right.isCollision(other_collider))
					{
						right_wj_eligible = true;

						if(rigidbody.normalized_dir.y() >= 0 &&
						   bn::keypad::right_held() && 
						  !bn::keypad::down_held()) 
						{
							wall_right_detected = true;
						}
					}
					
					// Test for wall riding on left side
					if(test_collider_left.isCollision(other_collider))
					{
						left_wj_eligible = true;

						if(rigidbody.normalized_dir.y() >= 0 &&
						   bn::keypad::left_held() &&
						  !bn::keypad::down_held()) 
						{
							wall_left_detected = true;
						}
					}

				}
						
				else if(tile_index >= ONEWAY_BLOCK_MIN_INDEX &&
						tile_index <= ONEWAY_BLOCK_MAX_INDEX)
				{
				
					other_collider = Collider(world_x, 
												world_y + ONEWAYBLOCK_COLLIDER_Y_OFFSET, 
												TILE_WIDTH, 
												ONEWAYBLOCK_COLLIDER_HEIGHT);

					if(collider_y_axis.p4.y() <= other_collider.p1.y() + PLAYER_GRAVITY)
					{
						// Test for, and log grounded collision
						if(test_collider.isCollision(other_collider) && 
							rigidbody.normalized_dir.y() >= 0)
						{
							if(!bn::keypad::down_held()) 
							{
								grounded_detected = true;
								if(air_frames_elapsed >= PLAYER_SQUISH_FRAMES_REQUIRED &&
								rigidbody.final_dir.y() >= PLAYER_SQUISH_SPEED_REQUIRED)
								{sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);
								sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);}
							}
						}
					}
				}

				else if(tile_index == UP_SPIKE_BLOCK_1_INDEX ||
						tile_index == UP_SPIKE_BLOCK_2_INDEX)
				{
					other_collider = Collider(world_x,
												world_y, 
												TILE_WIDTH, 
												TILE_HEIGHT);
					
					if(collider.isCollision(other_collider) && !kill_player)
					{
						rigidbody.removeForces();
						rigidbody.addForce(Force(bn::fixed_point_t<12>(PLAYER_DEATH_X_FORCE * 0, 
																		PLAYER_DEATH_Y_FORCE * UP),
																		PLAYER_DEATH_DECAY));
						kill_player = true;
					}
				}

				else if(tile_index == DOWN_SPIKE_BLOCK_1_INDEX ||
						tile_index == DOWN_SPIKE_BLOCK_2_INDEX)
				{
					other_collider = Collider(world_x,
											world_y, 
											TILE_WIDTH, 
											TILE_HEIGHT);
					
					if(collider.isCollision(other_collider) && !kill_player)
					{
						rigidbody.removeForces();
						rigidbody.addForce(Force(bn::fixed_point_t<12>(PLAYER_DEATH_X_FORCE * 0, 
																		PLAYER_DEATH_Y_FORCE * DOWN), 
																		PLAYER_DEATH_DECAY));
						kill_player = true;
					}
				}

				else if(tile_index == LEFT_SPIKE_BLOCK_1_INDEX ||
						tile_index == LEFT_SPIKE_BLOCK_2_INDEX)
				{
					other_collider = Collider(world_x,
												world_y, 
												TILE_WIDTH, 
												TILE_HEIGHT);
					
					if(collider.isCollision(other_collider) && !kill_player)
					{
						rigidbody.removeForces();
						rigidbody.addForce(Force(bn::fixed_point_t<12>(PLAYER_DEATH_X_FORCE * LEFT,
																		PLAYER_DEATH_Y_FORCE * 0), 
																		PLAYER_DEATH_DECAY));
						kill_player = true;
					}
				}

				else if(tile_index == RIGHT_SPIKE_BLOCK_1_INDEX || 
						tile_index == RIGHT_SPIKE_BLOCK_2_INDEX)
				{
					other_collider = Collider(world_x,
												world_y, 
												TILE_WIDTH,
												TILE_HEIGHT);
					
					if(collider.isCollision(other_collider) && !kill_player)
					{
						rigidbody.removeForces();
						rigidbody.addForce(Force(bn::fixed_point_t<12>(PLAYER_DEATH_X_FORCE * RIGHT,
																		PLAYER_DEATH_Y_FORCE * 0), 
																		PLAYER_DEATH_DECAY));
						kill_player = true;
					}
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
						if(air_frames_elapsed >= PLAYER_SQUISH_FRAMES_REQUIRED &&
							rigidbody.final_dir.y() >= PLAYER_SQUISH_SPEED_REQUIRED)
						{
							sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);				
							sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);
						}

						grounded_detected = true;

						// Offset the decline of the slope if player is moving with it.
						// This avoids a frame in air state when descending slopes.
						if(rigidbody.normalized_dir.x() == -1)
						{rigidbody.addForce(PLAYER_GRAVITY_FORCE);}
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
						if(air_frames_elapsed >= PLAYER_SQUISH_FRAMES_REQUIRED &&
							rigidbody.final_dir.y() >= PLAYER_SQUISH_SPEED_REQUIRED)
						{
							sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);				
							sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);
						}

						grounded_detected = true;

						// Offset the decline of the slope if player is moving with it.
						// This avoids a frame in air state when descending slopes.
						if(rigidbody.normalized_dir.x() == -1)
						{rigidbody.addForce(PLAYER_GRAVITY_FORCE);}
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
						if(air_frames_elapsed >= PLAYER_SQUISH_FRAMES_REQUIRED &&
							rigidbody.final_dir.y() >= PLAYER_SQUISH_SPEED_REQUIRED)
						{
							sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);				
							sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);
						}

						grounded_detected = true;

						// Offset the decline of the slope if player is moving with it.
						// This avoids a frame in air state when descending slopes.
						if(rigidbody.normalized_dir.x() == -1)
						{rigidbody.addForce(PLAYER_GRAVITY_FORCE);}
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
						if(air_frames_elapsed >= PLAYER_SQUISH_FRAMES_REQUIRED &&
							rigidbody.final_dir.y() >= PLAYER_SQUISH_SPEED_REQUIRED)
						{
							sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);				
							sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);
						}

						grounded_detected = true;

						// Offset the decline of the slope if player is moving with it.
						// This avoids a frame in air state when descending slopes.
						if(rigidbody.normalized_dir.x() == -1)
						{rigidbody.addForce(PLAYER_GRAVITY_FORCE);}
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
						if(air_frames_elapsed >= PLAYER_SQUISH_FRAMES_REQUIRED &&
							rigidbody.final_dir.y() >= PLAYER_SQUISH_SPEED_REQUIRED)
						{
							sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);				
							sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);
						}

						grounded_detected = true;

						// Offset the decline of the slope if player is moving with it.
						// This avoids a frame in air state when descending slopes.
						if(rigidbody.normalized_dir.x() == -1)
						{rigidbody.addForce(PLAYER_GRAVITY_FORCE);}
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
						if(air_frames_elapsed >= PLAYER_SQUISH_FRAMES_REQUIRED &&
							rigidbody.final_dir.y() >= PLAYER_SQUISH_SPEED_REQUIRED)
						{
							sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);				
							sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);
						}

						grounded_detected = true;

						// Offset the decline of the slope if player is moving with it.
						// This avoids a frame in air state when descending slopes.
						if(rigidbody.normalized_dir.x() == -1)
						{rigidbody.addForce(PLAYER_GRAVITY_FORCE);}
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
						collider.p1.y() + PLAYER_COLLIDER_HEIGHT > global_height)
					{
						if(air_frames_elapsed >= PLAYER_SQUISH_FRAMES_REQUIRED &&
							rigidbody.final_dir.y() >= PLAYER_SQUISH_SPEED_REQUIRED)
						{
							sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);				
							sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);
						}

						grounded_detected = true;

						if(rigidbody.normalized_dir.x() == 1)
						{rigidbody.addForce(PLAYER_GRAVITY_FORCE);}
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
						collider.p1.y() + PLAYER_COLLIDER_HEIGHT > global_height)
					{
						if(air_frames_elapsed >= PLAYER_SQUISH_FRAMES_REQUIRED &&
							rigidbody.final_dir.y() >= PLAYER_SQUISH_SPEED_REQUIRED)
						{
							sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);				
							sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);
						}

						grounded_detected = true;

						if(rigidbody.normalized_dir.x() == 1)
						{rigidbody.addForce(PLAYER_GRAVITY_FORCE);}
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
						collider.p1.y() + PLAYER_COLLIDER_HEIGHT > global_height)
					{
						if(air_frames_elapsed >= PLAYER_SQUISH_FRAMES_REQUIRED &&
							rigidbody.final_dir.y() >= PLAYER_SQUISH_SPEED_REQUIRED)
						{
							sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);				
							sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);
						}

						grounded_detected = true;

						if(rigidbody.normalized_dir.x() == 1)
						{rigidbody.addForce(PLAYER_GRAVITY_FORCE);}
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
						collider.p1.y() + PLAYER_COLLIDER_HEIGHT > global_height)
					{
						if(air_frames_elapsed >= PLAYER_SQUISH_FRAMES_REQUIRED &&
							rigidbody.final_dir.y() >= PLAYER_SQUISH_SPEED_REQUIRED)
						{
							sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);				
							sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);
						}

						grounded_detected = true;

						if(rigidbody.normalized_dir.x() == 1)
						{rigidbody.addForce(PLAYER_GRAVITY_FORCE);}
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
						collider.p1.y() + PLAYER_COLLIDER_HEIGHT > global_height)
					{
						if(air_frames_elapsed >= PLAYER_SQUISH_FRAMES_REQUIRED &&
							rigidbody.final_dir.y() >= PLAYER_SQUISH_SPEED_REQUIRED)
						{
							sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);				
							sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);
						}

						grounded_detected = true;

						if(rigidbody.normalized_dir.x() == 1)
						{rigidbody.addForce(PLAYER_GRAVITY_FORCE);}
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
						collider.p1.y() + PLAYER_COLLIDER_HEIGHT > global_height)
					{
						if(air_frames_elapsed >= PLAYER_SQUISH_FRAMES_REQUIRED &&
							rigidbody.final_dir.y() >= PLAYER_SQUISH_SPEED_REQUIRED)
						{
							sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);				
							sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);
						}

						grounded_detected = true;

						// Offset the decline of the slope if player is moving with it.
						// This avoids a frame in air state when descending slopes.
						if(rigidbody.normalized_dir.x() == 1)
						{rigidbody.addForce(PLAYER_GRAVITY_FORCE);}
					}
				}	
			}
		}
	}

	////////////////////
	// Clamp Position //
	////////////////////

	bn::fixed new_x = x();
	bn::fixed new_y = y();
	new_x = clamp(-half_level_width_pixels,  half_level_width_pixels,  new_x);
	new_y = clamp(-half_level_height_pixels, half_level_height_pixels, new_y);
	setPos(new_x, new_y);

	#define Y_KILL_BUFFER 32
	if(y() > half_level_height_pixels - Y_KILL_BUFFER) {kill_player = true;}

	/////////////////////
	// Update Hitboxes //
	/////////////////////

	if(hitbox_1_ptr != NULL)
	{

		hitbox_1_ptr->setPos(bn::point(x().integer() + (PLAYER_SCYTHE_GROUND_1_X_OFFSET * dir),
									   y().integer() + PLAYER_SCYTHE_GROUND_1_Y_OFFSET));
		hitbox_1_ptr->update(room_bounds,
						     game_objects,
							 bg_ptr, 
							 cells,
							 bg_item,
							 camera);
		hitbox_1_ptr->draw();

		// Hitbox Collision Check //
		for(int32 i = 0; i < game_objects.size(); i++)
		{
			other_collider = game_objects.at(i)->collider;

			switch(game_objects.at(i)->object_type)
			{
				case GROUND_GHOUL:
					
					if(hitbox_1_ptr->collider.isCollision(other_collider))
					{
						//((GroundGhoul*)(game_objects.at(i)))->setState(GROUND_GHOUL_DEATH_STATE);
						game_objects.at(i)->setHitFlash(GAME_OBJECT_MAX_HIT_FLASH_FRAMES);
					}
				
				break;

				case CEILING_GHOUL:
					
					if(hitbox_1_ptr->collider.isCollision(other_collider))
					{
						//((CeilingGhoul*)(game_objects.at(i)))->setState(CEILING_GHOUL_DEATH_STATE);
						game_objects.at(i)->setHitFlash(GAME_OBJECT_MAX_HIT_FLASH_FRAMES);
					}
			
				break;

				case WALL_LEFT_GHOUL:
					
					if(hitbox_1_ptr->collider.isCollision(other_collider))
					{
						//((WallLeftGhoul*)(game_objects.at(i)))->setState(WALL_LEFT_GHOUL_DEATH_STATE);
						game_objects.at(i)->setHitFlash(GAME_OBJECT_MAX_HIT_FLASH_FRAMES);
					}
		
				break;

				case WALL_RIGHT_GHOUL:
						
					if(hitbox_1_ptr->collider.isCollision(other_collider))
					{
						//((WallRightGhoul*)(game_objects.at(i)))->setState(WALL_RIGHT_GHOUL_DEATH_STATE);
						game_objects.at(i)->setHitFlash(GAME_OBJECT_MAX_HIT_FLASH_FRAMES);
					}
	
				break;

				case PHASE_ORB_UP:
				case PHASE_ORB_DOWN:
				case PHASE_ORB_LEFT:
				case PHASE_ORB_RIGHT:
				case TILE_PASSAGE:
				case DEVIL_PLATFORM:
				case ANGEL_PLATFORM:
				case SCYTHE_PLATFORM:	
				default:
				break;
			}
		}

		if(hitbox_1_ptr->is_inactive) 
		{
			delete hitbox_1_ptr;
			hitbox_1_ptr = NULL;
		}	

	}
	
	if(hitbox_2_ptr != NULL) 
	{

		hitbox_2_ptr->setPos(bn::point(x().integer() + (PLAYER_SCYTHE_GROUND_2_X_OFFSET * dir),
									   y().integer() + PLAYER_SCYTHE_GROUND_2_Y_OFFSET));
		hitbox_2_ptr->update(room_bounds,
							 game_objects,
							 bg_ptr, 
                    		 cells,
                    		 bg_item,
							 camera);
		hitbox_2_ptr->draw();

		// Hitbox Collision Check //
		for(int32 i = 0; i < game_objects.size(); i++)
		{
			other_collider = game_objects.at(i)->collider;

			switch(game_objects.at(i)->object_type)
			{
				case GROUND_GHOUL:
					
					if(hitbox_2_ptr->collider.isCollision(other_collider))
					{
						((GroundGhoul*)(game_objects.at(i)))->setState(GROUND_GHOUL_DEATH_STATE);
					}
				
				break;

				case CEILING_GHOUL:
					
					if(hitbox_2_ptr->collider.isCollision(other_collider))
					{
						((CeilingGhoul*)(game_objects.at(i)))->setState(CEILING_GHOUL_DEATH_STATE);
					}
		
				break;

				case WALL_LEFT_GHOUL:
					
					if(hitbox_2_ptr->collider.isCollision(other_collider))
					{
						((WallLeftGhoul*)(game_objects.at(i)))->setState(WALL_LEFT_GHOUL_DEATH_STATE);
					}
		
				break;

				case WALL_RIGHT_GHOUL:
						
					if(hitbox_2_ptr->collider.isCollision(other_collider))
					{
						((WallRightGhoul*)(game_objects.at(i)))->setState(WALL_RIGHT_GHOUL_DEATH_STATE);
					}

				break;

				case PHASE_ORB_UP:
				case PHASE_ORB_DOWN:
				case PHASE_ORB_LEFT:
				case PHASE_ORB_RIGHT:
				case TILE_PASSAGE:
				case DEVIL_PLATFORM:
				case ANGEL_PLATFORM:
				case SCYTHE_PLATFORM:	
				default:
				break;
			}
		}

		if(hitbox_2_ptr->is_inactive) 
		{
			delete hitbox_2_ptr;
			hitbox_2_ptr = NULL;
		}

	}
	
	if(hitbox_3_ptr != NULL)
	{
		hitbox_3_ptr->setPos(bn::point(x().integer() + (PLAYER_SCYTHE_GROUND_3_X_OFFSET * dir),
									   y().integer() + PLAYER_SCYTHE_GROUND_3_Y_OFFSET));

		hitbox_3_ptr->update(room_bounds,
							 game_objects,
							 bg_ptr, 
                    		 cells,
                    		 bg_item,
							 camera);
		hitbox_3_ptr->draw();

		for(int32 i = 0; i < game_objects.size(); i++)
		{
			other_collider = game_objects.at(i)->collider;

			switch(game_objects.at(i)->object_type)
			{
				case GROUND_GHOUL:
					
					if(hitbox_3_ptr->collider.isCollision(other_collider))
					{
						((GroundGhoul*)(game_objects.at(i)))->setState(GROUND_GHOUL_DEATH_STATE);
					}
				
				break;

				case CEILING_GHOUL:
					
					if(hitbox_3_ptr->collider.isCollision(other_collider))
					{
						((CeilingGhoul*)(game_objects.at(i)))->setState(CEILING_GHOUL_DEATH_STATE);
					}
		
				break;

				case WALL_LEFT_GHOUL:
					
					if(hitbox_3_ptr->collider.isCollision(other_collider))
					{
						((WallLeftGhoul*)(game_objects.at(i)))->setState(WALL_LEFT_GHOUL_DEATH_STATE);
					}
		
				break;

				case WALL_RIGHT_GHOUL:
						
					if(hitbox_3_ptr->collider.isCollision(other_collider))
					{
						((WallRightGhoul*)(game_objects.at(i)))->setState(WALL_RIGHT_GHOUL_DEATH_STATE);
					}

				break;

				case PHASE_ORB_UP:
				case PHASE_ORB_DOWN:
				case PHASE_ORB_LEFT:
				case PHASE_ORB_RIGHT:
				case TILE_PASSAGE:
				case DEVIL_PLATFORM:
				case ANGEL_PLATFORM:
				case SCYTHE_PLATFORM:	
				default:
				break;
			}
		}

		// Hitbox Collision Check //
		if(hitbox_3_ptr->is_inactive) 
		{
			delete hitbox_3_ptr;
			hitbox_3_ptr = NULL;
		}
	}
	
    ///////////////////
    // Update States //
    ///////////////////

	if(state != STATE_SCYTHE_GROUND_1 && 
	   state != STATE_SCYTHE_GROUND_2 && 
	   state != STATE_SCYTHE_GROUND_3 &&
	   state != STATE_PHASE_STEP)
	{
		PlayerState new_state = STATE_NO_STATE;

		if(grounded_detected)        
		{new_state = STATE_GROUNDED_NEUTRAL;}
	
		else if(wall_right_detected) 
		{new_state = STATE_WALL_SLIDE_RIGHT;}
	
		else if(wall_left_detected)  
		{new_state = STATE_WALL_SLIDE_LEFT;}
	
		else 
		{new_state = STATE_AIR_NEUTRAL;}
	
		if(kill_player) {new_state = STATE_DYING;}

		// Set the state

		// Special case for air scythe, let landing end
		// the attack state.
		if(state == STATE_SCYTHE_AIR_1)
		{
			if(new_state == STATE_GROUNDED_NEUTRAL) 
			{setState(new_state);}
		}
		// Otherwise, if the new state is new and not current state,
		// set the new state.
		else if(new_state != state) {setState(new_state);}
	}

	///////////////////
	// Update Timers //
	///////////////////

	v_collision_grace_frames--;
	v_collision_grace_frames = clamp(0, 
									 PLAYER_V_COLLISION_MAX_GRACE_FRAMES,
									 v_collision_grace_frames);

	received_platform_force = false;

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

void Player::setCamera(const bn::camera_ptr& camera)
{
	GameObject::setCamera(camera);
	pm_sprite_ptr->set_camera(camera);
}

void Player::jump()
{
	remaining_jump_input_frames = PLAYER_MAX_JUMP_INPUT_FRAMES;
	late_jump_grace_frames      = 0;
	rigidbody.addForce(PLAYER_JUMP_FORCE);
	sprite_ptr->set_vertical_scale(PLAYER_MAX_STRETCH_V);
	sprite_ptr->set_horizontal_scale(PLAYER_MIN_STRETCH_H);
}

void Player::fastFall()
{
	rigidbody.addForce(PLAYER_FAST_GRAVITY_FORCE);
	sprite_ptr->set_vertical_scale(PLAYER_FALL_STRETCH_V);
	sprite_ptr->set_horizontal_scale(PLAYER_FALL_STRETCH_H);
}

void Player::setState(PlayerState new_state)
{
	// Set State
	state = new_state;

	// Set animations & other state specific variables
	switch(new_state)
	{
		
		case STATE_NO_STATE:
		break;

		case STATE_GROUNDED_NEUTRAL:

			remaining_x_drift_lockout_frames = 0;
			air_frames_elapsed               = 0;
			late_jump_grace_frames           = PLAYER_LATE_JUMP_GRACE_FRAMES;
			
			animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  		  1,
								  								  		  bn::sprite_items::player.tiles_item(),
								  								  		  0,
								  								  		  0);

		break;

		case STATE_WALL_SLIDE_RIGHT:

			rigidbody.removeForces();
			remaining_x_drift_lockout_frames = 0;
			remaining_jump_input_frames      = 0;
			air_frames_elapsed               = 0;
			late_jump_grace_frames           = 0;
			scythe_ground_2_buffered         = false;
			scythe_ground_3_buffered         = false;
			dir                              = LEFT;

			animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  1,
								  								  bn::sprite_items::player.tiles_item(),
								  								  4,
								  								  4);

		break;

		case STATE_WALL_SLIDE_LEFT:
			
			rigidbody.removeForces();
			remaining_x_drift_lockout_frames = 0;
			remaining_jump_input_frames      = 0;
			air_frames_elapsed               = 0;
			late_jump_grace_frames           = 0;
			scythe_ground_2_buffered         = false;
			scythe_ground_3_buffered         = false;
			dir                              = RIGHT;

			animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  1,
								  								  bn::sprite_items::player.tiles_item(),
								  								  4,
								  								  4);

		break;

		case STATE_AIR_NEUTRAL:

			scythe_ground_2_buffered = false;
			scythe_ground_3_buffered = false;

			animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  1,
								  								  bn::sprite_items::player.tiles_item(),
								  								  2,
								  								  2);

		break;

		case STATE_DYING:

			scythe_ground_2_buffered = false;
			scythe_ground_3_buffered = false;

		break;

		case STATE_PHASE_STEP:

			rigidbody.removeForces();

			current_phase_frame              = 0;
			air_frames_elapsed               = 0;
			remaining_jump_input_frames      = 0;
			remaining_x_drift_lockout_frames = 0;
			late_jump_grace_frames           = PLAYER_LATE_JUMP_GRACE_FRAMES;

			animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
																		  1,
																		  bn::sprite_items::player.tiles_item(),
																		  5,
																		  5);
			sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H); 				
			sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);

			pm_sprite_ptr->set_position(phase_destination.x(), phase_destination.y());
			pm_sprite_ptr->set_visible(true);

		break;

		case STATE_SCYTHE_GROUND_1:

			current_scythe_frame = 0;
			remaining_x_drift_lockout_frames = 0;
			air_frames_elapsed = 0;

			animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  1,
								  								  bn::sprite_items::player.tiles_item(),
								  								  1,
								  								  1);

		break;

		case STATE_SCYTHE_GROUND_2:

			current_scythe_frame = 0;
			remaining_x_drift_lockout_frames = 0;
			air_frames_elapsed = 0;

			animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  1,
								  								  bn::sprite_items::player.tiles_item(),
								  								  1,
								  								  1);

		break;

		case STATE_SCYTHE_GROUND_3:

			current_scythe_frame             = 0;
			remaining_x_drift_lockout_frames = 0;
			air_frames_elapsed = 0;

			animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
																		  1,
																		  bn::sprite_items::player.tiles_item(),
																		  1,
																		  1);

		break;

		case STATE_SCYTHE_AIR_1:

			current_scythe_frame = 0;
			animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
																		1,
																		bn::sprite_items::player.tiles_item(),
																		3,
																		3);

		break;

		default:
		break;
	}

}

void Player::createGroundedScythe1Hitboxes(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects, 
	                                       const bn::camera_ptr&                      camera)
{
	if(game_objects.size() >= MAX_GAME_OBJECTS) {return;}

	delete hitbox_1_ptr;
	hitbox_1_ptr = new Hitbox(bn::point(x().integer() + (PLAYER_SCYTHE_GROUND_1_X_OFFSET * dir),
								  y().integer() + PLAYER_SCYTHE_GROUND_1_Y_OFFSET),
								  PLAYER_SCYTHE_GROUND_1_HITSTUN_FRAMES,
								  PLAYER_SCYTHE_GROUND_1_HB_LIFESPAN_FRAMES,
								  PLAYER_SCYTHE_GROUND_1_X_KNOCKBACK,
								  PLAYER_SCYTHE_GROUND_1_Y_KNOCKBACK,	
								  PLAYER_SCYTHE_GROUND_1_KNOCKBACK_DECAY,
								  PLAYER_SCYTHE_GROUND_1_HB_WIDTH,
								  PLAYER_SCYTHE_GROUND_1_HB_HEIGHT,
								  dir,
								  HITBOX_SCYTHE_GROUND_1);

	hitbox_1_ptr->setCamera(camera);

	// Add more hitboxes...
}

void Player::createAirScythe1Hitboxes(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects, 
							  		  const bn::camera_ptr&                      camera)
{
	if(game_objects.size() >= MAX_GAME_OBJECTS) {return;}

	delete hitbox_1_ptr;
	hitbox_1_ptr = new Hitbox(bn::point(x().integer() + (PLAYER_SCYTHE_AIR_1_X_OFFSET * dir),
								  y().integer() + PLAYER_SCYTHE_AIR_1_Y_OFFSET),
								  PLAYER_SCYTHE_AIR_1_HITSTUN_FRAMES,
								  PLAYER_SCYTHE_AIR_1_HB_LIFESPAN_FRAMES,
								  PLAYER_SCYTHE_AIR_1_X_KNOCKBACK,
								  PLAYER_SCYTHE_AIR_1_Y_KNOCKBACK,	
								  PLAYER_SCYTHE_AIR_1_KNOCKBACK_DECAY,
								  PLAYER_SCYTHE_AIR_1_HB_WIDTH,
								  PLAYER_SCYTHE_AIR_1_HB_HEIGHT,
								  dir,
								  HITBOX_SCYTHE_AIR_1);

	hitbox_1_ptr->setCamera(camera);

	// Add more hitboxes...
}
