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

void Player::update(GameObject** game_objects_p, uint8 game_objects_size)
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
	if(bn::keypad::left_held())  {rigidbody_p->addForce(getForceWalkLeft());}
	if(bn::keypad::right_held()) {rigidbody_p->addForce(getForceWalkRight());}
	if(bn::keypad::a_pressed())
	{
	    rigidbody_p->addForce(getForceJump());
	    sprite_ptr->set_vertical_scale(PLAYER_MAX_STRETCH_V);
	}

	break;
	
    case STATE_AIR_NEUTRAL:

	//////////////////////////////
	// Player Air Neutral State //
	//////////////////////////////

	// Get Input //
	if(bn::keypad::left_held())  {rigidbody_p->addForce(getForceWalkLeft());}
	if(bn::keypad::right_held()) {rigidbody_p->addForce(getForceWalkRight());}
	
	// Add Gravity //
	rigidbody_p->addForce(getForceGravity());
    	
	break;
	
    case STATE_WALL_SLIDE_RIGHT:

	///////////////////////////////////
	// Player Wall Slide Right State //
	///////////////////////////////////

	// Get Input //
	if(bn::keypad::left_held())  {rigidbody_p->addForce(getForceWalkLeft());}
	if(bn::keypad::right_held()) {rigidbody_p->addForce(getForceWalkRight());}
	if(bn::keypad::a_pressed())
	{
	    rigidbody_p->addForce(getForceWallJumpLeft());
	    sprite_ptr->set_vertical_scale(PLAYER_MAX_STRETCH_V);
	}
	
	// Add Gravity //
	rigidbody_p->addForce(getForceWallGravity());
	
	break;
	
    case STATE_WALL_SLIDE_LEFT:

	// Get Input //
	if(bn::keypad::left_held())  {rigidbody_p->addForce(getForceWalkLeft());}
	if(bn::keypad::right_held()) {rigidbody_p->addForce(getForceWalkRight());}
	if(bn::keypad::a_pressed())
	{
	    rigidbody_p->addForce(getForceWallJumpRight());
	    sprite_ptr->set_vertical_scale(PLAYER_MAX_STRETCH_V);
	}
	
	// Add Gravity //
	rigidbody_p->addForce(getForceWallGravity());
	
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
    
    bn::fixed_point final_dir = rigidbody_p->applyForces(*this);

    ///////////////////////
    // Resolve Collision //
    ///////////////////////
    
    bool wall_right_detected = false;
    bool wall_left_detected  = false;
    bool grounded_detected   = false;

    for(uint8 i = 0; i < game_objects_size; i++)
    {
	if(game_objects_p[i] != this)
	{
	    Collider* other_collider_p = game_objects_p[i]->collider_p;
	    if(other_collider_p != NULL)
	    {
		if(collider_p->isCollision(*(other_collider_p)))
		{
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
		    delete temp_collider_x_p;
		    delete temp_collider_y_p;
		}
	    }
	}
    }

    ////////////////////
    // Get State Info //
    ////////////////////
    BN_LOG("", x());
    for(uint8 i = 0; i < game_objects_size; i++)
    {
	if(game_objects_p[i] != this)
	{
	    Collider* other_collider_p = game_objects_p[i]->collider_p;
	    if(other_collider_p != NULL)
	    {
		// Test for, and log grounded collision if any //
		const uint8 ground_ray_length = 1;
		Collider*   test_collider_p = new Collider(collider_p->x(),
							   collider_p->y() + ground_ray_length,
							   collider_p->size);
		if(test_collider_p->isCollision(*(other_collider_p)))
		{
		    if(state == STATE_AIR_NEUTRAL) {sprite_ptr->set_horizontal_scale(PLAYER_MAX_STRETCH_H);}
		    grounded_detected = true;
		}
		delete test_collider_p;

		// Test for, and log wall collision if any //
		const uint8 wall_ray_length = 1;
	        Collider* test_collider_right_p = new Collider(collider_p->x() + wall_ray_length,
							       collider_p->y(),
							       collider_p->size);
		Collider* test_collider_left_p = new Collider(collider_p->x() - wall_ray_length,
							       collider_p->y(),
							       collider_p->size);

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
		delete test_collider_right_p;
		delete test_collider_left_p;
	    }
	}
    }
    
    // Apply Decay to Forces
    rigidbody_p->applyDecay();
    
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

Force* Player::getForceWalkLeft()
{
    return new Force(bn::fixed_point_t<12>(-walk_speed, 0), 1);
}

Force* Player::getForceWalkRight()
{
    return new Force(bn::fixed_point_t<12>(walk_speed, 0), 1);
}

Force* Player::getForceWalkUp()
{
    return new Force(bn::fixed_point_t<12>(0, -walk_speed), 1);
}

Force* Player::getForceWalkDown()
{
    return new Force(bn::fixed_point_t<12>(0, walk_speed), 1);
}

Force* Player::getForceJump()
{
    return new Force(bn::fixed_point_t<12>(0, jump_force), 0.1);
}

Force* Player::getForceWallJumpRight()
{
    return new Force(bn::fixed_point_t<12>(wall_jump_force.x(), wall_jump_force.y()), 0.1);
}

Force* Player::getForceWallJumpLeft()
{
    return new Force(bn::fixed_point_t<12>(-wall_jump_force.x(), wall_jump_force.y()), 0.1);
}

Force* Player::getForceGravity()
{
    return new Force(bn::fixed_point_t<12>(0, gravity), 1);
}

Force* Player::getForceWallGravity()
{
    return new Force(bn::fixed_point_t<12>(0, wall_ride_gravity), 1);
}
