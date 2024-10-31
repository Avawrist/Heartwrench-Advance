#ifndef PLAYER_H
#define PLAYER_H

// Butano
#include "bn_optional.h"
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

#define PLAYER_MAX_STRETCH_V 1.75
#define PLAYER_MAX_STRETCH_H 1.5

#define MIN_X_SPEED 0
#define MAX_X_SPEED 2

#define X_SPEED_ACC_RATE 0.2
#define X_SPEED_DECAY_RATE 0.1
#define PLAYER_X_LEFT_FORCE  	      new Force(bn::fixed_point_t<12>(-x_speed, 0), 1)
#define PLAYER_X_RIGHT_FORCE 	      new Force(bn::fixed_point_t<12>(x_speed, 0),  1)
#define PLAYER_X_LEFT_DECAY_FORCE     new Force(bn::fixed_point_t<12>(-x_speed, 0), X_SPEED_DECAY_RATE)
#define PLAYER_X_RIGHT_DECAY_FORCE    new Force(bn::fixed_point_t<12>(x_speed, 0),  X_SPEED_DECAY_RATE)

#define PLAYER_JUMP_FORCE             new Force(bn::fixed_point_t<12>(0, jump_force), 0.1)
#define PLAYER_WALL_JUMP_RIGHT_FORCE  new Force(bn::fixed_point_t<12>(wall_jump_force.x(), wall_jump_force.y()), 0.1)
#define PLAYER_WALL_JUMP_LEFT_FORCE   new Force(bn::fixed_point_t<12>(-wall_jump_force.x(), wall_jump_force.y()), 0.1)

#define PLAYER_GRAVITY_FORCE          new Force(bn::fixed_point_t<12>(0, gravity), 1)
#define PLAYER_WALL_GRAVITY_FORCE     new Force(bn::fixed_point_t<12>(0, wall_ride_gravity), 1)

#define PLAYER_DASH_DECAY_RATE        0.05
#define PLAYER_DASH_UP_FORCE          new Force(bn::fixed_point_t<12>(0, -dash_force), PLAYER_DASH_DECAY_RATE)
#define PLAYER_DASH_DOWN_FORCE		  new Force(bn::fixed_point_t<12>(0,  dash_force), PLAYER_DASH_DECAY_RATE)
#define PLAYER_DASH_LEFT_FORCE		  new Force(bn::fixed_point_t<12>(-dash_force, 0), PLAYER_DASH_DECAY_RATE)
#define PLAYER_DASH_RIGHT_FORCE		  new Force(bn::fixed_point_t<12>(dash_force,  0), PLAYER_DASH_DECAY_RATE)

enum PlayerState {
	STATE_GROUNDED_NEUTRAL,
	STATE_AIR_NEUTRAL,
	STATE_WALL_SLIDE_RIGHT,
	STATE_WALL_SLIDE_LEFT
};

struct Player : GameObject {
	RigidBody*  	rigidbody_p = NULL;
	PlayerState 	state;
	bn::fixed       x_speed;
	bn::fixed       jump_force;
	bn::fixed 		dash_force;
	bn::fixed_point wall_jump_force;
	bn::fixed       gravity;
	bn::fixed       wall_ride_gravity;
	
	Player();
	~Player() override;
	void update(GameObject** game_objects_p,
		    	uint32 game_objects_size) override;
};

#endif
