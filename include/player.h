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
		    	uint8 game_objects_size) override;
	Force* getForceWalkLeft();
	Force* getForceWalkRight();
	Force* getForceWalkUp();
	Force* getForceWalkDown();
	Force* getForceJump();
	Force* getForceWallJumpRight();
	Force* getForceWallJumpLeft();
	Force* getForceGravity();
	Force* getForceWallGravity();
};

#endif
