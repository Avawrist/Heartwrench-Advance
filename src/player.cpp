#include "player.h"

///////////////////
// Struct Player //
///////////////////

Player::Player()
{
	// Initialize all Player variables
	object_type = PLAYER;
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
	dash_force        	 = PLAYER_DASH_FORCE;
    wall_jump_force   	 = bn::fixed_point(PLAYER_WALL_JUMP_X_FORCE, 
										   PLAYER_WALL_JUMP_Y_FORCE);
    gravity           	 = PLAYER_GRAVITY;
	wall_ride_gravity 	 = PLAYER_WALL_RIDE_GRAVITY;
	
	remaining_jump_input_frames      = 0;
	remaining_x_drift_lockout_frames = 0;
	air_frames_elapsed               = 0;
}

Player::~Player()
{
    delete rigidbody_ptr;
    delete collider_ptr;
}

void Player::update(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects)
{
    //////////////////////////
    // Player State Machine //
    //////////////////////////

	bool gripping_wall_right = false;
	bool gripping_wall_left  = false;
	bool kill_player         = false;

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
			if(bn::keypad::left_held())       {rigidbody_ptr->addForce(PLAYER_X_LEFT_FORCE);}
			else if(bn::keypad::right_held()) {rigidbody_ptr->addForce(PLAYER_X_RIGHT_FORCE);}

			// Jump
			if(bn::keypad::a_pressed())
			{
				remaining_jump_input_frames = PLAYER_MAX_JUMP_INPUT_FRAMES;
				rigidbody_ptr->addForce(PLAYER_JUMP_FORCE);
				sprite_ptr->set_vertical_scale(PLAYER_MAX_STRETCH_V);
			}

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
			if(bn::keypad::left_held() && !remaining_x_drift_lockout_frames)       {rigidbody_ptr->addForce(PLAYER_X_LEFT_FORCE);}
			else if(bn::keypad::right_held() && !remaining_x_drift_lockout_frames) {rigidbody_ptr->addForce(PLAYER_X_RIGHT_FORCE);}
			
			if(bn::keypad::a_held() && remaining_jump_input_frames > 0)
			{
				rigidbody_ptr->addForce(PLAYER_SECONDARY_JUMP_FORCE);
			}
			else if(bn::keypad::a_released()) {remaining_jump_input_frames = 0;}
			
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
				remaining_x_drift_lockout_frames = PLAYER_X_DRIFT_LOCKOUT_FRAMES;
			}
			if(bn::keypad::left_held() && !remaining_x_drift_lockout_frames) {rigidbody_ptr->addForce(PLAYER_X_LEFT_FORCE);}
			else if(bn::keypad::right_held()) 								 {gripping_wall_right = true;}
			
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
				remaining_x_drift_lockout_frames = PLAYER_X_DRIFT_LOCKOUT_FRAMES;
			}
			if(bn::keypad::left_held())  	  									   {gripping_wall_left = true;}
			else if(bn::keypad::right_held() && !remaining_x_drift_lockout_frames) {rigidbody_ptr->addForce(PLAYER_X_RIGHT_FORCE);}
			
			// Add Gravity //
			if(gripping_wall_left) {rigidbody_ptr->addForce(PLAYER_WALL_GRAVITY_FORCE);}
			else {rigidbody_ptr->addForce(PLAYER_GRAVITY_FORCE);}
		
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
    bn::fixed_point final_dir = rigidbody_ptr->applyForces(*this);

    ///////////////////////
    // Resolve Collision //
    ///////////////////////
    
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

	// Initialize state variables, to be updated on collision.
    bool wall_right_detected = false;
    bool wall_left_detected  = false;
    bool grounded_detected   = false;

    for(int32 i = 0; i < game_objects.size(); i++)
    {
		if(game_objects.at(i) != this)
		{
			Collider* other_collider_ptr = game_objects.at(i)->collider_ptr;
			if(other_collider_ptr != NULL && collider_ptr->isCollision(*(other_collider_ptr)))
			{
				switch(game_objects.at(i)->object_type)
				{
					case BLOCK:
					case MOVING_PLATFORM:

						// Handle Default Collision Cases //
						while(temp_collider_x_ptr->isCollision(*other_collider_ptr))
						{
							if(normalized_dir.x() == 0) {kill_player = true; break;}
							temp_collider_x_ptr->setX(temp_collider_x_ptr->x() - normalized_dir.x());
							setX(x() - normalized_dir.x());
						}

						while(temp_collider_y_ptr->isCollision(*other_collider_ptr))
						{
							if(normalized_dir.y() == 0) {kill_player = true; break;}
							temp_collider_y_ptr->setY(temp_collider_y_ptr->y() - normalized_dir.y());
							setY(y() - normalized_dir.y());
						}

						// If there is still collision somehow, must be corner case //
						while(collider_ptr->isCollision(*(other_collider_ptr)))
						{
							if(normalized_dir.x() == 0) {kill_player = true; break;}
							// We always resolve diagonal corner collisions with a horizontal shift. 
							setX(x() - normalized_dir.x());
						}
						
					break;

					case ONE_WAY_BLOCK:

						if(temp_collider_y_ptr->p4.y() <= other_collider_ptr->p1.y() + PLAYER_GRAVITY)
						{
							// Handle Corner Case //
							if(!temp_collider_x_ptr->isCollision(*(other_collider_ptr)) &&
								!temp_collider_y_ptr->isCollision(*(other_collider_ptr)))
							{
								while(collider_ptr->isCollision(*(other_collider_ptr)))
								{
									setY(y() - 1);
								}
							}
						
							// Handle Remaining Collision Cases //
							else
							{
								while(temp_collider_y_ptr->isCollision(*(other_collider_ptr)))
								{
									temp_collider_y_ptr->setY(temp_collider_y_ptr->y() - 1);
									setY(y() - 1);
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

    ////////////////////
    // Get State Info //
    ////////////////////

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

    for(int32 i = 0; i < game_objects.size(); i++)
    {
		if(game_objects.at(i) != this)
		{
			Collider* other_collider_ptr = game_objects.at(i)->collider_ptr;
			switch(game_objects.at(i)->object_type)
			{
				case BLOCK:
				case MOVING_PLATFORM:

					// Test for, and log grounded collision
					if(test_collider_ptr->isCollision(*other_collider_ptr) && normalized_dir.y() >= 0)
					{
						if(air_frames_elapsed == PLAYER_SQUISH_FRAMES_REQUIRED) {sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);}
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

				break;

				case ONE_WAY_BLOCK:

					if(temp_collider_y_ptr->p4.y() <= other_collider_ptr->p1.y() + PLAYER_GRAVITY)
					{
						// Test for, and log grounded collision
						if(test_collider_ptr->isCollision(*(other_collider_ptr)) && normalized_dir.y() >= 0)
						{
							if(air_frames_elapsed == PLAYER_SQUISH_FRAMES_REQUIRED) {sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);}
							grounded_detected = true;
						}
					}

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
    if(grounded_detected)        
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

void Player::draw()
{
	animate_action_ptr->update();
}

void Player::setCamera(const bn::camera_ptr& camera)
{
	sprite_ptr->set_camera(camera);
    collider_ptr->setCamera(camera);
}

bn::fixed Player::x() const
{
    return sprite_ptr->x().integer();
}

bn::fixed Player::y() const
{
    return sprite_ptr->y().integer();
}

bn::fixed_point Player::pos() const
{
    bn::fixed_point point(sprite_ptr->position().x().integer(),
			  			  sprite_ptr->position().y().integer());
    return point; 
}

void Player::setX(bn::fixed new_x)
{
    sprite_ptr->set_x(new_x.integer());
    collider_ptr->setX(new_x.integer() + collider_offset_x);
}

void Player::setY(bn::fixed new_y)
{
    sprite_ptr->set_y(new_y.integer());
    collider_ptr->setY(new_y.integer() + collider_offset_y);
}

void Player::setPos(bn::fixed new_x, bn::fixed new_y)
{
    sprite_ptr->set_x(new_x.integer());
    sprite_ptr->set_y(new_y.integer());
	collider_ptr->setX(new_x.integer() + collider_offset_x);
	collider_ptr->setY(new_y.integer() + collider_offset_y);
}

void Player::setPos(bn::fixed_point new_pos)
{
    sprite_ptr->set_x(new_pos.x().integer());
    sprite_ptr->set_y(new_pos.y().integer());
	collider_ptr->setX(new_pos.x().integer() + collider_offset_x);
	collider_ptr->setY(new_pos.y().integer() + collider_offset_y);
}
