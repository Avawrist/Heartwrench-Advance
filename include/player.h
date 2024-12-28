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
#include "scythe_platform.h"

///////////////////
// Struct Player //
///////////////////

#define PLAYER_COLLIDER_WIDTH 16
#define PLAYER_COLLIDER_HEIGHT 16

#define PLAYER_MIN_STRETCH_V 0.7
#define PLAYER_MAX_STRETCH_V 1.8
#define PLAYER_FALL_STRETCH_V 1.5
#define PLAYER_MIN_STRETCH_H 0.7
#define PLAYER_MAX_STRETCH_H 1.8
#define PLAYER_FALL_STRETCH_H 0.75

#define PLAYER_MIN_X_SPEED     0
#define PLAYER_MAX_X_SPEED     2
#define PLAYER_PLUMMET_X_SPEED 1
#define PLAYER_X_DECAY         1
#define X_SPEED_ACC_RATE       0.2
#define X_SPEED_DECAY_RATE     0.1

#define PLAYER_BASE_JUMP_FORCE       -7
#define PLAYER_SECOND_JUMP_FORCE     -3
#define PLAYER_WALL_JUMP_X_FORCE      10
#define PLAYER_WALL_JUMP_Y_FORCE     -10
#define PLAYER_JUMP_DECAY             0.1
#define PLAYER_SECONDARY_JUMP_DECAY   0.4
#define PLAYER_X_DRIFT_LOCKOUT_FRAMES 12
#define PLAYER_MAX_JUMP_INPUT_FRAMES  15
#define PLAYER_WALL_JUMP_DECAY        0.1

#define PLAYER_GRAVITY       	 3
#define PLAYER_FAST_FALL_GRAVITY 2
#define PLAYER_WALL_RIDE_GRAVITY 1
#define PLAYER_SCYTHE_GRAVITY    1
#define PLAYER_GRAVITY_DECAY 	 1

#define PLAYER_SQUISH_FRAMES_REQUIRED 3

#define PLAYER_PHASE_STEP_TOTAL_FRAMES 25
#define PLAYER_PHASE_FRAME             15

#define PLAYER_SCYTHE_THROW_FRAMES 6
#define PLAYER_THROW_SCYTHE_FRAME  5
#define PLAYER_THROW_COOLDOWN_FRAMES 40

#define PLAYER_SCYTHE_1_TOTAL_FRAMES 30
#define PLAYER_SCYTHE_2_TOTAL_FRAMES 30
#define PLAYER_SCYTHE_3_TOTAL_FRAMES 40
#define PLAYER_MIN_SCYTHE_2_BUFFER_FRAMES 10
#define PLAYER_MIN_SCYTHE_3_BUFFER_FRAMES 10

#define PLAYER_V_COLLISION_MAX_GRACE_FRAMES 4
#define PLAYER_LATE_JUMP_GRACE_FRAMES       8

#define PLAYER_SCYTHE_MAX_CHARGE_FRAMES 35
 
#define PLAYER_DEATH_FRAMES       20
#define PLAYER_DEATH_X_FORCE      5
#define PLAYER_DEATH_Y_FORCE      5
#define PLAYER_DEATH_DECAY        0.2

#define PLAYER_X_LEFT_FORCE  	     new Force(bn::fixed_point_t<12>(-x_speed, 0), PLAYER_X_DECAY)
#define PLAYER_X_RIGHT_FORCE 	     new Force(bn::fixed_point_t<12>( x_speed, 0), PLAYER_X_DECAY)
#define PLAYER_X_PLUMMET_LEFT_FORCE  new Force(bn::fixed_point_t<12>(-PLAYER_PLUMMET_X_SPEED, 0), PLAYER_X_DECAY)
#define PLAYER_X_PLUMMET_RIGHT_FORCE new Force(bn::fixed_point_t<12>( PLAYER_PLUMMET_X_SPEED, 0), PLAYER_X_DECAY)
#define PLAYER_X_LEFT_DECAY_FORCE    new Force(bn::fixed_point_t<12>(-x_speed, 0), X_SPEED_DECAY_RATE)
#define PLAYER_X_RIGHT_DECAY_FORCE   new Force(bn::fixed_point_t<12> (x_speed, 0), X_SPEED_DECAY_RATE)

