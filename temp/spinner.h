#ifndef SPINNER_H
#define SPINNER_H

// Butano
#include "bn_core.h"
#include "bn_optional.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"

// Assets
#include "bn_sprite_items_spinner_sprite.h"

// Enum Spinner State // 
enum SpinnerState {
	SPINNER_STATE_IDLE,
	SPINNER_STATE_LAUNCH,
	SPINNER_STATE_RETURN
};

// Struct Spinner //
#define MAX_SPINNER_ANIM_FRAMES 4
#define MAX_SPINNER_LAUNCH_FRAMES 100
#define MAX_SPINNER_RETURN_FRAMES 100

struct Spinner {
	unsigned int move_speed = 2;
	unsigned int x_dir = 0;
	unsigned int y_dir = 0;
	unsigned int launch_timer = MAX_SPINNER_LAUNCH_FRAMES;
	unsigned int return_timer = MAX_SPINNER_RETURN_FRAMES;
	bn::optional<bn::sprite_ptr> sprite_ptr;
	bn::optional<bn::sprite_animate_action<MAX_SPINNER_ANIM_FRAMES>> animate_action_ptr;
	SpinnerState state;

	Spinner();
	void update(bn::fixed player_x, bn::fixed player_y);
	void draw();
	void setState(SpinnerState new_state);
	void setDir(unsigned int new_x_dir, unsigned int new_y_dir);
	void setPos(unsigned int new_x, unsigned int new_y);
	void setPos(bn::fixed new_x, bn::fixed new_y);
};

#endif
