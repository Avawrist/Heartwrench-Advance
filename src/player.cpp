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
    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  2,
								  								  bn::sprite_items::player.tiles_item(),
								  								  0,
								  								  0);

	collider            = Collider(x(), y(), PLAYER_COLLIDER_WIDTH, PLAYER_COLLIDER_HEIGHT);
	collider_x_axis     = collider;
	collider_y_axis     = collider;
	test_collider       = collider;
	test_collider_right = collider;
	test_collider_left  = collider;
	collider_offset_x = 0;
	collider_offset_y = 0;
    
	state             	 = STATE_AIR_NEUTRAL;
    x_speed        	  	 = PLAYER_MIN_X_SPEED;
    jump_force           = PLAYER_BASE_JUMP_FORCE;
	secondary_jump_force = PLAYER_SECOND_JUMP_FORCE;
    wall_jump_force   	 = bn::fixed_point(PLAYER_WALL_JUMP_X_FORCE,
										   PLAYER_WALL_JUMP_Y_FORCE);

    gravity           	 = PLAYER_GRAVITY;
	wall_ride_gravity 	 = PLAYER_WALL_RIDE_GRAVITY;
	
	remaining_jump_input_frames      = 0;
	remaining_x_drift_lockout_frames = 0;
	air_frames_elapsed               = 0;
	v_collision_grace_frames         = 0;
	late_jump_grace_frames           = 0;
	scythe_charge_frames             = 0;
	current_death_frame              = 0;
	current_phase_step_frame         = 0;

	wall_right_detected   = false;
    wall_left_detected    = false;
    grounded_detected     = false;
	grounded_owp_detected = false;
	scythe_2_buffered     = false;
	scythe_3_buffered     = false;
	kill_player           = false;
	is_dead               = false;

	//hitbox_1_ptr = NULL;
	//hitbox_2_ptr = NULL;
	//hitbox_3_ptr = NULL;
	
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
	
	remaining_jump_input_frames      = other.remaining_jump_input_frames;
	remaining_x_drift_lockout_frames = other.remaining_x_drift_lockout_frames;
	air_frames_elapsed               = other.air_frames_elapsed;
	v_collision_grace_frames         = other.v_collision_grace_frames;
	late_jump_grace_frames           = other.late_jump_grace_frames;
	scythe_charge_frames             = other.scythe_charge_frames;
	current_death_frame              = other.current_death_frame;
	current_phase_step_frame         = other.current_phase_step_frame;

	wall_right_detected   = other.wall_right_detected;
    wall_left_detected    = other.wall_left_detected;
    grounded_detected     = other.grounded_detected;
	grounded_owp_detected = other.grounded_owp_detected;
	scythe_2_buffered     = other.scythe_2_buffered;
	scythe_3_buffered     = other.scythe_3_buffered;
	kill_player           = other.kill_player;
	is_dead               = other.is_dead;

	test_collider         = other.test_collider;
	test_collider_right   = other.test_collider_right;
	test_collider_left    = other.test_collider_left;

	//hitbox_1 = other.hitbox_1;
	//hitbox_2 = other.hitbox_2;
	//hitbox_3 = other.hitbox_3;
}

Player::~Player()
{
	
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
	
	remaining_jump_input_frames      = other.remaining_jump_input_frames;
	remaining_x_drift_lockout_frames = other.remaining_x_drift_lockout_frames;
	air_frames_elapsed               = other.air_frames_elapsed;
	v_collision_grace_frames         = other.v_collision_grace_frames;
	late_jump_grace_frames           = other.late_jump_grace_frames;
	scythe_charge_frames             = other.scythe_charge_frames;
	current_death_frame              = other.current_death_frame;
	current_phase_step_frame         = other.current_phase_step_frame;

	wall_right_detected   = other.wall_right_detected;
    wall_left_detected    = other.wall_left_detected;
    grounded_detected     = other.grounded_detected;
	grounded_owp_detected = other.grounded_owp_detected;
	scythe_2_buffered     = other.scythe_2_buffered;
	scythe_3_buffered     = other.scythe_3_buffered;
	kill_player           = other.kill_player;
	is_dead               = other.is_dead;

	test_collider         = other.test_collider;
	test_collider_right   = other.test_collider_right;
	test_collider_left    = other.test_collider_left;

	//hitbox_1 = other.hitbox_1;
	//hitbox_2 = other.hitbox_2;
	//hitbox_3 = other.hitbox_3;

	return *this;
}

