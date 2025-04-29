#ifndef PLAYER_H
#define PLAYER_H

// Butano
#include "bn_keypad.h"
#include "bn_math.h"

// Assets
#include "bn_sprite_items_player.h"
#include "bn_sprite_items_phase_marker.h"

// Base Class
#include "game_object.h"

// My Libs
#include "hitbox.h"
#include "tile_passage.h"
#include "phase_orb.h"
#include "ground_ghoul.h"
#include "ceiling_ghoul.h"

///////////////////
// Struct Player //
///////////////////

#define PLAYER_Z_ORDER 0

#define PLAYER_COLLIDER_WIDTH  14
#define PLAYER_COLLIDER_HEIGHT 28

#define PLAYER_COLLIDER_OFFSET_X 0
#define PLAYER_COLLIDER_OFFSET_Y 2

#define PLAYER_MIN_STRETCH_V  0.7
#define PLAYER_MAX_STRETCH_V  1.8
#define PLAYER_FALL_STRETCH_V 1.5
#define PLAYER_MIN_STRETCH_H  0.7
#define PLAYER_MAX_STRETCH_H  1.8
#define PLAYER_FALL_STRETCH_H 0.75

#define PLAYER_MIN_X_SPEED     0
#define PLAYER_MAX_X_SPEED     2
#define PLAYER_X_DECAY         1
#define X_SPEED_ACC_RATE       0.2
#define X_SPEED_DECAY_RATE     0.1

#define PLAYER_BASE_JUMP_FORCE       -6
#define PLAYER_SECOND_JUMP_FORCE     -4
#define PLAYER_WALL_JUMP_X_FORCE      4
#define PLAYER_WALL_JUMP_Y_FORCE     -6
#define PLAYER_JUMP_DECAY             0.1
#define PLAYER_SECONDARY_JUMP_DECAY   1
#define PLAYER_X_DRIFT_LOCKOUT_FRAMES 6
#define PLAYER_MAX_JUMP_INPUT_FRAMES  24
#define PLAYER_WALL_JUMP_DECAY        0.05

#define PLAYER_GRAVITY       	             3
#define PLAYER_PROLONGED_GRAVITY             1
#define PLAYER_PROLONGED_AIR_FRAMES_REQUIRED 15
#define PLAYER_FAST_FALL_GRAVITY             1
#define PLAYER_MIN_FAST_FALL_FRAMES          3
#define PLAYER_WALL_RIDE_GRAVITY             1
#define PLAYER_SCYTHE_GRAVITY                1
#define PLAYER_GRAVITY_DECAY 	             1

#define PLAYER_SQUISH_SPEED_REQUIRED  PLAYER_GRAVITY + PLAYER_PROLONGED_GRAVITY
#define PLAYER_SQUISH_FRAMES_REQUIRED 4
#define PLAYER_MAX_AIR_FRAMES         180

#define PLAYER_PHASE_STEP_MAX_DISTANCE PLAYER_COLLIDER_WIDTH * 6
#define PLAYER_PHASE_STEP_SPEED        4
#define PLAYER_PHASE_STEP_EXIT_X_FORCE 6
#define PLAYER_PHASE_STEP_EXIT_Y_FORCE 8
#define PLAYER_PHASE_STEP_EXIT_DECAY   0.05
#define PLAYER_PHASE_JUMP_LOCKOUT_FRAMES 5

#define PLAYER_SCYTHE_GROUND_1_TOTAL_FRAMES      20
#define PLAYER_SCYTHE_GROUND_2_TOTAL_FRAMES      20
#define PLAYER_SCYTHE_GROUND_3_TOTAL_FRAMES      20
#define PLAYER_SCYTHE_AIR_1_TOTAL_FRAMES         20
#define PLAYER_MIN_SCYTHE_GROUND_1_BUFFER_FRAMES 11 // Must be greater than PLAYER_SCYTHE_1_CREATE_HB_FRAME
#define PLAYER_MIN_SCYTHE_GROUND_2_BUFFER_FRAMES 11 // Must be greater than PLAYER_SCYTHE_2_CREATE_HB_FRAME
#define PLAYER_MIN_SCYTHE_GROUND_3_BUFFER_FRAMES 11 // Must be greater than PLAYER_SCYTHE_2_CREATE_HB_FRAME

#define PLAYER_SCYTHE_GROUND_1_X_OFFSET           32
#define PLAYER_SCYTHE_GROUND_1_Y_OFFSET           0
#define PLAYER_SCYTHE_GROUND_1_HB_WIDTH           16
#define PLAYER_SCYTHE_GROUND_1_HB_HEIGHT          16
#define PLAYER_SCYTHE_GROUND_1_CREATE_HB_FRAME    3
#define PLAYER_SCYTHE_GROUND_1_HB_LIFESPAN_FRAMES 10
#define PLAYER_SCYTHE_GROUND_1_HITSTUN_FRAMES     10
#define PLAYER_SCYTHE_GROUND_1_X_KNOCKBACK        5
#define PLAYER_SCYTHE_GROUND_1_Y_KNOCKBACK        0
#define PLAYER_SCYTHE_GROUND_1_KNOCKBACK_DECAY    0.2

