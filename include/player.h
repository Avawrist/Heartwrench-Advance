#ifndef PLAYER_H
#define PLAYER_H

// Butano
#include "bn_optional.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"
#include "bn_keypad.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_math.h"

// My Libs
#include "utility.h"
#include "physics.h"
#include "collider.h"

// Assets
#include "bn_sprite_items_player.h"

// Base Class
#include "game_object.h"

///////////////////
// Struct Player //
///////////////////

#define PLAYER_COLLIDER_WIDTH 16
#define PLAYER_COLLIDER_HEIGHT 16

#define PLAYER_MAX_STRETCH_V 1.75
#define PLAYER_MAX_STRETCH_H 1.5

#define PLAYER_MIN_X_SPEED 0
#define PLAYER_MAX_X_SPEED 2
#define PLAYER_X_DECAY     1
#define X_SPEED_ACC_RATE   0.2
#define X_SPEED_DECAY_RATE 0.1

#define PLAYER_BASE_JUMP_FORCE       -7
#define PLAYER_SECOND_JUMP_FORCE     -3
#define PLAYER_WALL_JUMP_X_FORCE      12
#define PLAYER_WALL_JUMP_Y_FORCE     -12
#define PLAYER_JUMP_DECAY             0.1
#define PLAYER_SECONDARY_JUMP_DECAY   0.4
#define PLAYER_X_DRIFT_LOCKOUT_FRAMES 8
#define PLAYER_MAX_JUMP_INPUT_FRAMES  15
#define PLAYER_WALL_JUMP_DECAY        0.1

#define PLAYER_GRAVITY       	 3
#define PLAYER_WALL_RIDE_GRAVITY 1
#define PLAYER_GRAVITY_DECAY 	 1

#define PLAYER_DASH_FORCE      10
#define PLAYER_DASH_DECAY_RATE 0.05

#define PLAYER_SQUISH_FRAMES_REQUIRED 3

#define PLAYER_X_LEFT_FORCE  	      new Force(bn::fixed_point_t<12>(-x_speed, 0), PLAYER_X_DECAY)
#define PLAYER_X_RIGHT_FORCE 	      new Force(bn::fixed_point_t<12>( x_speed, 0), PLAYER_X_DECAY)
#define PLAYER_X_LEFT_DECAY_FORCE     new Force(bn::fixed_point_t<12>(-x_speed, 0), X_SPEED_DECAY_RATE)
#define PLAYER_X_RIGHT_DECAY_FORCE    new Force(bn::fixed_point_t<12> (x_speed, 0), X_SPEED_DECAY_RATE)

#define PLAYER_JUMP_FORCE             new Force(bn::fixed_point_t<12>(0, jump_force), PLAYER_JUMP_DECAY)
#define PLAYER_SECONDARY_JUMP_FORCE   new Force(bn::fixed_point_t<12>(0, secondary_jump_force), PLAYER_SECONDARY_JUMP_DECAY)
#define PLAYER_WALL_JUMP_RIGHT_FORCE  new Force(bn::fixed_point_t<12>( wall_jump_force.x(), wall_jump_force.y()), PLAYER_WALL_JUMP_DECAY)
#define PLAYER_WALL_JUMP_LEFT_FORCE   new Force(bn::fixed_point_t<12>(-wall_jump_force.x(), wall_jump_force.y()), PLAYER_WALL_JUMP_DECAY)

#define PLAYER_GRAVITY_FORCE          new Force(bn::fixed_point_t<12>(0, gravity), 			 PLAYER_GRAVITY_DECAY)
#define PLAYER_WALL_GRAVITY_FORCE     new Force(bn::fixed_point_t<12>(0, wall_ride_gravity), PLAYER_GRAVITY_DECAY)

#define PLAYER_DASH_UP_FORCE          new Force(bn::fixed_point_t<12>(0, -dash_force), PLAYER_DASH_DECAY_RATE)
#define PLAYER_DASH_DOWN_FORCE		  new Force(bn::fixed_point_t<12>(0,  dash_force), PLAYER_DASH_DECAY_RATE)
#define PLAYER_DASH_LEFT_FORCE		  new Force(bn::fixed_point_t<12>(-dash_force, 0), PLAYER_DASH_DECAY_RATE)
#define PLAYER_DASH_RIGHT_FORCE		  new Force(bn::fixed_point_t<12>(dash_force,  0), PLAYER_DASH_DECAY_RATE)

enum PlayerState {
	STATE_GROUNDED_NEUTRAL,
	STATE_AIR_NEUTRAL,
	STATE_WALL_SLIDE_RIGHT,
	STATE_WALL_SLIDE_LEFT,
	STATE_DEAD,
};

struct Player : GameObject {
	
	bn::optional<bn::sprite_ptr> sprite_ptr;
	bn::optional<bn::sprite_animate_action<MAX_ANIM_FRAMES>> animate_action_ptr;

	RigidBody*  	rigidbody_ptr = NULL;

	PlayerState 	state;
	bn::fixed       x_speed;
	bn::fixed       jump_force;
	bn::fixed       secondary_jump_force;
	bn::fixed 		dash_force;
	bn::fixed_point wall_jump_force;
	bn::fixed       gravity;
	bn::fixed       wall_ride_gravity;
	int32           remaining_jump_input_frames;
	int32           remaining_x_drift_lockout_frames;
	int32           air_frames_elapsed;
	
	Player();
	~Player() override;

	void update(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects) override;
	void draw() override;
	void setCamera(const bn::camera_ptr& camera) override;
	
	bn::fixed x() const override;
	bn::fixed y() const override;
	bn::fixed_point pos() const override;
	void setX(bn::fixed new_x) override;
	void setY(bn::fixed new_y) override;
	void setPos(bn::fixed new_x, bn::fixed new_y) override;
	void setPos(bn::fixed_point new_pos) override;
};

#endif
