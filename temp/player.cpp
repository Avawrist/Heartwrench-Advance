#include "player.h"

Player::Player()
{
    sprite_ptr = bn::sprite_items::player_sprite.create_sprite(0, 0);
    setState(PLAYER_STATE_IDLE_DOWN);

    collider = Collider(x(), y(), COLLIDER_16x);
}

bn::fixed Player::x() const
{
    return sprite_ptr->x();
}

bn::fixed Player::y() const
{
    return sprite_ptr->y();
}

bn::fixed_point Player::position() const
{
    return sprite_ptr->position();
}

void Player::update(const Collider& other_collider)
{
    bn::fixed move_x = 0;
    bn::fixed move_y = 0;
    
    ///////////////
    // Get Input //
    ///////////////
    if(state == PLAYER_STATE_IDLE_UP ||
       state == PLAYER_STATE_IDLE_DOWN ||
       state == PLAYER_STATE_IDLE_LEFT ||
       state == PLAYER_STATE_IDLE_RIGHT ||
       state == PLAYER_STATE_WALK_UP ||
       state == PLAYER_STATE_WALK_DOWN ||
       state == PLAYER_STATE_WALK_LEFT ||
       state == PLAYER_STATE_WALK_RIGHT)
    {
	if(bn::keypad::up_held())
	{
	    if(state == PLAYER_STATE_WALK_RIGHT || state == PLAYER_STATE_WALK_LEFT)
	    {
		//setPos(sprite_ptr->x(), sprite_ptr->y() - walk_speed);
	        move_y -= walk_speed;
	    }
	    else
	    {
		//setPos(sprite_ptr->x(), sprite_ptr->y() - walk_speed);
	        move_y -= walk_speed;
		if(state != PLAYER_STATE_WALK_UP)
		{setState(PLAYER_STATE_WALK_UP);}
	    }
	}
	if(bn::keypad::down_held())
	{
	    if(state == PLAYER_STATE_WALK_RIGHT || state == PLAYER_STATE_WALK_LEFT)
	    {
		//setPos(sprite_ptr->x(), sprite_ptr->y() + walk_speed);
	        move_y += walk_speed;
	    }
	    else
	    {
		//setPos(sprite_ptr->x(), sprite_ptr->y() + walk_speed);
	        move_y += walk_speed;
		if(state != PLAYER_STATE_WALK_DOWN)
		{setState(PLAYER_STATE_WALK_DOWN);}
	    }
	}
	if(bn::keypad::left_held())
	{
	    if(state == PLAYER_STATE_WALK_UP || state == PLAYER_STATE_WALK_DOWN)
	    {
		//setPos(sprite_ptr->x() - walk_speed, sprite_ptr->y());
	        move_x -= walk_speed;
	    }
	    else
	    {
		//setPos(sprite_ptr->x() - walk_speed, sprite_ptr->y());
	        move_x -= walk_speed;
		if(state != PLAYER_STATE_WALK_LEFT)
		{setState(PLAYER_STATE_WALK_LEFT);}
	    }
	}
	if(bn::keypad::right_held())
	{
	    if(state == PLAYER_STATE_WALK_UP || state == PLAYER_STATE_WALK_DOWN)
	    {
		//setPos(sprite_ptr->x() + walk_speed, sprite_ptr->y());
	        move_x += walk_speed;
	    }
	    else
	    {
		//setPos(sprite_ptr->x() + walk_speed, sprite_ptr->y());
	        move_x += walk_speed;
		if(state != PLAYER_STATE_WALK_RIGHT)
		{setState(PLAYER_STATE_WALK_RIGHT);}
	    }
	}
    
	if(state == PLAYER_STATE_WALK_UP && bn::keypad::up_released())
	{
	    if(bn::keypad::left_held())
	    {
		setState(PLAYER_STATE_WALK_LEFT);
	    }
	    if(bn::keypad::right_held())
	    {
		setState(PLAYER_STATE_WALK_RIGHT);
	    }
	    else
	    {
		setState(PLAYER_STATE_IDLE_UP);
	    }
	}
	if(state == PLAYER_STATE_WALK_DOWN && bn::keypad::down_released())
	{
	    if(bn::keypad::left_held())
	    {
		setState(PLAYER_STATE_WALK_LEFT);
	    }
	    if(bn::keypad::right_held())
	    {
		setState(PLAYER_STATE_WALK_RIGHT);
	    }
	    else
	    {
		setState(PLAYER_STATE_IDLE_DOWN);
	    }
	}
	if(state == PLAYER_STATE_WALK_LEFT && bn::keypad::left_released())
	{
	    if(bn::keypad::up_held())
	    {
		setState(PLAYER_STATE_WALK_UP);
	    }
	    if(bn::keypad::down_held())
	    {
		setState(PLAYER_STATE_WALK_DOWN);
	    }
	    else
	    {
		setState(PLAYER_STATE_IDLE_LEFT);
	    }
	}
	if(state == PLAYER_STATE_WALK_RIGHT && bn::keypad::right_released())
	{
	    if(bn::keypad::up_held())
	    {
		setState(PLAYER_STATE_WALK_UP);
	    }
	    if(bn::keypad::down_held())
	    {
		setState(PLAYER_STATE_WALK_DOWN);
	    }
	    else
	    {
		setState(PLAYER_STATE_IDLE_RIGHT);
	    }
	}
	if(bn::keypad::b_pressed())
	{
	    if(state == PLAYER_STATE_WALK_UP ||
	       state == PLAYER_STATE_IDLE_UP)
	    {
		setState(PLAYER_STATE_TWIRL_UP);
		if(spinner.state == SPINNER_STATE_IDLE)
		{
		    spinner.setState(SPINNER_STATE_LAUNCH);
		    spinner.setDir(0, -1);
		}
	    }
	    if(state == PLAYER_STATE_WALK_DOWN ||
	       state == PLAYER_STATE_IDLE_DOWN)
	    {
		setState(PLAYER_STATE_TWIRL_DOWN);
		if(spinner.state == SPINNER_STATE_IDLE)
		{
		    spinner.setState(SPINNER_STATE_LAUNCH);
		    spinner.setDir(0, 1);
		}
	    }
	    if(state == PLAYER_STATE_WALK_LEFT ||
	       state == PLAYER_STATE_IDLE_LEFT)
	    {
		setState(PLAYER_STATE_TWIRL_LEFT);
		if(spinner.state == SPINNER_STATE_IDLE)
		{
		    spinner.setState(SPINNER_STATE_LAUNCH);
		    spinner.setDir(-1, 0);
		}
	    }
	    if(state == PLAYER_STATE_WALK_RIGHT ||
	       state == PLAYER_STATE_IDLE_RIGHT)
	    {
		setState(PLAYER_STATE_TWIRL_RIGHT);
		if(spinner.state == SPINNER_STATE_IDLE)
		{
		    spinner.setState(SPINNER_STATE_LAUNCH);
		    spinner.setDir(1, 0);
		}
	    }
	}
    }

    if(state == PLAYER_STATE_TWIRL_UP ||
       state == PLAYER_STATE_TWIRL_DOWN ||
       state == PLAYER_STATE_TWIRL_LEFT ||
       state == PLAYER_STATE_TWIRL_RIGHT)
    {
	twirl_timer -= 1;
	if(twirl_timer <= 0)
	{
	    twirl_timer = TWIRL_TIMER_FRAMES;
	    if(state == PLAYER_STATE_TWIRL_UP) {setState(PLAYER_STATE_IDLE_UP);}
	    if(state == PLAYER_STATE_TWIRL_DOWN) {setState(PLAYER_STATE_IDLE_DOWN);}
	    if(state == PLAYER_STATE_TWIRL_LEFT) {setState(PLAYER_STATE_IDLE_LEFT);}
	    if(state == PLAYER_STATE_TWIRL_RIGHT) {setState(PLAYER_STATE_IDLE_RIGHT);}
	}
    }

    spinner.update(sprite_ptr->x(), sprite_ptr->y());

    ////////////////////
    // Apply Movement //
    ////////////////////

    while(collider.willCollide(other_collider, bn::fixed_point(move_x, 0)))
    {
	if(move_x > 0) {move_x -= 1;}
	else if(move_x < 0) {move_x += 1;}
    }
    while(collider.willCollide(other_collider, bn::fixed_point(0, move_y)))
    {
	if(move_y > 0) {move_y -= 1;}
	else if(move_y < 0) {move_y += 1;}
    }
    setX(x() + move_x);
    setY(y() + move_y);
}

