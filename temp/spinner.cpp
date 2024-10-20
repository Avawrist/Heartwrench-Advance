#include "spinner.h"

Spinner::Spinner()
{
    sprite_ptr = bn::sprite_items::spinner_sprite.create_sprite(0, 0);
    setState(SPINNER_STATE_IDLE);
}

void Spinner::update(bn::fixed player_x, bn::fixed player_y)
{
    if(state == SPINNER_STATE_IDLE)
    {
	setPos(player_x, player_y);
    }
    
    if(state == SPINNER_STATE_LAUNCH)
    {
	if(launch_timer <= 0)
	{
	    launch_timer = MAX_SPINNER_LAUNCH_FRAMES;
	    setState(SPINNER_STATE_IDLE);
	}
	else
	{
	    setPos(sprite_ptr->x() + (x_dir * move_speed),
		   sprite_ptr->y() + (y_dir * move_speed));
	    launch_timer -= 1;
	}
    }
}

void Spinner::draw()
{
    sprite_ptr->set_z_order(sprite_ptr->y().round_integer() * -1);
    animate_action_ptr->update();
}

void Spinner::setState(SpinnerState new_state)
{
    state = new_state;

    switch(new_state)
    {
    case SPINNER_STATE_IDLE:
	animate_action_ptr = bn::create_sprite_animate_action_forever(
	    sprite_ptr.value(),
	    4,
	    bn::sprite_items::spinner_sprite.tiles_item(),
	    0, 1, 2, 3);
	sprite_ptr->set_visible(false);
	break;
    case SPINNER_STATE_LAUNCH:
	animate_action_ptr = bn::create_sprite_animate_action_forever(
	    sprite_ptr.value(),
	    4,
	    bn::sprite_items::spinner_sprite.tiles_item(),
	    0, 1, 2, 3);
	sprite_ptr->set_visible(true);
	break;
    case SPINNER_STATE_RETURN:
	animate_action_ptr = bn::create_sprite_animate_action_forever(
	    sprite_ptr.value(),
	    4,
	    bn::sprite_items::spinner_sprite.tiles_item(),
	    0, 1, 2, 3);
	sprite_ptr->set_visible(true);
	break;
    default:
	break;
    }
}

void Spinner::setDir(unsigned int new_x_dir, unsigned int new_y_dir)
{
    x_dir = new_x_dir;
    y_dir = new_y_dir;
}

void Spinner::setPos(unsigned int new_x, unsigned int new_y)
{
    sprite_ptr->set_x(new_x);
    sprite_ptr->set_y(new_y);
}

void Spinner::setPos(bn::fixed new_x, bn::fixed new_y)
{
    sprite_ptr->set_x(new_x);
    sprite_ptr->set_y(new_y);
}