#define PLAYER_SCYTHE_GROUND_2_X_OFFSET           32
#define PLAYER_SCYTHE_GROUND_2_Y_OFFSET           0
#define PLAYER_SCYTHE_GROUND_2_HB_WIDTH           16
#define PLAYER_SCYTHE_GROUND_2_HB_HEIGHT          16
#define PLAYER_SCYTHE_GROUND_2_CREATE_HB_FRAME    10
#define PLAYER_SCYTHE_GROUND_2_HB_LIFESPAN_FRAMES 10
#define PLAYER_SCYTHE_GROUND_2_HITSTUN_FRAMES     10
#define PLAYER_SCYTHE_GROUND_2_X_KNOCKBACK        5
#define PLAYER_SCYTHE_GROUND_2_Y_KNOCKBACK        0
#define PLAYER_SCYTHE_GROUND_2_KNOCKBACK_DECAY    0.2

#define PLAYER_SCYTHE_GROUND_3_X_OFFSET           32
#define PLAYER_SCYTHE_GROUND_3_Y_OFFSET           0
#define PLAYER_SCYTHE_GROUND_3_HB_WIDTH           16
#define PLAYER_SCYTHE_GROUND_3_HB_HEIGHT          16
#define PLAYER_SCYTHE_GROUND_3_CREATE_HB_FRAME    10
#define PLAYER_SCYTHE_GROUND_3_HB_LIFESPAN_FRAMES 10
#define PLAYER_SCYTHE_GROUND_3_HITSTUN_FRAMES     10
#define PLAYER_SCYTHE_GROUND_3_X_KNOCKBACK        5
#define PLAYER_SCYTHE_GROUND_3_Y_KNOCKBACK        0
#define PLAYER_SCYTHE_GROUND_3_KNOCKBACK_DECAY    0.2

#define PLAYER_SCYTHE_AIR_1_X_OFFSET           32
#define PLAYER_SCYTHE_AIR_1_Y_OFFSET           0
#define PLAYER_SCYTHE_AIR_1_HB_WIDTH           16
#define PLAYER_SCYTHE_AIR_1_HB_HEIGHT          16
#define PLAYER_SCYTHE_AIR_1_CREATE_HB_FRAME    3
#define PLAYER_SCYTHE_AIR_1_HB_LIFESPAN_FRAMES 10
#define PLAYER_SCYTHE_AIR_1_HITSTUN_FRAMES     10
#define PLAYER_SCYTHE_AIR_1_X_KNOCKBACK        5
#define PLAYER_SCYTHE_AIR_1_Y_KNOCKBACK        0
#define PLAYER_SCYTHE_AIR_1_KNOCKBACK_DECAY    0.2

#define PLAYER_V_COLLISION_MAX_GRACE_FRAMES 4
#define PLAYER_LATE_JUMP_GRACE_FRAMES       6
 
#define PLAYER_DEATH_FRAMES       20
#define PLAYER_DEATH_X_FORCE      5
#define PLAYER_DEATH_Y_FORCE      5
#define PLAYER_DEATH_DECAY        0.2

#define PLAYER_MIN_PASSAGE_SPEED PLAYER_GRAVITY + PLAYER_PROLONGED_GRAVITY + PLAYER_FAST_FALL_GRAVITY

#define PLAYER_X_LEFT_FORCE  	     Force(bn::fixed_point_t<12>(-x_speed, 0), PLAYER_X_DECAY)
#define PLAYER_X_RIGHT_FORCE 	     Force(bn::fixed_point_t<12>( x_speed, 0), PLAYER_X_DECAY)
#define PLAYER_X_LEFT_DECAY_FORCE    Force(bn::fixed_point_t<12>(-x_speed, 0), X_SPEED_DECAY_RATE)
#define PLAYER_X_RIGHT_DECAY_FORCE   Force(bn::fixed_point_t<12> (x_speed, 0), X_SPEED_DECAY_RATE)

#define PLAYER_JUMP_FORCE              Force(bn::fixed_point_t<12>(0, jump_force), PLAYER_JUMP_DECAY)
#define PLAYER_SECONDARY_JUMP_FORCE    Force(bn::fixed_point_t<12>(0, secondary_jump_force), PLAYER_SECONDARY_JUMP_DECAY)
#define PLAYER_WALL_JUMP_RIGHT_FORCE   Force(bn::fixed_point_t<12>( wall_jump_force.x(), wall_jump_force.y()), PLAYER_WALL_JUMP_DECAY)
#define PLAYER_WALL_JUMP_LEFT_FORCE    Force(bn::fixed_point_t<12>(-wall_jump_force.x(), wall_jump_force.y()), PLAYER_WALL_JUMP_DECAY)