void Player::update(const RoomBounds& 								  room_bounds,
					bn::vector<GameObject*, MAX_GAME_OBJECTS>&        game_objects,
					const bn::regular_bg_ptr&                         bg_ptr, 
                    const bn::span<const bn::regular_bg_map_cell>&    cells,
                    const bn::regular_bg_item&                        bg_item,
					const bn::camera_ptr&                             camera)
{
    //////////////////////////
    // Player State Machine //
    //////////////////////////

	bool gripping_wall_right = false;
	bool gripping_wall_left  = false;
	bool in_scythe_1         = false;
	bool in_scythe_2         = false;
	bool in_scythe_3         = false;
	//bool create_scythe_hb_1  = false;
	//bool create_scythe_hb_2  = false;
	//bool create_scythe_hb_3  = false;
	bool in_phase_step       = false;
	bool plummet_eligible    = false;

	///////////////////
	// Handle State ///
	///////////////////
	
    switch(state)
    {
		case STATE_GROUNDED_NEUTRAL:

			///////////////////////////////////
			// Player Grounded Neutral State //
			///////////////////////////////////

			// Set Jump Grace frames to full
			late_jump_grace_frames = PLAYER_LATE_JUMP_GRACE_FRAMES;

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

			// Jump
			if(bn::keypad::a_pressed()) {jump();}

			// Phase Step
			if(bn::keypad::r_pressed()) 
			{in_phase_step = true;}

			// Scythe 1
			if(bn::keypad::b_pressed())
			{in_scythe_1 = true;}
			
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

			// Fast Fall
			if(bn::keypad::down_held() && 
			   rigidbody.normalized_dir.y() >= 0 && 
			   air_frames_elapsed >= PLAYER_MIN_FAST_FALL_FRAMES)
			{fastFall();}
			
			// Late Jump
			if(bn::keypad::a_pressed() && late_jump_grace_frames) 
			{jump();}

			// High Jump
			if(bn::keypad::a_held() && remaining_jump_input_frames > 0)
			{rigidbody.addForce(PLAYER_SECONDARY_JUMP_FORCE);}

			else if(bn::keypad::a_released()) 
			{remaining_jump_input_frames = 0;}

			// Phase Step
			if(bn::keypad::r_pressed()) 
			{in_phase_step = true;}

			// Scythe 1
			if(bn::keypad::b_pressed())
			{in_scythe_1 = true;}
			
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

		break;
		
		case STATE_AIR_PLUMMET:
			
			//////////////////////////////
			// Player Air Plummet State //
			//////////////////////////////

			plummet_eligible = true;

			// Get Input //

			// Drift
			if(bn::keypad::left_held() && !remaining_x_drift_lockout_frames)       
			{rigidbody.addForce(PLAYER_X_PLUMMET_LEFT_FORCE); dir = LEFT;}

			else if(bn::keypad::right_held() && !remaining_x_drift_lockout_frames)
			{rigidbody.addForce(PLAYER_X_PLUMMET_RIGHT_FORCE); dir = RIGHT;}
			
			// Add Plummet Gravity //
			rigidbody.addForce(PLAYER_GRAVITY_FORCE);
			rigidbody.addForce(PLAYER_PLUMMET_GRAVITY_FORCE);

			// Update Squish frames for squish eligibility // 
			air_frames_elapsed++;
			air_frames_elapsed = clamp(0, 
									   PLAYER_MAX_AIR_FRAMES, 
									   air_frames_elapsed);

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

			// Get Input //
	
			// Drift
			if(bn::keypad::left_held() && !remaining_x_drift_lockout_frames) 
			{rigidbody.addForce(PLAYER_X_LEFT_FORCE);}
			else if(bn::keypad::right_held()) 								 
			{gripping_wall_right = true; dir = LEFT;}

			// Wall Jump
			if(bn::keypad::a_pressed())
			{
				rigidbody.addForce(PLAYER_WALL_JUMP_LEFT_FORCE);
				sprite_ptr->set_vertical_scale(PLAYER_MAX_STRETCH_V);
				sprite_ptr->set_horizontal_scale(PLAYER_MIN_STRETCH_H);
				remaining_x_drift_lockout_frames = PLAYER_X_DRIFT_LOCKOUT_FRAMES;
				dir = LEFT;
			}

			// Phase Step
			if(bn::keypad::r_pressed()) 
			{in_phase_step = true;}

			// Scythe 1
			if(bn::keypad::b_pressed())
			{in_scythe_1 = true;}

			// Fast Fall
			if(bn::keypad::down_held() && rigidbody.normalized_dir.y() >= 0) 
			{fastFall();}
			
			// Add Gravity //
			if(gripping_wall_right) {rigidbody.addForce(PLAYER_WALL_GRAVITY_FORCE);}
			else 
			{
				rigidbody.addForce(PLAYER_GRAVITY_FORCE);
				if(air_frames_elapsed >= PLAYER_PROLONGED_AIR_FRAMES_REQUIRED)
				{rigidbody.addForce(PLAYER_PROLONGED_GRAVITY_FORCE);}
			}

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

			// Get Input //

			// Drift
			if(bn::keypad::left_held()) {gripping_wall_left = true; dir = RIGHT;}
			else if(bn::keypad::right_held() && !remaining_x_drift_lockout_frames) 
			{rigidbody.addForce(PLAYER_X_RIGHT_FORCE);}

			// Wall Jump
			if(bn::keypad::a_pressed())
			{
				rigidbody.addForce(PLAYER_WALL_JUMP_RIGHT_FORCE);
				sprite_ptr->set_vertical_scale(PLAYER_MAX_STRETCH_V);
				sprite_ptr->set_horizontal_scale(PLAYER_MIN_STRETCH_H);
				remaining_x_drift_lockout_frames = PLAYER_X_DRIFT_LOCKOUT_FRAMES;
				dir = RIGHT;
			}

			// Phase Step
			if(bn::keypad::r_pressed()) 
			{in_phase_step = true;}

			// Scythe 1
			if(bn::keypad::b_pressed())
			{in_scythe_1 = true;}

			// Fast Fall
			if(bn::keypad::down_held() && rigidbody.normalized_dir.y() >= 0) 
			{fastFall();}
			
			// Add Gravity //
			if(gripping_wall_left) {rigidbody.addForce(PLAYER_WALL_GRAVITY_FORCE);}
			else 
			{
				rigidbody.addForce(PLAYER_GRAVITY_FORCE);
				if(air_frames_elapsed >= PLAYER_PROLONGED_AIR_FRAMES_REQUIRED)
				{rigidbody.addForce(PLAYER_PROLONGED_GRAVITY_FORCE);}
			}

			// Update Squish frames for squish eligibility // 
			air_frames_elapsed++;
			air_frames_elapsed = clamp(0, 
									   PLAYER_MAX_AIR_FRAMES, 
									   air_frames_elapsed);
		
		break;

		case STATE_PHASE_STEP:

			// Increment Frame Counter
			current_phase_step_frame++;
			current_phase_step_frame = clamp(0,
											 PLAYER_PHASE_STEP_TOTAL_FRAMES, 
											 current_phase_step_frame);

			// Phase Player
			if(current_phase_step_frame == PLAYER_PHASE_FRAME)
			{
				// Copy the player's collider at the full Phase Step
				// distance, and increment it towards the player
				// until there are no tile collisions. 

				// Get current cell index that player resides in:
				int32 half_level_width_pixels  = bg_ptr.dimensions().width()  / 2;
				int32 half_level_height_pixels = bg_ptr.dimensions().height() / 2;
				bn::fixed index_x = (x() + half_level_width_pixels)  / TILE_WIDTH;
				bn::fixed index_y = (y() + half_level_height_pixels) / TILE_HEIGHT;
				bn::point cell_index = bn::point(index_x.integer(), index_y.integer());

				int32 x_offset = PLAYER_PHASE_STEP_MAX_DISTANCE * dir;
				int32 max_x    = (x_offset / TILE_WIDTH) + (dir * 2);

				Collider other_collider;
				Collider phase_collider = Collider(collider.x() + x_offset,
												   collider.y(),
												   collider.width,
												   collider.height);

				// Clamp Phase collider BEFORE checking for tile collision
				bn::fixed new_x = phase_collider.x();
				bn::fixed new_y = phase_collider.y();
				new_x = clamp(room_bounds.left_bound, room_bounds.right_bound,  new_x);
				new_y = clamp(room_bounds.top_bound,  room_bounds.bottom_bound, new_y);
				phase_collider.setPos(new_x, new_y);

				for(int32 y = -2; y < 3; y++)
				{
					for(int32 x = max_x; x != 0; x += (dir * -1))
					{
						// 1. Get tile type at index
						int32 check_index_x = cell_index.x() + x;
						int32 check_index_y = cell_index.y() + y;

						// Determine world coords in case we need to make a collider.
						int32 world_x = ((check_index_x * TILE_WIDTH)  - half_level_width_pixels)  + (TILE_WIDTH  / 2);
						int32 world_y = ((check_index_y * TILE_HEIGHT) - half_level_height_pixels) + (TILE_HEIGHT / 2);

						uint32 tile_index = getTileAtBGIndex(check_index_x, check_index_y,
															 bg_ptr, cells, bg_item);

						// 2. If the tile is collidable, check for collision
						switch(tile_index)
						{
							case HARD_BLOCK_INDEX:
							case SOFT_BLOCK_INDEX:
							case UP_SPIKE_BLOCK_INDEX:
							case DOWN_SPIKE_BLOCK_INDEX:
							case LEFT_SPIKE_BLOCK_INDEX:
							case RIGHT_SPIKE_BLOCK_INDEX:

								other_collider = Collider(world_x, 
														  world_y, 
														  TILE_WIDTH, 
														  TILE_HEIGHT);

								while(phase_collider.isCollision(other_collider))
								{phase_collider.setX(phase_collider.x() + (dir * -1));}

							break;

							default:
							break;
						}
					}
				}

				// Update player position
				setPos(phase_collider.x(), phase_collider.y());

			} 

			// Keep the player in phase step state until frames are up
			if(current_phase_step_frame < PLAYER_PHASE_STEP_TOTAL_FRAMES)
			{in_phase_step = true;}
			else
			{current_phase_step_frame = 0;
			 plummet_eligible         = true;}

		break;

		case STATE_SCYTHE_1:

			// Increment Frame Counter
			current_scythe_frame++;
			current_scythe_frame = clamp(0, 
										 PLAYER_SCYTHE_1_TOTAL_FRAMES,
										 current_scythe_frame);

			// Create Hitbox
			if(current_scythe_frame == PLAYER_SCYTHE_1_CREATE_HB_FRAME &&
			   game_objects.size() < MAX_GAME_OBJECTS)
			{
				//create_scythe_hb_1 = true;
			}

			// Take Cancel Input
			if(bn::keypad::b_pressed() && 
			   current_scythe_frame >= PLAYER_MIN_SCYTHE_1_CANCEL_FRAMES)
			{in_scythe_2          = true;
			 current_scythe_frame = 0;} 

			if(bn::keypad::r_pressed() && 
			   current_scythe_frame >= PLAYER_MIN_SCYTHE_1_CANCEL_FRAMES)
			{in_phase_step        = true;
			 current_scythe_frame = 0;}

			// End state if frames are not up
			else if(current_scythe_frame < PLAYER_SCYTHE_1_TOTAL_FRAMES)
			{in_scythe_1 = true;}

			else
			{
				current_scythe_frame = 0;
				plummet_eligible     = true;
				if(scythe_2_buffered) {in_scythe_2 = true;}
			}

		break;

		case STATE_SCYTHE_2:

			// Increment Frame Counter
			current_scythe_frame++;
			current_scythe_frame = clamp(0, 
										 PLAYER_SCYTHE_2_TOTAL_FRAMES, 
										 current_scythe_frame);

			// Create Hitbox
			if(current_scythe_frame == PLAYER_SCYTHE_2_CREATE_HB_FRAME &&
			   game_objects.size() < MAX_GAME_OBJECTS)
			{
				//create_scythe_hb_2 = true;
			}

			// Take Cancel Input
			if(bn::keypad::b_pressed() && 
			   current_scythe_frame >= PLAYER_MIN_SCYTHE_2_CANCEL_FRAMES)
			{in_scythe_3 = true;
			 current_scythe_frame = 0;}

			if(bn::keypad::r_pressed() && 
			   current_scythe_frame >= PLAYER_MIN_SCYTHE_2_CANCEL_FRAMES)
			{in_phase_step        = true;
			 current_scythe_frame = 0;}

			// End state if frames are not up
			else if(current_scythe_frame < PLAYER_SCYTHE_2_TOTAL_FRAMES)
			{in_scythe_2 = true;}

			else
			{
				current_scythe_frame = 0;
				plummet_eligible     = true;
				if(scythe_3_buffered) {in_scythe_3 = true;}
			}

		break;

		case STATE_SCYTHE_3:

			// Increment Frame Counter
			current_scythe_frame++;
			current_scythe_frame = clamp(0, 
										 PLAYER_SCYTHE_3_TOTAL_FRAMES, 
										 current_scythe_frame);

			// Create Hitbox
			if(current_scythe_frame == PLAYER_SCYTHE_3_CREATE_HB_FRAME &&
			   game_objects.size() < MAX_GAME_OBJECTS)
			{
				//create_scythe_hb_3 = true;
			}

			// Take Cancel Input
			if(bn::keypad::r_pressed() && 
			   current_scythe_frame >= PLAYER_MIN_SCYTHE_3_CANCEL_FRAMES)
			{in_phase_step        = true;
			 current_scythe_frame = 0;}

			// End state if frames are not up
			else if(current_scythe_frame < PLAYER_SCYTHE_3_TOTAL_FRAMES)
			{in_scythe_3 = true;}

			else 
			{
				current_scythe_frame = 0;
				plummet_eligible     = true;
			}

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

	for(int32 i = 0; i < game_objects.size(); i++)
    {
		other_collider = game_objects.at(i)->collider;

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

			default:
			break;
		}
	}

	////////////////////////////
    // Resolve Tile Collision //
    ////////////////////////////
	
	for(int32 y = -2; y < 3; y++)
	{
		for(int32 x = -2; x < 3; x++)
		{
			// 0. Early outs for optimization
			if(rigidbody.normalized_dir.x().integer() == 0 &&
		       rigidbody.normalized_dir.y().integer() == 0) {break;}
			
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

			bn::fixed col_x_offset;
			bn::fixed col_y_offset;

			int32 index;
			int32 local_height;
			int32 global_height;

			// 2. If the tile is collidable make a temporary collider //
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
						setY(global_height - (PLAYER_COLLIDER_HEIGHT / 2));
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
						setY(global_height - (PLAYER_COLLIDER_HEIGHT / 2));
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
						setY(global_height - (PLAYER_COLLIDER_HEIGHT / 2));
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
						setY(global_height - (PLAYER_COLLIDER_HEIGHT / 2));
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
						setY(global_height - (PLAYER_COLLIDER_HEIGHT / 2));
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
						setY(global_height - (PLAYER_COLLIDER_HEIGHT / 2));
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
						setY(global_height - (PLAYER_COLLIDER_HEIGHT / 2));
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
						setY(global_height - (PLAYER_COLLIDER_HEIGHT / 2));
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
						setY(global_height - (PLAYER_COLLIDER_HEIGHT / 2));
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
						setY(global_height - (PLAYER_COLLIDER_HEIGHT / 2));
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
						setY(global_height - (PLAYER_COLLIDER_HEIGHT / 2));
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
						setY(global_height - (PLAYER_COLLIDER_HEIGHT / 2));
					}
				
				break;

				case ONEWAY_BLOCK_INDEX:

					other_collider = Collider(world_x, 
											  world_y + ONEWAYBLOCK_COLLIDER_Y_OFFSET, 
											  TILE_WIDTH, 
											  ONEWAYBLOCK_COLLIDER_HEIGHT);

					if(rigidbody.normalized_dir.y() >= 0 &&
					   collider_y_axis.p4.y() <= other_collider.p1.y() + PLAYER_GRAVITY)
					{

						if(bn::keypad::down_held()) {rigidbody.addForce(PLAYER_GRAVITY_FORCE); break;}
					
						// Handle Remaining Collision Cases //
						while(collider_y_axis.isCollision(other_collider))
						{
							collider_y_axis.setY(collider_y_axis.y() - 1);
							setY(this->y() - 1);
						}
					}

				break;

				default:
				break;
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

	/////////////////////////////////////
	// Get State Info from GameObjects //
	/////////////////////////////////////

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

			default:
			break;
		}
	}

	///////////////////////////////
    // Get State Info from Tiles //
    ///////////////////////////////							   

	int32 index;
	int32 local_height;
	int32 global_height;
	
	for(int32 y = -2; y < 3; y++)
	{
		for(int32 x = -2; x < 3; x++)
		{
			// 0. Early outs for optimization
			if(rigidbody.normalized_dir.x().integer() == 0 &&
		       rigidbody.normalized_dir.y().integer() == 0) {break;}
			
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

			switch(tile_index)
			{
				case HARD_BLOCK_INDEX:
				case SOFT_BLOCK_INDEX:

					other_collider = Collider(world_x,
											  world_y, 
											  TILE_WIDTH, 
											  TILE_HEIGHT);

					// Test for, and log grounded collision
					if(test_collider.isCollision(other_collider) && 
					   rigidbody.normalized_dir.y() >= 0)
					{
						if(air_frames_elapsed >= PLAYER_SQUISH_FRAMES_REQUIRED)
						{
							sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H); 				
							sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);

							if(rigidbody.final_dir.y() >= PLAYER_ROLL_SPEED_THRESHOLD)
							{
								if(bn::keypad::right_held())
								{
									dir = RIGHT;
									rigidbody.addForce(PLAYER_ROLL_FORCE);
								}
								else if(bn::keypad::left_held())
								{
									dir = LEFT;
									rigidbody.addForce(PLAYER_ROLL_FORCE);
								}
							}
						}
						grounded_detected = true;
					}

					// Test for wall riding on right side
					if(test_collider_right.isCollision(other_collider) && 
					   rigidbody.final_dir.y() >= 0)
					{
						wall_right_detected = true;
					}
					
					// Test for wall riding on left side
					if(test_collider_left.isCollision(other_collider) && 
					   rigidbody.final_dir.y() >= 0)
					{
						wall_left_detected = true;
					}

				break;

				case ONEWAY_BLOCK_INDEX:

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
								if(air_frames_elapsed >= PLAYER_SQUISH_FRAMES_REQUIRED)
								{sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);
						     	 sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);}
							}
						}
					}

				break;

				case UP_SPIKE_BLOCK_INDEX:

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

				break;

				case DOWN_SPIKE_BLOCK_INDEX:

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

				break;

				case LEFT_SPIKE_BLOCK_INDEX:

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

				break;

				case RIGHT_SPIKE_BLOCK_INDEX:

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

				break;

				case LEFT_SHALLOW_SLOPE_1_INDEX:
					
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
						   rigidbody.normalized_dir.y() >= 0)
						{
							sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);				
							sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);

							if(rigidbody.final_dir.y() >= PLAYER_ROLL_SPEED_THRESHOLD)
							{
								if(bn::keypad::right_held())
								{
									dir = RIGHT;
									rigidbody.addForce(PLAYER_ROLL_FORCE);
								}
								else if(bn::keypad::left_held())
								{
									dir = LEFT;
									rigidbody.addForce(PLAYER_ROLL_FORCE);
								}
							}
						}

						grounded_detected = true;
					}
					
				break;

				case LEFT_SHALLOW_SLOPE_2_INDEX:
					
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
						   rigidbody.normalized_dir.y() >= 0)
						{
							sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);				
							sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);

							if(rigidbody.final_dir.y() >= PLAYER_ROLL_SPEED_THRESHOLD)
							{
								if(bn::keypad::right_held())
								{
									dir = RIGHT;
									rigidbody.addForce(PLAYER_ROLL_FORCE);
								}
								else if(bn::keypad::left_held())
								{
									dir = LEFT;
									rigidbody.addForce(PLAYER_ROLL_FORCE);
								}
							}
						}

						grounded_detected = true;
					}
					
				break;

				case LEFT_SHALLOW_SLOPE_3_INDEX:
					
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
						   rigidbody.normalized_dir.y() >= 0)
						{
							sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);				
							sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);

							if(rigidbody.final_dir.y() >= PLAYER_ROLL_SPEED_THRESHOLD)
							{
								if(bn::keypad::right_held())
								{
									dir = RIGHT;
									rigidbody.addForce(PLAYER_ROLL_FORCE);
								}
								else if(bn::keypad::left_held())
								{
									dir = LEFT;
									rigidbody.addForce(PLAYER_ROLL_FORCE);
								}
							}
						}

						grounded_detected = true;
					}
					
				break;

				case LEFT_SHALLOW_SLOPE_4_INDEX:
					
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
						   rigidbody.normalized_dir.y() >= 0)
						{
							sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);				
							sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);

							if(rigidbody.final_dir.y() >= PLAYER_ROLL_SPEED_THRESHOLD)
							{
								if(bn::keypad::right_held())
								{
									dir = RIGHT;
									rigidbody.addForce(PLAYER_ROLL_FORCE);
								}
								else if(bn::keypad::left_held())
								{
									dir = LEFT;
									rigidbody.addForce(PLAYER_ROLL_FORCE);
								}
							}
						}

						grounded_detected = true;
					}
					
				break;

				case LEFT_STEEP_SLOPE_1_INDEX:
					
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
						   rigidbody.normalized_dir.y() >= 0)
						{
							sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);				
							sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);

							if(rigidbody.final_dir.y() >= PLAYER_ROLL_SPEED_THRESHOLD)
							{
								if(bn::keypad::right_held())
								{
									dir = RIGHT;
									rigidbody.addForce(PLAYER_ROLL_FORCE);
								}
								else if(bn::keypad::left_held())
								{
									dir = LEFT;
									rigidbody.addForce(PLAYER_ROLL_FORCE);
								}
							}
						}

						grounded_detected = true;
					}
					
				break;

				case LEFT_STEEP_SLOPE_2_INDEX:
					
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
						   rigidbody.normalized_dir.y() >= 0)
						{
							sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);				
							sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);

							if(rigidbody.final_dir.y() >= PLAYER_ROLL_SPEED_THRESHOLD)
							{
								if(bn::keypad::right_held())
								{
									dir = RIGHT;
									rigidbody.addForce(PLAYER_ROLL_FORCE);
								}
								else if(bn::keypad::left_held())
								{
									dir = LEFT;
									rigidbody.addForce(PLAYER_ROLL_FORCE);
								}
							}
						}

						grounded_detected = true;
					}
					
				break;

				case RIGHT_SHALLOW_SLOPE_1_INDEX:

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
						   rigidbody.normalized_dir.y() >= 0)
						{
							sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);				
							sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);

							if(rigidbody.final_dir.y() >= PLAYER_ROLL_SPEED_THRESHOLD)
							{
								if(bn::keypad::right_held())
								{
									dir = RIGHT;
									rigidbody.addForce(PLAYER_ROLL_FORCE);
								}
								else if(bn::keypad::left_held())
								{
									dir = LEFT;
									rigidbody.addForce(PLAYER_ROLL_FORCE);
								}
							}
						}

						grounded_detected = true;
					}
					
				break;

				case RIGHT_SHALLOW_SLOPE_2_INDEX:

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
						   rigidbody.normalized_dir.y() >= 0)
						{
							sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);				
							sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);

							if(rigidbody.final_dir.y() >= PLAYER_ROLL_SPEED_THRESHOLD)
							{
								if(bn::keypad::right_held())
								{
									dir = RIGHT;
									rigidbody.addForce(PLAYER_ROLL_FORCE);
								}
								else if(bn::keypad::left_held())
								{
									dir = LEFT;
									rigidbody.addForce(PLAYER_ROLL_FORCE);
								}
							}
						}

						grounded_detected = true;
					}
					
				break;

				case RIGHT_SHALLOW_SLOPE_3_INDEX:

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
						   rigidbody.normalized_dir.y() >= 0)
						{
							sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);				
							sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);

							if(rigidbody.final_dir.y() >= PLAYER_ROLL_SPEED_THRESHOLD)
							{
								if(bn::keypad::right_held())
								{
									dir = RIGHT;
									rigidbody.addForce(PLAYER_ROLL_FORCE);
								}
								else if(bn::keypad::left_held())
								{
									dir = LEFT;
									rigidbody.addForce(PLAYER_ROLL_FORCE);
								}
							}
						}

						grounded_detected = true;
					}
					
				break;

				case RIGHT_SHALLOW_SLOPE_4_INDEX:

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
						   rigidbody.normalized_dir.y() >= 0)
						{
							sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);				
							sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);

							if(rigidbody.final_dir.y() >= PLAYER_ROLL_SPEED_THRESHOLD)
							{
								if(bn::keypad::right_held())
								{
									dir = RIGHT;
									rigidbody.addForce(PLAYER_ROLL_FORCE);
								}
								else if(bn::keypad::left_held())
								{
									dir = LEFT;
									rigidbody.addForce(PLAYER_ROLL_FORCE);
								}
							}
						}

						grounded_detected = true;
					}
					
				break;

				case RIGHT_STEEP_SLOPE_1_INDEX:

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
						   rigidbody.normalized_dir.y() >= 0)
						{
							sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);				
							sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);

							if(rigidbody.final_dir.y() >= PLAYER_ROLL_SPEED_THRESHOLD)
							{
								if(bn::keypad::right_held())
								{
									dir = RIGHT;
									rigidbody.addForce(PLAYER_ROLL_FORCE);
								}
								else if(bn::keypad::left_held())
								{
									dir = LEFT;
									rigidbody.addForce(PLAYER_ROLL_FORCE);
								}
							}
						}

						grounded_detected = true;
					}
					
				break;

				case RIGHT_STEEP_SLOPE_2_INDEX:

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
						   rigidbody.normalized_dir.y() >= 0)
						{
							sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);				
							sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);

							if(rigidbody.final_dir.y() >= PLAYER_ROLL_SPEED_THRESHOLD)
							{
								if(bn::keypad::right_held())
								{
									dir = RIGHT;
									rigidbody.addForce(PLAYER_ROLL_FORCE);
								}
								else if(bn::keypad::left_held())
								{
									dir = LEFT;
									rigidbody.addForce(PLAYER_ROLL_FORCE);
								}
							}
						}

						grounded_detected = true;
					}
					
				break;

				default:
				break;
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
	// Manage Hitboxes //
	/////////////////////

	// Create hitboxes
	/*
	if(create_scythe_hb_1)
	{
		hitbox_1 = Hitbox(bn::point(x().integer() + (PLAYER_SCYTHE_1_X_OFFSET * dir),
									y().integer() + PLAYER_SCYTHE_1_Y_OFFSET),
										PLAYER_SCYTHE_1_HITSTUN_FRAMES,
										PLAYER_SCYTHE_1_HB_LIFESPAN_FRAMES,
										PLAYER_SCYTHE_1_X_KNOCKBACK,
										PLAYER_SCYTHE_1_Y_KNOCKBACK,	
										PLAYER_SCYTHE_1_KNOCKBACK_DECAY,
										PLAYER_SCYTHE_1_HB_WIDTH,
										PLAYER_SCYTHE_1_HB_HEIGHT,
										dir,
										HITBOX_SCYTHE_1);

		hitbox_1.setCamera(camera);
	}

	if(create_scythe_hb_2)
	{
		hitbox_2 = Hitbox(bn::point(x().integer() + (PLAYER_SCYTHE_2_X_OFFSET * dir),
									y().integer() + PLAYER_SCYTHE_2_Y_OFFSET),
						  PLAYER_SCYTHE_2_HITSTUN_FRAMES,
						  PLAYER_SCYTHE_2_HB_LIFESPAN_FRAMES,
						  PLAYER_SCYTHE_2_X_KNOCKBACK,
						  PLAYER_SCYTHE_2_Y_KNOCKBACK,	
						  PLAYER_SCYTHE_2_KNOCKBACK_DECAY,
						  PLAYER_SCYTHE_2_HB_WIDTH,
						  PLAYER_SCYTHE_2_HB_HEIGHT,
						  dir,
						  HITBOX_SCYTHE_2);
		hitbox_2.setCamera(camera);
	}

	if(create_scythe_hb_3)
	{
		hitbox_3 = Hitbox(bn::point(x().integer() + (PLAYER_SCYTHE_3_X_OFFSET * dir), 
									y().integer() + PLAYER_SCYTHE_3_Y_OFFSET),
						  PLAYER_SCYTHE_3_HITSTUN_FRAMES,
						  PLAYER_SCYTHE_3_HB_LIFESPAN_FRAMES,
						  PLAYER_SCYTHE_3_X_KNOCKBACK,
						  PLAYER_SCYTHE_3_Y_KNOCKBACK,	
						  PLAYER_SCYTHE_3_KNOCKBACK_DECAY,
						  PLAYER_SCYTHE_3_HB_WIDTH,
						  PLAYER_SCYTHE_3_HB_HEIGHT,
						  dir,
						  HITBOX_SCYTHE_3);

		hitbox_3_ptr->setCamera(camera);
	}

	// Update hitboxes
	hitbox_1_ptr->setPos(bn::point(x().integer() + (PLAYER_SCYTHE_1_X_OFFSET * dir),
								   y().integer() + PLAYER_SCYTHE_1_Y_OFFSET));

	hitbox_1_ptr->update(room_bounds,
							 game_objects,
							 bg_ptr, 
                    		 cells,
                    		 bg_item,
							 camera);
	hitbox_1_ptr->draw();

		if(hitbox_1_ptr->is_inactive) 
		{
			delete hitbox_1_ptr;
			hitbox_1_ptr = NULL;
		}													 
	}
	
	if(hitbox_2_ptr != NULL) 
	{
		hitbox_2_ptr->setPos(bn::point(x().integer() + (PLAYER_SCYTHE_2_X_OFFSET * dir),
									   y().integer() + PLAYER_SCYTHE_2_Y_OFFSET));

		hitbox_2_ptr->update(room_bounds,
							 game_objects,
							 bg_ptr, 
                    		 cells,
                    		 bg_item,
							 camera);
		hitbox_2_ptr->draw();

		if(hitbox_2_ptr->is_inactive) 
		{
			delete hitbox_2_ptr;
			hitbox_2_ptr = NULL;
		}
	}
	
	if(hitbox_3_ptr != NULL) 
	{
		hitbox_3_ptr->setPos(bn::point(x().integer() + (PLAYER_SCYTHE_3_X_OFFSET * dir),
									   y().integer() + PLAYER_SCYTHE_3_Y_OFFSET));

		hitbox_3_ptr->update(room_bounds,
							 game_objects,
							 bg_ptr, 
                    		 cells,
                    		 bg_item,
							 camera);
		hitbox_3_ptr->draw();

		if(hitbox_3_ptr->is_inactive) 
		{
			delete hitbox_3_ptr;
			hitbox_3_ptr = NULL;
		}
	}
	*/
	
    ///////////////////
    // Update States //
    ///////////////////

	PlayerState new_state = STATE_GROUNDED_NEUTRAL;

    if(grounded_detected)        
	{new_state = STATE_GROUNDED_NEUTRAL;}

    else if(wall_right_detected) 
	{new_state = STATE_WALL_SLIDE_RIGHT;}

    else if(wall_left_detected)  
	{new_state = STATE_WALL_SLIDE_LEFT;}

    else 
	{
		if(plummet_eligible) {new_state = STATE_AIR_PLUMMET;}
		else 				 {new_state = STATE_AIR_NEUTRAL;}
	}

	if (in_phase_step) {new_state = STATE_PHASE_STEP;}
	if (in_scythe_1)   {new_state = STATE_SCYTHE_1;}
	if (in_scythe_2)   {new_state = STATE_SCYTHE_2;}
	if (in_scythe_3)   {new_state = STATE_SCYTHE_3;}

	if(kill_player) {new_state = STATE_DYING;}

	// Set the state
	setState(new_state);

	///////////////////
	// Update Timers //
	///////////////////

	v_collision_grace_frames--;
	v_collision_grace_frames = clamp(0, 
									 PLAYER_V_COLLISION_MAX_GRACE_FRAMES,
									 v_collision_grace_frames);

	late_jump_grace_frames--;
	late_jump_grace_frames = clamp(0, 
								   PLAYER_LATE_JUMP_GRACE_FRAMES, 
								   late_jump_grace_frames);

	received_platform_force = false;

	/////////////////////////////
	// Update Sprite Direction //
	/////////////////////////////
	
	if      (dir == LEFT)  {sprite_ptr->set_horizontal_flip(true);}
	else if (dir == RIGHT) {sprite_ptr->set_horizontal_flip(false);}
    
    ////////////////////////////
    // Correct Sprite Offsets //
    ////////////////////////////
	
    bn::fixed h_scale   = sprite_ptr->horizontal_scale();
    bn::fixed v_scale   = sprite_ptr->vertical_scale();
    bn::fixed increment = 0.1;

    // Correct H Scale
    if(h_scale > 1) {sprite_ptr->set_horizontal_scale(h_scale - increment);}
    else if (h_scale < 1) {sprite_ptr->set_horizontal_scale(h_scale + increment);}
    if(abs(1 - sprite_ptr->horizontal_scale()) < increment) {sprite_ptr->set_horizontal_scale(1);}
    
    // Correct V Scale
    if(v_scale > 1) {sprite_ptr->set_vertical_scale(v_scale - increment);}
    else if (v_scale < 1) {sprite_ptr->set_vertical_scale(v_scale + increment);}
    if(abs(1 - sprite_ptr->vertical_scale()) < increment) {sprite_ptr->set_vertical_scale(1);}
	
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
		
		case STATE_GROUNDED_NEUTRAL:
			remaining_x_drift_lockout_frames = 0;
			air_frames_elapsed = 0;
			scythe_2_buffered  = false;
			scythe_3_buffered  = false;
		break;

		case STATE_WALL_SLIDE_RIGHT:
			remaining_x_drift_lockout_frames = 0;
			air_frames_elapsed = 0;
			scythe_2_buffered  = false;
			scythe_3_buffered  = false;
		break;

		case STATE_WALL_SLIDE_LEFT:
			remaining_x_drift_lockout_frames = 0;
			air_frames_elapsed = 0;
			scythe_2_buffered  = false;
			scythe_3_buffered  = false;
		break;

		case STATE_AIR_NEUTRAL:
			scythe_2_buffered = false;
			scythe_3_buffered = false;
		break;

		case STATE_AIR_PLUMMET:
			remaining_x_drift_lockout_frames = 0;
			scythe_2_buffered = false;
			scythe_3_buffered = false;
		break;

		case STATE_DYING:
			scythe_2_buffered = false;
			scythe_3_buffered = false;
		break;

		case STATE_PHASE_STEP:
			air_frames_elapsed = 0;
			remaining_x_drift_lockout_frames = 0;
			scythe_2_buffered  = false;
			scythe_3_buffered  = false;
		break;

		case STATE_SCYTHE_1:
			remaining_x_drift_lockout_frames = 0;
			air_frames_elapsed = 0;
		break;

		case STATE_SCYTHE_2:
			remaining_x_drift_lockout_frames = 0;
			air_frames_elapsed = 0;
		break;

		case STATE_SCYTHE_3:
			remaining_x_drift_lockout_frames = 0;
			air_frames_elapsed = 0;
		break;

		default:
		break;
	}

}
