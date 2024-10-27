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

#define PLAYER_WALK_LEFT_FORCE  	 new Force(bn::fixed_point_t<12>(-walk_speed, 0), 1)
#define PLAYER_WALK_RIGHT_FORCE 	 new Force(bn::fixed_point_t<12>(walk_speed, 0), 1)
#define PLAYER_JUMP_FORCE            new Force(bn::fixed_point_t<12>(0, jump_force), 0.1)
#define PLAYER_WALL_JUMP_RIGHT_FORCE new Force(bn::fixed_point_t<12>(wall_jump_force.x(), wall_jump_force.y()), 0.1)
#define PLAYER_WALL_JUMP_LEFT_FORCE  new Force(bn::fixed_point_t<12>(-wall_jump_force.x(), wall_jump_force.y()), 0.1)
#define PLAYER_GRAVITY_FORCE         new Force(bn::fixed_point_t<12>(0, gravity), 1)
#define PLAYER_WALL_GRAVITY_FORCE    new Force(bn::fixed_point_t<12>(0, wall_ride_gravity), 1)

enum PlayerState {
	STATE_GROUNDED_NEUTRAL,
	STATE_AIR_NEUTRAL,
	STATE_WALL_SLIDE_RIGHT,
	STATE_WALL_SLIDE_LEFT
};

struct Player : GameObject {
	RigidBody*  rigidbody_p = NULL;
	PlayerState state;
	bn::fixed       walk_speed;
	bn::fixed       jump_force;
	bn::fixed_point wall_jump_force;
	bn::fixed       gravity;
	bn::fixed       wall_ride_gravity;
	
	Player();
	~Player() override;
	void update(GameObject** game_objects_p,
		    	uint32 game_objects_size) override;
};

#endif
