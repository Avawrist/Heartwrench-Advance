#ifndef PLAYER_H
#define PLAYER_H

// Butano
#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_optional.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"
#include "bn_camera_ptr.h"
#include "bn_fixed_fwd.h"
#include "bn_fixed_point_fwd.h"

// Assets
#include "bn_sprite_items_player_sprite.h"

// Game Files
#include "spinner.h"
#include "collider.h"

// Enum Player State //
enum PlayerState {
	PLAYER_STATE_IDLE_UP,
	PLAYER_STATE_IDLE_DOWN,
	PLAYER_STATE_IDLE_LEFT,
	PLAYER_STATE_IDLE_RIGHT,
	PLAYER_STATE_WALK_UP,
	PLAYER_STATE_WALK_DOWN,
	PLAYER_STATE_WALK_LEFT,
	PLAYER_STATE_WALK_RIGHT,
	PLAYER_STATE_TWIRL_UP,
	PLAYER_STATE_TWIRL_DOWN,
	PLAYER_STATE_TWIRL_LEFT,
	PLAYER_STATE_TWIRL_RIGHT
};

// Struct Player //
#define MAX_PLAYER_ANIM_FRAMES 16
#define TWIRL_TIMER_FRAMES 30

struct Player {
	unsigned int walk_speed  = 1;
	unsigned int twirl_timer = TWIRL_TIMER_FRAMES; 
	bn::optional<bn::sprite_ptr> sprite_ptr;
	bn::optional<bn::sprite_animate_action<MAX_PLAYER_ANIM_FRAMES>> animate_action_ptr;
        PlayerState state;
	Collider collider;

	Spinner spinner;
	
	Player();
	bn::fixed x() const;
	bn::fixed y() const;
	bn::fixed_point position() const;
	void update(const Collider& other_collider);
	void draw();
	void setCamera(const bn::camera_ptr& camera);
	void setState(PlayerState new_state);
	void setX(bn::fixed new_x);
	void setY(bn::fixed new_y);
	void setPos(bn::fixed new_x, bn::fixed new_y);
	void setPos(bn::fixed_point new_pos);
};

#endif