#define PLAYER_JUMP_FORCE            new Force(bn::fixed_point_t<12>(0, jump_force), PLAYER_JUMP_DECAY)
#define PLAYER_SECONDARY_JUMP_FORCE  new Force(bn::fixed_point_t<12>(0, secondary_jump_force), PLAYER_SECONDARY_JUMP_DECAY)
#define PLAYER_WALL_JUMP_RIGHT_FORCE new Force(bn::fixed_point_t<12>( wall_jump_force.x(), wall_jump_force.y()), PLAYER_WALL_JUMP_DECAY)
#define PLAYER_WALL_JUMP_LEFT_FORCE  new Force(bn::fixed_point_t<12>(-wall_jump_force.x(), wall_jump_force.y()), PLAYER_WALL_JUMP_DECAY)

#define PLAYER_GRAVITY_FORCE         new Force(bn::fixed_point_t<12>(0, gravity), 			       PLAYER_GRAVITY_DECAY)
#define PLAYER_FAST_GRAVITY_FORCE    new Force(bn::fixed_point_t<12>(0, PLAYER_FAST_FALL_GRAVITY), PLAYER_GRAVITY_DECAY)
#define PLAYER_WALL_GRAVITY_FORCE    new Force(bn::fixed_point_t<12>(0, wall_ride_gravity),        PLAYER_GRAVITY_DECAY)
#define PLAYER_SCYTHE_GRAVITY_FORCE  new Force(bn::fixed_point_t<12>(0, PLAYER_SCYTHE_GRAVITY),    PLAYER_GRAVITY_DECAY)

enum PlayerState {

	STATE_GROUNDED_NEUTRAL,
	STATE_AIR_NEUTRAL,
	STATE_AIR_PLUMMET,
	STATE_WALL_SLIDE_RIGHT,
	STATE_WALL_SLIDE_LEFT,
	STATE_PHASE_STEP,
	STATE_THROWING,
	STATE_SCYTHE_1,
	STATE_SCYTHE_2,
	STATE_SCYTHE_3,
	STATE_DYING,

};

struct Player : GameObject {
	
	PlayerState 	state;

	bn::fixed       x_speed;
	bn::fixed       jump_force;
	bn::fixed       secondary_jump_force;
	bn::fixed_point wall_jump_force;
	bn::fixed       gravity;
	bn::fixed       wall_ride_gravity;

	int32 remaining_jump_input_frames;
	int32 remaining_x_drift_lockout_frames;
	int32 current_scythe_throw_frames;
	int32 scythe_throw_cooldown_frames;
	int32 air_frames_elapsed;
	int32 v_collision_grace_frames;
	int32 late_jump_grace_frames;
	int32 scythe_charge_frames;
	int32 current_death_frame;
	int32 current_scythe_frame;
	int32 current_phase_step_frame;

	bool wall_right_detected;
    bool wall_left_detected;
    bool grounded_detected;
	bool grounded_owp_detected;
	bool throw_scythe;
	bool scythe_2_buffered;
	bool scythe_3_buffered;
	bool kill_player;
	bool is_dead;

	bn::point       respawn_pos;
	
	Player();
	~Player();

	void update(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects,
				bn::regular_bg_ptr                         bg_ptr, 
                bn::span<const bn::regular_bg_map_cell>    cells,
                bn::regular_bg_item                        bg_item,
				bn::camera_ptr                             camera) override;
	void jump();
	void fastFall();
	void throwScythe();
	void setState(PlayerState new_state);
	
};

#endif
