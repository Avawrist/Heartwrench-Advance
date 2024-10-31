#include "player.h"

///////////////////
// Struct Player //
///////////////////

Player::Player()
{
    sprite_ptr  = bn::sprite_items::player.create_sprite(0, 0);
    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  2,
								  bn::sprite_items::player.tiles_item(),
								  0,
								  0);

    rigidbody_p       = new RigidBody();
    collider_p        = new Collider(x(), y(), COLLIDER_16);
    state             = STATE_AIR_NEUTRAL;
    walk_speed        = 1;
    jump_force        = -10;
    wall_jump_force   = bn::fixed_point(5, -8);
    gravity           = 2;
    wall_ride_gravity = 1;
}

Player::~Player()
{
    delete rigidbody_p;
    delete collider_p;
}

void Player::update(GameObject** game_objects_p, uint32 game_objects_size)
{
    //////////////////////////
    // Player State Machine //
    //////////////////////////

    switch(state)
    {
		case STATE_GROUNDED_NEUTRAL:

		///////////////////////////////////
		// Player Grounded Neutral State //
		///////////////////////////////////

		// Get Input //
		if(bn::keypad::left_held())  {rigidbody_p->addForce(PLAYER_WALK_LEFT_FORCE);}
		if(bn::keypad::right_held()) {rigidbody_p->addForce(PLAYER_WALK_RIGHT_FORCE);}
		if(bn::keypad::a_pressed())
		{
			rigidbody_p->addForce(PLAYER_JUMP_FORCE);
			sprite_ptr->set_vertical_scale(PLAYER_MAX_STRETCH_V);
		}

		break;
	
		case STATE_AIR_NEUTRAL:

		//////////////////////////////
		// Player Air Neutral State //
		//////////////////////////////

		// Get Input //
		if(bn::keypad::left_held())  {rigidbody_p->addForce(PLAYER_WALK_LEFT_FORCE);}
		if(bn::keypad::right_held()) {rigidbody_p->addForce(PLAYER_WALK_RIGHT_FORCE);}
		
		// Add Gravity //
		rigidbody_p->addForce(PLAYER_GRAVITY_FORCE);
			
		break;
		
		case STATE_WALL_SLIDE_RIGHT:

		///////////////////////////////////
		// Player Wall Slide Right State //
		///////////////////////////////////

		// Get Input //
		if(bn::keypad::left_held())  {rigidbody_p->addForce(PLAYER_WALK_LEFT_FORCE);}
		if(bn::keypad::right_held()) {rigidbody_p->addForce(PLAYER_WALK_RIGHT_FORCE);}
		if(bn::keypad::a_pressed())
		{
			rigidbody_p->addForce(PLAYER_WALL_JUMP_LEFT_FORCE);
			sprite_ptr->set_vertical_scale(PLAYER_MAX_STRETCH_V);
		}
		
		// Add Gravity //
		rigidbody_p->addForce(PLAYER_WALL_GRAVITY_FORCE);
		
		break;
		
		case STATE_WALL_SLIDE_LEFT:

		// Get Input //
		if(bn::keypad::left_held())  {rigidbody_p->addForce(PLAYER_WALK_LEFT_FORCE);}
		if(bn::keypad::right_held()) {rigidbody_p->addForce(PLAYER_WALK_RIGHT_FORCE);}
		if(bn::keypad::a_pressed())
		{
			rigidbody_p->addForce(PLAYER_WALL_JUMP_RIGHT_FORCE);
			sprite_ptr->set_vertical_scale(PLAYER_MAX_STRETCH_V);
		}
		
		// Add Gravity //
		rigidbody_p->addForce(PLAYER_WALL_GRAVITY_FORCE);
		
		//////////////////////////////////
		// Player Wall Slide Left State //
		//////////////////////////////////
		
		break;
		
		default:
		break;
    }
    
    ///////////////////
    // Apply Physics //
    ///////////////////
    
	// Apply Decay to Forces
    rigidbody_p->applyDecay();

    bn::fixed_point final_dir = rigidbody_p->applyForces(*this);

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
	Collider* temp_collider_x_p = new Collider(collider_p->x(),
											   collider_p->y() - final_dir.y(),
											   collider_p->size);
	Collider* temp_collider_y_p = new Collider(collider_p->x() - final_dir.x(),
											   collider_p->y(),
											   collider_p->size);

	// Initialize state variables, to be updated on collision.
    bool wall_right_detected = false;
    bool wall_left_detected  = false;
    bool grounded_detected   = false;

    for(uint32 i = 0; i < game_objects_size; i++)
    {
		if(game_objects_p[i] != this)
		{
			Collider* other_collider_p = game_objects_p[i]->collider_p;
			if(other_collider_p != NULL)
			{
				if(collider_p->isCollision(*(other_collider_p)))
				{
					// Handle Corner Case //
					if(!temp_collider_x_p->isCollision(*(other_collider_p)) &&
					   !temp_collider_y_p->isCollision(*(other_collider_p)))
					{
						while(collider_p->isCollision(*(other_collider_p)))
						{
							// We always resolve diagonal corner collisions with a horizontal shift. 
							setX(x() - normalized_dir.x());
						}
					}
				
					// Handle Remaining Collision Cases //
					else
					{
						while(temp_collider_x_p->isCollision(*(other_collider_p)))
						{
							temp_collider_x_p->setX(temp_collider_x_p->x() - normalized_dir.x());
							setX(x() - normalized_dir.x());
						}

						while(temp_collider_y_p->isCollision(*(other_collider_p)))
						{
							temp_collider_y_p->setY(temp_collider_y_p->y() - normalized_dir.y());
							setY(y() - normalized_dir.y());
						}
					}
				}
			}
		}
    }

    ////////////////////
    // Get State Info //
    ////////////////////

	// Create test collider for grounded collision checks
	const uint32 ground_ray_length = 1;
	Collider*    test_collider_p   = new Collider(collider_p->x(),
												  collider_p->y() + ground_ray_length,
												  collider_p->size);

	// Create test colliders for wall collision checks
	const uint32 wall_ray_length       = 1;
	Collider*    test_collider_right_p = new Collider(collider_p->x() + wall_ray_length,
													  collider_p->y(),
													  collider_p->size);
	Collider*    test_collider_left_p  = new Collider(collider_p->x() - wall_ray_length,
													  collider_p->y(),
													  collider_p->size);										   

    for(uint32 i = 0; i < game_objects_size; i++)
    {
		if(game_objects_p[i] != this)
		{
			Collider* other_collider_p = game_objects_p[i]->collider_p;
			if(other_collider_p != NULL)
			{
				// Test for, and log grounded collision
				if(test_collider_p->isCollision(*(other_collider_p)))
				{
					if(state == STATE_AIR_NEUTRAL) {sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);}
					grounded_detected = true;
				}

				// Test for wall riding on right side
				if(test_collider_right_p->isCollision(*other_collider_p) &&
				   final_dir.y() >= 0 &&
				   bn::keypad::right_held())
				{
					wall_right_detected = true;
				}

				// Test for wall riding on left side
				if(test_collider_left_p->isCollision(*other_collider_p) &&
				   final_dir.y() >= 0 &&
				   bn::keypad::left_held())
				{
					wall_left_detected = true;
				}
			}
		}
    }

	// Clean up temp colliders
	delete temp_collider_x_p;
	delete temp_collider_y_p;
	delete test_collider_p;
	delete test_collider_right_p;
	delete test_collider_left_p;
    
    ///////////////////
    // Update States //
    ///////////////////
    if(grounded_detected)        {state = STATE_GROUNDED_NEUTRAL;}
    else if(wall_right_detected) {state = STATE_WALL_SLIDE_RIGHT;}
    else if(wall_left_detected)  {state = STATE_WALL_SLIDE_LEFT;}
    else                         {state = STATE_AIR_NEUTRAL;}
    
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
