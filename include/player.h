#ifndef PLAYER_H
#define PLAYER_H

// Butano
#include "bn_keypad.h"
#include "bn_math.h"

// Assets
#include "bn_sprite_items_player.h"

// Base Class
#include "game_object.h"

// My Libs
#include "missile_platform.h"

///////////////////
// Struct Player //
///////////////////

#define PLAYER_COLLIDER_WIDTH 16
#define PLAYER_COLLIDER_HEIGHT 16

#define PLAYER_MIN_STRETCH_V 0.75
#define PLAYER_MAX_STRETCH_V 1.75
#define PLAYER_MIN_STRETCH_H 0.75
#define PLAYER_MAX_STRETCH_H 1.5

#define PLAYER_MIN_X_SPEED 0
#define PLAYER_MAX_X_SPEED 2
#define PLAYER_X_DECAY     1
#define X_SPEED_ACC_RATE   0.2
#define X_SPEED_DECAY_RATE 0.1

#define PLAYER_BASE_JUMP_FORCE       -7
#define PLAYER_SECOND_JUMP_FORCE     -3
#define PLAYER_WALL_JUMP_X_FORCE      7
#define PLAYER_WALL_JUMP_Y_FORCE     -11
#define PLAYER_JUMP_DECAY             0.1
#define PLAYER_SECONDARY_JUMP_DECAY   0.4
#define PLAYER_X_DRIFT_LOCKOUT_FRAMES 8
#define PLAYER_MAX_JUMP_INPUT_FRAMES  15
#define PLAYER_WALL_JUMP_DECAY        0.1

#define PLAYER_GRAVITY       	 3
#define PLAYER_WALL_RIDE_GRAVITY 1
#define PLAYER_GRAVITY_DECAY 	 1

#define PLAYER_THROW_X_FORCE 5
#define PLAYER_THROW_Y_FORCE -5
#define PLAYER_THROW_FORCE_DECAY 0.05

#define PLAYER_SQUISH_FRAMES_REQUIRED 3

#define PLAYER_MISSILE_THROW_FRAMES 6
#define PLAYER_THROW_MISSILE_FRAME  5
#define PLAYER_THROW_COOLDOWN_FRAMES 40

#define PLAYER_LEAP_X_FORCE 10 
#define PLAYER_LEAP_Y_FORCE -8
#define PLAYER_LEAP_FORCE_DECAY 0.05
#define PLAYER_MAX_LEAP_CANCEL_FRAMES 25

#define PLAYER_OWP_SNAP_FRAMES 2

#define PLAYER_MAX_AMMO 1

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

#define PLAYER_THROW_FORCE            new Force(bn::fixed_point_t<12>(PLAYER_THROW_X_FORCE * dir * -1, PLAYER_THROW_Y_FORCE), PLAYER_THROW_FORCE_DECAY)
#define PLAYER_LEAP_FORCE             new Force(bn::fixed_point_t<12>(PLAYER_LEAP_X_FORCE * dir, PLAYER_LEAP_Y_FORCE), PLAYER_LEAP_FORCE_DECAY)


enum PlayerState {
	STATE_GROUNDED_NEUTRAL,
	STATE_AIR_NEUTRAL,
	STATE_WALL_SLIDE_RIGHT,
	STATE_WALL_SLIDE_LEFT,
	STATE_THROWING,
	STATE_DEAD,
};

struct Player : GameObject {
	
	PlayerState 	state;
	bn::fixed       x_speed;
	bn::fixed       jump_force;
	bn::fixed       secondary_jump_force;
	//bn::fixed 		dash_force;
	bn::fixed_point wall_jump_force;
	bn::fixed       gravity;
	bn::fixed       wall_ride_gravity;
	int32           remaining_jump_input_frames;
	int32           remaining_x_drift_lockout_frames;
	int32           current_missile_throw_frames;
	int32           missile_throw_cooldown_frames;
	int32           owp_grace_frames;
	int32           air_frames_elapsed;
	int32           remaining_leap_cancel_frames;
	int32           ammo_count;

	bn::point       respawn_pos;
	
	Player();
	~Player();

	void update(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects,
				bn::regular_bg_ptr                         bg_ptr, 
                bn::span<const bn::regular_bg_map_cell>    cells,
                bn::regular_bg_item                        bg_item,
				bn::camera_ptr                             camera) override;
	
};

#endif
