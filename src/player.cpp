#include "player.h"

///////////////////
// Struct Player //
///////////////////

Player::Player()
{
    // Reset Variables //
    sprite_ptr.reset();
    animate_action_ptr.reset();
    delete rigidbody_ptr;
    delete collider_ptr;

    // Init Variables //
	object_type = PLAYER;
	dir         = RIGHT;
    sprite_ptr  = bn::sprite_items::player.create_sprite(0, 0);
    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  2,
								  								  bn::sprite_items::player.tiles_item(),
								  								  0,
								  								  0);

    rigidbody_ptr = new RigidBody();
	collider_ptr  = new Collider(x(), y(), PLAYER_COLLIDER_WIDTH, PLAYER_COLLIDER_HEIGHT);

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
	current_scythe_throw_frames      = 0;
	owp_grace_frames                 = 0;
	air_frames_elapsed               = 0;

}

Player::~Player()
{
	
}

void Player::update(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects,
					const Room& room,
					const bn::camera_ptr& camera)
{

    //////////////////////////
    // Player State Machine //
    //////////////////////////

	bool gripping_wall_right = false;
	bool gripping_wall_left  = false;
	bool throw_scythe        = false;
	bool kill_player         = false;

	#define SCYTHE_X_OFFSET 20 
	#define SCYTHE_Y_OFFSET 0

    switch(state)
    {
		case STATE_GROUNDED_NEUTRAL:

			///////////////////////////////////
			// Player Grounded Neutral State //
			///////////////////////////////////

			// Update walk speed //
			if(bn::keypad::left_released())        
			{
				rigidbody_ptr->addForce(PLAYER_X_LEFT_DECAY_FORCE);
				x_speed = PLAYER_MIN_X_SPEED;
			}
			else if (bn::keypad::right_released()) 
			{
				rigidbody_ptr->addForce(PLAYER_X_RIGHT_DECAY_FORCE);
				x_speed = PLAYER_MIN_X_SPEED;
			}

			// Simulate friction/momentum
			if(bn::keypad::left_held() || bn::keypad::right_held())  
			{
				x_speed += X_SPEED_ACC_RATE;
				x_speed = clamp(PLAYER_MIN_X_SPEED, PLAYER_MAX_X_SPEED, x_speed);
			}

			// Get Input //
			
			// Walk
			if(bn::keypad::left_held())       
			{rigidbody_ptr->addForce(PLAYER_X_LEFT_FORCE); dir = LEFT;}

			else if(bn::keypad::right_held()) 
			{rigidbody_ptr->addForce(PLAYER_X_RIGHT_FORCE); dir = RIGHT;}

			// Jump
			if(bn::keypad::a_pressed())
			{
				remaining_jump_input_frames = PLAYER_MAX_JUMP_INPUT_FRAMES;
				rigidbody_ptr->addForce(PLAYER_JUMP_FORCE);
				sprite_ptr->set_vertical_scale(PLAYER_MAX_STRETCH_V);
				sprite_ptr->set_horizontal_scale(PLAYER_MIN_STRETCH_H);
			}

			// Scythe Throw
			if(bn::keypad::b_pressed()) {current_scythe_throw_frames = 0; throw_scythe = true;}

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
				rigidbody_ptr->addForce(PLAYER_X_LEFT_DECAY_FORCE);
				x_speed = PLAYER_MIN_X_SPEED;
			}
			else if (bn::keypad::right_released()) 
			{
				rigidbody_ptr->addForce(PLAYER_X_RIGHT_DECAY_FORCE);
				x_speed = PLAYER_MIN_X_SPEED;
			}

			// Get Input //
			if(bn::keypad::left_held() && !remaining_x_drift_lockout_frames)       
			{rigidbody_ptr->addForce(PLAYER_X_LEFT_FORCE); dir = LEFT;}

			else if(bn::keypad::right_held() && !remaining_x_drift_lockout_frames) 
			{rigidbody_ptr->addForce(PLAYER_X_RIGHT_FORCE); dir = RIGHT;}
			
			if(bn::keypad::a_held() && remaining_jump_input_frames > 0)
			{rigidbody_ptr->addForce(PLAYER_SECONDARY_JUMP_FORCE);}

			else if(bn::keypad::a_released()) {remaining_jump_input_frames = 0;}

			// Scythe Throw
			if(bn::keypad::b_pressed()) {current_scythe_throw_frames = 0; throw_scythe = true;}
			
			// Add Gravity //
			rigidbody_ptr->addForce(PLAYER_GRAVITY_FORCE);

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
									   PLAYER_SQUISH_FRAMES_REQUIRED, 
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
			if(bn::keypad::a_pressed())
			{
				rigidbody_ptr->addForce(PLAYER_WALL_JUMP_LEFT_FORCE);
				sprite_ptr->set_vertical_scale(PLAYER_MAX_STRETCH_V);
				sprite_ptr->set_horizontal_scale(PLAYER_MIN_STRETCH_H);
				remaining_x_drift_lockout_frames = PLAYER_X_DRIFT_LOCKOUT_FRAMES;
				dir = LEFT;
			}
			
			if(bn::keypad::left_held() && !remaining_x_drift_lockout_frames) 
			{rigidbody_ptr->addForce(PLAYER_X_LEFT_FORCE);}
			else if(bn::keypad::right_held()) 								 
			{gripping_wall_right = true; dir = LEFT;}

			// Scythe Throw
			if(bn::keypad::b_pressed()) {current_scythe_throw_frames = 0; throw_scythe = true;}
			
			// Add Gravity //
			if(gripping_wall_right) {rigidbody_ptr->addForce(PLAYER_WALL_GRAVITY_FORCE);}
			else {rigidbody_ptr->addForce(PLAYER_GRAVITY_FORCE);}
			
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
			if(bn::keypad::a_pressed())
			{
				rigidbody_ptr->addForce(PLAYER_WALL_JUMP_RIGHT_FORCE);
				sprite_ptr->set_vertical_scale(PLAYER_MAX_STRETCH_V);
				sprite_ptr->set_horizontal_scale(PLAYER_MIN_STRETCH_H);
				remaining_x_drift_lockout_frames = PLAYER_X_DRIFT_LOCKOUT_FRAMES;
				dir = RIGHT;
			}
			
			if(bn::keypad::left_held()) {gripping_wall_left = true; dir = RIGHT;}
			else if(bn::keypad::right_held() && !remaining_x_drift_lockout_frames) 
			{rigidbody_ptr->addForce(PLAYER_X_RIGHT_FORCE);}

			// Scythe Throw
			if(bn::keypad::b_pressed()) {current_scythe_throw_frames = 0; throw_scythe = true;}
			
			// Add Gravity //
			if(gripping_wall_left) {rigidbody_ptr->addForce(PLAYER_WALL_GRAVITY_FORCE);}
			else {rigidbody_ptr->addForce(PLAYER_GRAVITY_FORCE);}
		
		break;

		case STATE_THROWING:

			if(current_scythe_throw_frames == PLAYER_THROW_SCYTHE_FRAME)
			{
				// Add force :) //
				rigidbody_ptr->addForce(PLAYER_THROW_FORCE);

				// Add stretch for fun
				sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);
				sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);

				// Create Scythe //
				ScythePlatform* scythe_ptr = new ScythePlatform(dir, bn::fixed_point(x() + (dir * SCYTHE_X_OFFSET), 
																					 y() + SCYTHE_Y_OFFSET));
				scythe_ptr->setCamera(camera);
				
				// If a scythe already exists at the assigned index, delete it and create a new scythe.
				if(game_objects.at(SCYTHE_OBJECT_LIST_INDEX)->object_type == SCYTHE_PLATFORM)
				{
					delete game_objects.at(SCYTHE_OBJECT_LIST_INDEX);
					game_objects.at(SCYTHE_OBJECT_LIST_INDEX) = scythe_ptr;
				}
				// Otherwise, insert a new scythe for the first time, to the right of the Player index (0)
				else
				{
					bn::ivector<GameObject*>::iterator first = game_objects.begin();
					first++;
					game_objects.insert(first, scythe_ptr);
				}
				// Hide the scythe until it's had its first update
				scythe_ptr->sprite_ptr->set_visible(false);
			}

			// Update frame counter
			current_scythe_throw_frames++;
			current_scythe_throw_frames = clamp(0, 
												PLAYER_SCYTHE_THROW_FRAMES, 
												current_scythe_throw_frames);

			if(current_scythe_throw_frames != PLAYER_SCYTHE_THROW_FRAMES) {throw_scythe = true;}

		break;

		case STATE_DEAD:

			// Flesh out the death state later
			setPos(-64, 0);
			state = STATE_AIR_NEUTRAL;

		break;
		
		default:
		break;
    }

    ///////////////////
    // Apply Physics //
    ///////////////////
    
	// Apply Decay to Forces
    rigidbody_ptr->applyDecay();

	// Apply forces to player
    bn::fixed_point final_dir = applyForces();

	//////////////////////////////
	// Init Collision Variables //
	//////////////////////////////
    
	// Get a normalized direction vector, to be used for collision correction if
	// there is a collision.
	bn::fixed normalized_dir_x = 0;
	bn::fixed normalized_dir_y = 0;
	if(final_dir.x() != 0) {normalized_dir_x = final_dir.x() / abs(final_dir.x());}
	if(final_dir.y() != 0) {normalized_dir_y = final_dir.y() / abs(final_dir.y());}
	bn::fixed_point normalized_dir = bn::fixed_point(normalized_dir_x, normalized_dir_y);

	// Create one temporary collider for each axis. If a collider finds a collision
	// in its axis, move the temp collider AND the Player back along the dir vector
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

	// Get current cell index that player resides in:
	int32 half_room_width_pixels  = room.bg_ptr->dimensions().width() / 2;
	int32 half_room_height_pixels = room.bg_ptr->dimensions().height() / 2;
	bn::fixed index_x = (x() + half_room_width_pixels)  / TILE_WIDTH;
	bn::fixed index_y = (y() + half_room_height_pixels) / TILE_HEIGHT;
	bn::point cell_index = bn::point(index_x.integer(), index_y.integer());

	//////////////////////////////////
	// Resolve GameObject Collision //
	//////////////////////////////////

	for(int32 i = 0; i < game_objects.size(); i++)
    {
		other_collider_ptr = game_objects.at(i)->collider_ptr;

		switch(game_objects.at(i)->object_type)
		{
			case DEVIL_PLATFORM:
				
				if(collider_ptr->isCollision(*other_collider_ptr))
				{
					// Handle Default Collision Cases //
					while(temp_collider_x_ptr->isCollision(*other_collider_ptr))
					{
						if(normalized_dir.x() == 0) {kill_player = true; break;}
						temp_collider_x_ptr->setX(temp_collider_x_ptr->x() - normalized_dir.x());
						setX(this->x() - normalized_dir.x());
					}

					while(temp_collider_y_ptr->isCollision(*other_collider_ptr))
					{
						if(normalized_dir.y() == 0) {kill_player = true; break;}
						temp_collider_y_ptr->setY(temp_collider_y_ptr->y() - normalized_dir.y());
						setY(this->y() - normalized_dir.y());
					}

					// If there is still collision somehow, must be corner case //
					while(collider_ptr->isCollision(*(other_collider_ptr)))
					{
						if(normalized_dir.x() == 0) {kill_player = true; break;}
						// We always resolve diagonal corner collisions with a horizontal shift. 
						setX(this->x() - normalized_dir.x());
					}	
				}

			break;

			case ANGEL_PLATFORM:

				if(temp_collider_y_ptr->p4.y() <= other_collider_ptr->p1.y() + PLAYER_GRAVITY)
				{

					// Handle Corner Case //
					if(!temp_collider_x_ptr->isCollision(*(other_collider_ptr)) &&
					   !temp_collider_y_ptr->isCollision(*(other_collider_ptr)))
					{
						while(collider_ptr->isCollision(*(other_collider_ptr)))
						{setY(this->y() - 1);}
					}
				
					// Handle Remaining Collision Cases //
					else
					{

						while(temp_collider_y_ptr->isCollision(*other_collider_ptr))
						{temp_collider_y_ptr->setY(temp_collider_y_ptr->y() - 1);
							setY(this->y() - 1);}
					}
				}
				
			break;

			case SCYTHE_PLATFORM:
				
				if(temp_collider_y_ptr->p4.y() <= other_collider_ptr->p1.y() + PLAYER_GRAVITY)
				{

					// Handle Corner Case //
					if(!temp_collider_x_ptr->isCollision(*(other_collider_ptr)) &&
					   !temp_collider_y_ptr->isCollision(*(other_collider_ptr)))
					{
						while(collider_ptr->isCollision(*(other_collider_ptr)))
						{
							owp_grace_frames = PLAYER_OWP_SNAP_FRAMES;
							setY(this->y() - 1);
						}
					}
				
					// Handle Remaining Collision Cases //
					else
					{

						while(temp_collider_y_ptr->isCollision(*other_collider_ptr))
						{
							owp_grace_frames = PLAYER_OWP_SNAP_FRAMES;
							temp_collider_y_ptr->setY(temp_collider_y_ptr->y() - 1);
							setY(this->y() - 1);
						}
					}
				}
				
			break;

			default:
			break;
		}

		other_collider_ptr = NULL;
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
						
						// If Player snapped through a OWP this frame, handle differently
						// because player may have no move speed 
						if(owp_grace_frames)
						{
							// Ignore x axis, only resolve y axis
							while(temp_collider_y_ptr->isCollision(*other_collider_ptr))
							{
								if(normalized_dir.y() == 0)
								{
									temp_collider_y_ptr->setY(temp_collider_y_ptr->y() + 1);
									setY(this->y() + 1);

									// Cheeky fix, move scythe platform down one pixel. 
									if(game_objects.at(SCYTHE_OBJECT_LIST_INDEX) != NULL)
									{
										GameObject* temp = game_objects.at(SCYTHE_OBJECT_LIST_INDEX);
										temp->setY(temp->y() + 1);
										temp = NULL;
									}
								}
								else
								{
									temp_collider_y_ptr->setY(temp_collider_y_ptr->y() - normalized_dir.y());
									setY(this->y() - normalized_dir.y());
								}
							}

							// If there is still collision somehow, must be corner case //
							while(collider_ptr->isCollision(*(other_collider_ptr)))
							{
								if(normalized_dir.x() == 0) {break;}
								// We always resolve diagonal corner collisions with a horizontal shift. 
								setX(this->x() - normalized_dir.x());
							}
						}

						// If didn't snap to OWP this frame, handle the default way.
						else
						{
							// Handle Default Collision Cases //
							while(temp_collider_x_ptr->isCollision(*other_collider_ptr))
							{
								if(normalized_dir.x() == 0) {kill_player = true; break;}
								temp_collider_x_ptr->setX(temp_collider_x_ptr->x() - normalized_dir.x());
								setX(this->x() - normalized_dir.x());
							}

							while(temp_collider_y_ptr->isCollision(*other_collider_ptr))
							{
								if(normalized_dir.y() == 0) {kill_player = true; break;}
								temp_collider_y_ptr->setY(temp_collider_y_ptr->y() - normalized_dir.y());
								setY(this->y() - normalized_dir.y());
							}

							// If there is still collision somehow, must be corner case //
							while(collider_ptr->isCollision(*(other_collider_ptr)))
							{
								if(normalized_dir.x() == 0) {kill_player = true; break;}
								// We always resolve diagonal corner collisions with a horizontal shift. 
								setX(this->x() - normalized_dir.x());
							}
						}
					}

					delete other_collider_ptr;

				break;

				case ONEWAYBLOCK_INDEX:

					other_collider_ptr = new Collider(world_x, 
													  world_y + ONEWAYBLOCK_COLLIDER_Y_OFFSET, 
													  TILE_WIDTH, 
													  ONEWAYBLOCK_COLLIDER_HEIGHT);

					if(temp_collider_y_ptr->p4.y() <= other_collider_ptr->p1.y() + PLAYER_GRAVITY)
					{
						// Handle Corner Case //
						if(!temp_collider_x_ptr->isCollision(*(other_collider_ptr)) &&
						   !temp_collider_y_ptr->isCollision(*(other_collider_ptr)))
						{
							while(collider_ptr->isCollision(*(other_collider_ptr)))
							{
								setY(this->y() - 1);
							}
						}
					
						// Handle Remaining Collision Cases //
						else
						{
							while(temp_collider_y_ptr->isCollision(*(other_collider_ptr)))
							{
								temp_collider_y_ptr->setY(temp_collider_y_ptr->y() - 1);
								setY(this->y() - 1);
							}
						}
					}

					delete other_collider_ptr;

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
    bool wall_right_detected = false;
    bool wall_left_detected  = false;
    bool grounded_detected   = false;

	// Create test collider for grounded collision checks
	const uint32 ground_ray_length = 1;
	Collider*    test_collider_ptr = new Collider(collider_ptr->x(),
											      collider_ptr->y() + ground_ray_length,
												  collider_ptr->width,
												  collider_ptr->height);

	// Create test colliders for wall collision checks
	const uint32 wall_ray_length         = 1;
	Collider*    test_collider_right_ptr = new Collider(collider_ptr->x() + wall_ray_length,
													    collider_ptr->y(),
													    collider_ptr->width,
												 		collider_ptr->height);
	Collider*    test_collider_left_ptr  = new Collider(collider_ptr->x() - wall_ray_length,
													    collider_ptr->y(),
													    collider_ptr->width,
												 		collider_ptr->height);	

	/////////////////////////////////////
	// Get State Info from GameObjects //
	/////////////////////////////////////

	for(int32 i = 0; i < game_objects.size(); i++)
	{
		other_collider_ptr = game_objects.at(i)->collider_ptr;

		switch(game_objects.at(i)->object_type)
		{
			case DEVIL_PLATFORM:

				// Test for, and log grounded collision
				if(test_collider_ptr->isCollision(*other_collider_ptr) && normalized_dir.y() >= 0)
				{
					if(air_frames_elapsed == PLAYER_SQUISH_FRAMES_REQUIRED) 
					{sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H); 				
					 sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);}
					grounded_detected = true;
				}

				// Test for wall riding on right side
				if(test_collider_right_ptr->isCollision(*other_collider_ptr) && final_dir.y() >= 0)
				{wall_right_detected = true;}
				
				// Test for wall riding on left side
				if(test_collider_left_ptr->isCollision(*other_collider_ptr) && final_dir.y() >= 0)
				{wall_left_detected = true;}

			break;

			case ANGEL_PLATFORM:
			case SCYTHE_PLATFORM:
				
				if(temp_collider_y_ptr->p4.y() <= other_collider_ptr->p1.y() + PLAYER_GRAVITY)
				{
					// Test for, and log grounded collision
					if(test_collider_ptr->isCollision(*(other_collider_ptr)) && normalized_dir.y() >= 0)
					{
						if(air_frames_elapsed == PLAYER_SQUISH_FRAMES_REQUIRED) 
						{sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);
						 sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);}
						grounded_detected = true;
					}
				}
				
			break;

			default:
			break;
		}

		other_collider_ptr = NULL;
	}

	///////////////////////////////
    // Get State Info from Tiles //
    ///////////////////////////////							   

	for(int32 y = -1; y < 2; y++)
	{
		for(int32 x = -2; x < 3; x++)
		{
			// 1. Get tile type at index //
			int32 check_index_x = cell_index.x() + x;
			int32 check_index_y = cell_index.y() + y;

			// Clamp index values so we don't crash by going out of bounds.
			while(check_index_x < 0) {check_index_x++;}
			while(check_index_x > (room.bg_ptr->dimensions().width() / 8) - 1)  {check_index_x--;}

			while(check_index_y < 0) {check_index_y++;}
			while(check_index_y > (room.bg_ptr->dimensions().height() / 8) - 1) {check_index_y--;}

			// Determine world coords in case we need to make a collider.
			int32 world_x = ((check_index_x * TILE_WIDTH)  - half_room_width_pixels)  + (TILE_WIDTH / 2);
			int32 world_y = ((check_index_y * TILE_HEIGHT) - half_room_height_pixels) + (TILE_HEIGHT / 2);

			uint32 tile_index = room.getTileAtIndex(check_index_x, check_index_y);

			switch(tile_index)
			{
				case BLOCK_INDEX:

					other_collider_ptr = new Collider(world_x,
													  world_y, 
													  TILE_WIDTH, 
													  TILE_HEIGHT);

					// Test for, and log grounded collision
					if(test_collider_ptr->isCollision(*other_collider_ptr) && normalized_dir.y() >= 0)
					{
						if(air_frames_elapsed == PLAYER_SQUISH_FRAMES_REQUIRED) 
						{sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H); 				
						 sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);}
						grounded_detected = true;
					}

					// Test for wall riding on right side
					if(test_collider_right_ptr->isCollision(*other_collider_ptr) && final_dir.y() >= 0)
					{
						wall_right_detected = true;
					}
					
					// Test for wall riding on left side
					if(test_collider_left_ptr->isCollision(*other_collider_ptr) && final_dir.y() >= 0)
					{
						wall_left_detected = true;
					}

					delete other_collider_ptr;

				break;

				case ONEWAYBLOCK_INDEX:

					other_collider_ptr = new Collider(world_x, 
													  world_y + ONEWAYBLOCK_COLLIDER_Y_OFFSET, 
													  TILE_WIDTH, 
													  ONEWAYBLOCK_COLLIDER_HEIGHT);

					if(temp_collider_y_ptr->p4.y() <= other_collider_ptr->p1.y() + PLAYER_GRAVITY)
					{
						// Test for, and log grounded collision
						if(test_collider_ptr->isCollision(*(other_collider_ptr)) && normalized_dir.y() >= 0)
						{
							if(air_frames_elapsed == PLAYER_SQUISH_FRAMES_REQUIRED) 
							{sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);
						     sprite_ptr->set_vertical_scale(PLAYER_MIN_STRETCH_V);}
							grounded_detected = true;
						}
					}

					delete other_collider_ptr;

				break;

				default:
				break;
			}
		}
	}

	// Clean up temp colliders
	delete temp_collider_x_ptr;
	delete temp_collider_y_ptr;
	delete test_collider_ptr;
	delete test_collider_right_ptr;
	delete test_collider_left_ptr;
	
    ///////////////////
    // Update States //
    ///////////////////

	if(throw_scythe)
	{
		state = STATE_THROWING;
	}
    else if(grounded_detected)        
	{
		state = STATE_GROUNDED_NEUTRAL;
		air_frames_elapsed = 0;
	}
    else if(wall_right_detected) 
	{
		state = STATE_WALL_SLIDE_RIGHT;
		air_frames_elapsed = 0;
	}
    else if(wall_left_detected)  
	{
		state = STATE_WALL_SLIDE_LEFT;
		air_frames_elapsed = 0;
	}
    else {state = STATE_AIR_NEUTRAL;}

	if(kill_player) {state = STATE_DEAD;}

	///////////////////
	// Update Timers //
	///////////////////
	owp_grace_frames--;
	owp_grace_frames = clamp(0, PLAYER_OWP_SNAP_FRAMES, owp_grace_frames);

	received_platform_force = false;

	/////////////////////////////
	// Update Sprite Direction //
	/////////////////////////////
	
	if(dir == LEFT)        {sprite_ptr->set_horizontal_flip(true);}
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