#define PLAYER_GRAVITY_FORCE           Force(bn::fixed_point_t<12>(0, gravity), 			     PLAYER_GRAVITY_DECAY)
#define PLAYER_PROLONGED_GRAVITY_FORCE Force(bn::fixed_point_t<12>(0, PLAYER_PROLONGED_GRAVITY), PLAYER_GRAVITY_DECAY)
#define PLAYER_FAST_GRAVITY_FORCE      Force(bn::fixed_point_t<12>(0, PLAYER_FAST_FALL_GRAVITY), PLAYER_GRAVITY_DECAY)
#define PLAYER_WALL_GRAVITY_FORCE      Force(bn::fixed_point_t<12>(0, wall_ride_gravity),        PLAYER_GRAVITY_DECAY)
#define PLAYER_SCYTHE_GRAVITY_FORCE    Force(bn::fixed_point_t<12>(0, PLAYER_SCYTHE_GRAVITY),    PLAYER_GRAVITY_DECAY)

#define PLAYER_PHASE_STEP_EXIT_FORCE_UP    Force(bn::fixed_point_t<12>(0, -PLAYER_PHASE_STEP_EXIT_Y_FORCE), PLAYER_PHASE_STEP_EXIT_DECAY)
#define PLAYER_PHASE_STEP_EXIT_FORCE_DOWN  Force(bn::fixed_point_t<12>(0,  PLAYER_PHASE_STEP_EXIT_Y_FORCE), PLAYER_PHASE_STEP_EXIT_DECAY)
#define PLAYER_PHASE_STEP_EXIT_FORCE_LEFT  Force(bn::fixed_point_t<12>(-PLAYER_PHASE_STEP_EXIT_X_FORCE, 0), PLAYER_PHASE_STEP_EXIT_DECAY)
#define PLAYER_PHASE_STEP_EXIT_FORCE_RIGHT Force(bn::fixed_point_t<12>(PLAYER_PHASE_STEP_EXIT_X_FORCE,  0), PLAYER_PHASE_STEP_EXIT_DECAY)

enum PlayerState {
	STATE_NO_STATE,
	STATE_GROUNDED_NEUTRAL,
	STATE_AIR_NEUTRAL,
	STATE_WALL_SLIDE_RIGHT,
	STATE_WALL_SLIDE_LEFT,
	STATE_PHASE_STEP,
	STATE_SCYTHE_GROUND_1,
	STATE_SCYTHE_GROUND_2,
	STATE_SCYTHE_GROUND_3,
	STATE_SCYTHE_AIR_1,
	STATE_DYING,
};

enum PhaseDir
{
	PHASE_UP = 0,
	PHASE_DOWN,
	PHASE_LEFT,
	PHASE_RIGHT
};

struct Player : GameObject {
	
	PlayerState 	state;

	bn::fixed       x_speed;
	bn::fixed       jump_force;
	bn::fixed       secondary_jump_force;
	bn::fixed_point wall_jump_force;
	bn::fixed       gravity;
	bn::fixed       wall_ride_gravity;
	bn::fixed_point phase_destination;

	int32 remaining_jump_input_frames;
	int32 remaining_x_drift_lockout_frames;
	int32 air_frames_elapsed;
	int32 v_collision_grace_frames;
	int32 late_jump_grace_frames;
	int32 current_death_frame;
	int32 current_scythe_frame;
	int32 current_phase_frame;
	
	bool wall_right_detected;
    bool wall_left_detected;
    bool grounded_detected;
	bool grounded_owp_detected;
	bool left_wj_eligible;
	bool right_wj_eligible;
	bool scythe_ground_2_buffered;
	bool scythe_ground_3_buffered;
	bool kill_player;

	Collider test_collider;
	Collider test_collider_right;
	Collider test_collider_left;

	Hitbox* hitbox_1_ptr;
	Hitbox* hitbox_2_ptr;
	Hitbox* hitbox_3_ptr;

	bn::optional<bn::sprite_ptr> pm_sprite_ptr;
	PhaseDir phase_dir;
	
	Player();
	Player(const Player& other);
	~Player();

	Player& operator =(const Player& other);

	void update(const RoomBounds& 								 room_bounds,
				bn::vector<GameObject*, MAX_GAME_OBJECTS>&       game_objects,
				const bn::regular_bg_ptr&                        bg_ptr, 
                const bn::span<const bn::regular_bg_map_cell>&   cells,
                const bn::regular_bg_item&                       bg_item,
				const bn::camera_ptr&                            camera) override;
	void setCamera(const bn::camera_ptr& camera) override;
	void jump();
	void fastFall();
	void setState(PlayerState new_state);
	void createGroundedScythe1Hitboxes(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects, 
		                               const bn::camera_ptr&                      camera);
	void createAirScythe1Hitboxes(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects, 
								  const bn::camera_ptr&                      camera);
	
};

#endif