void Player::draw()
{
    sprite_ptr->set_z_order(sprite_ptr->y().round_integer() * -1);
    animate_action_ptr->update();
    spinner.draw();
}

void Player::setCamera(const bn::camera_ptr& camera)
{
    sprite_ptr->set_camera(camera);
    collider.setCamera(camera);
    spinner.sprite_ptr->set_camera(camera);
}

void Player::setState(PlayerState new_state)
{
    state = new_state;
    
    switch(new_state)
    {
    case PLAYER_STATE_IDLE_UP:
	animate_action_ptr = bn::create_sprite_animate_action_forever(
	    sprite_ptr.value(),
	    2,
	    bn::sprite_items::player_sprite.tiles_item(),
	    2, 2);
	sprite_ptr->set_horizontal_flip(false);
	break;
    case PLAYER_STATE_IDLE_DOWN:
	animate_action_ptr = bn::create_sprite_animate_action_forever(
	    sprite_ptr.value(),
	    2,
	    bn::sprite_items::player_sprite.tiles_item(),
	    0, 0);
	sprite_ptr->set_horizontal_flip(false);
	break;
    case PLAYER_STATE_IDLE_LEFT:
	animate_action_ptr = bn::create_sprite_animate_action_forever(
	    sprite_ptr.value(),
	    1,
	    bn::sprite_items::player_sprite.tiles_item(),
	    1, 1);
	sprite_ptr->set_horizontal_flip(false);
	break;
    case PLAYER_STATE_IDLE_RIGHT:
	animate_action_ptr = bn::create_sprite_animate_action_forever(
	    sprite_ptr.value(),
	    2,
	    bn::sprite_items::player_sprite.tiles_item(),
	    1, 1);
        sprite_ptr->set_horizontal_flip(true);
	break;
    case PLAYER_STATE_WALK_DOWN:
	animate_action_ptr = bn::create_sprite_animate_action_forever(
	    sprite_ptr.value(),
	    2,
	    bn::sprite_items::player_sprite.tiles_item(),
	    3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
	sprite_ptr->set_horizontal_flip(false);
	break;
    case PLAYER_STATE_WALK_UP:
	animate_action_ptr = bn::create_sprite_animate_action_forever(
	    sprite_ptr.value(),
	    2,
	    bn::sprite_items::player_sprite.tiles_item(),
	    23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
	sprite_ptr->set_horizontal_flip(false);
	break;
    case PLAYER_STATE_WALK_LEFT:
	animate_action_ptr = bn::create_sprite_animate_action_forever(
	    sprite_ptr.value(),
	    2,
	    bn::sprite_items::player_sprite.tiles_item(),
	    13, 14, 15, 16, 17, 18, 19, 20, 21, 22);
	sprite_ptr->set_horizontal_flip(false);
	break;
    case PLAYER_STATE_WALK_RIGHT:
	animate_action_ptr = bn::create_sprite_animate_action_forever(
	    sprite_ptr.value(),
	    2,
	    bn::sprite_items::player_sprite.tiles_item(),
	    13, 14, 15, 16, 17, 18, 19, 20, 21, 22);
	sprite_ptr->set_horizontal_flip(true);
	break;
    case PLAYER_STATE_TWIRL_UP:
	animate_action_ptr = bn::create_sprite_animate_action_forever(
	    sprite_ptr.value(),
	    2,
	    bn::sprite_items::player_sprite.tiles_item(),
	    53, 54, 55, 56, 57, 58, 59, 60, 61, 62);
	sprite_ptr->set_horizontal_flip(false);
	break;
    case PLAYER_STATE_TWIRL_DOWN:
	animate_action_ptr = bn::create_sprite_animate_action_forever(
	    sprite_ptr.value(),
	    2,
	    bn::sprite_items::player_sprite.tiles_item(),
	    33, 34, 35, 36, 37, 38, 39, 40, 41, 42);
	sprite_ptr->set_horizontal_flip(false);
	break;
    case PLAYER_STATE_TWIRL_LEFT:
	animate_action_ptr = bn::create_sprite_animate_action_forever(
	    sprite_ptr.value(),
	    2,
	    bn::sprite_items::player_sprite.tiles_item(),
	    43, 44, 45, 46, 47, 48, 49, 50, 51, 52);
	sprite_ptr->set_horizontal_flip(false);
	break;
    case PLAYER_STATE_TWIRL_RIGHT:
	animate_action_ptr = bn::create_sprite_animate_action_forever(
	    sprite_ptr.value(),
	    2,
	    bn::sprite_items::player_sprite.tiles_item(),
	    43, 44, 45, 46, 47, 48, 49, 50, 51, 52);
	sprite_ptr->set_horizontal_flip(true);
	break;
    default:
	break;
    }
}

void Player::setX(bn::fixed new_x)
{
    sprite_ptr->set_x(new_x);
    collider.setX(new_x);
}

void Player::setY(bn::fixed new_y)
{
    sprite_ptr->set_y(new_y);
    collider.setY(new_y);
}

void Player::setPos(bn::fixed new_x, bn::fixed new_y)
{
    sprite_ptr->set_x(new_x);
    sprite_ptr->set_y(new_y);

    collider.setX(new_x);
    collider.setY(new_y);
}

void Player::setPos(bn::fixed_point new_pos)
{
    sprite_ptr->set_position(new_pos);
    collider.setPos(new_pos);
}
