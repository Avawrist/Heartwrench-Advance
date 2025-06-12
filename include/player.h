#ifndef PLAYER_H
#define PLAYER_H

// Butano
#include "bn_keypad.h"
#include "bn_math.h"

// Base Class
#include "game_object.h"

// My Libs
#include "hitbox.h"
#include "phase_orb.h"

///////////////////
// Struct Player //
///////////////////

#define PLAYER_Z_ORDER 0

#define PLAYER_COLLIDER_WIDTH  14
#define PLAYER_COLLIDER_HEIGHT 27

#define PLAYER_COLLIDER_OFFSET_X 0
#define PLAYER_COLLIDER_OFFSET_Y 2

#define PLAYER_FALL_STRETCH_V 1.5
#define PLAYER_FALL_STRETCH_H 0.75

#define PLAYER_MAX_HITPOINTS 3

#define PLAYER_ATTACK_BUFFER_FRAMES 12
#define PLAYER_ROLL_BUFFER_FRAMES   12
#define PLAYER_JUMP_BUFFER_FRAMES   12

#define PLAYER_MIN_X_SPEED 0
#define PLAYER_MAX_X_SPEED 2
#define PLAYER_X_DECAY     1
#define X_SPEED_ACC_RATE   0.2
#define X_SPEED_DECAY_RATE 0.1

#define PLAYER_BASE_JUMP_FORCE            -8
#define PLAYER_SECOND_JUMP_Y_FORCE        -2
#define PLAYER_TERTIARY_JUMP_Y_FORCE      -1
#define PLAYER_WALL_JUMP_X_FORCE           4
#define PLAYER_WALL_JUMP_Y_FORCE          -6
#define PLAYER_JUMP_DECAY                  0.04
#define PLAYER_SECONDARY_JUMP_DECAY        1
#define PLAYER_TERTIARY_JUMP_DECAY         1
#define PLAYER_X_DRIFT_LOCKOUT_FRAMES      0
#define PLAYER_MAX_JUMP_INPUT_FRAMES       6
#define PLAYER_MAX_WALL_JUMP_INPUT_FRAMES  6
#define PLAYER_WALL_JUMP_DECAY             0.05

#define PLAYER_GRAVITY       	             3
#define PLAYER_PROLONGED_GRAVITY             1
#define PLAYER_PROLONGED_AIR_FRAMES_REQUIRED 16
#define PLAYER_FAST_FALL_GRAVITY             2
#define PLAYER_MIN_FAST_FALL_FRAMES          3
#define PLAYER_WALL_RIDE_GRAVITY             1
#define PLAYER_ATTACK_GRAVITY                1
#define PLAYER_GRAVITY_DECAY 	             1

#define PLAYER_SQUISH_SPEED_REQUIRED  PLAYER_GRAVITY
#define PLAYER_SQUISH_FRAMES_REQUIRED 4
#define PLAYER_MAX_AIR_FRAMES         100

#define PLAYER_PHASE_STEP_MAX_DISTANCE PLAYER_COLLIDER_WIDTH * 6
#define PLAYER_PHASE_STEP_SPEED        4
#define PLAYER_PHASE_STEP_EXIT_X_FORCE 6
#define PLAYER_PHASE_STEP_EXIT_Y_FORCE 8
#define PLAYER_PHASE_STEP_EXIT_DECAY   0.05
#define PLAYER_PHASE_JUMP_LOCKOUT_FRAMES 5

#define PLAYER_ATTACK_GROUND_1_X_OFFSET           24
#define PLAYER_ATTACK_GROUND_1_Y_OFFSET           0
#define PLAYER_ATTACK_GROUND_1_HB_WIDTH           32
#define PLAYER_ATTACK_GROUND_1_HB_HEIGHT          32
#define PLAYER_ATTACK_GROUND_1_CREATE_HB_FRAME    18
#define PLAYER_ATTACK_GROUND_1_HB_LIFESPAN_FRAMES 8
#define PLAYER_ATTACK_GROUND_1_X_KNOCKBACK        10
#define PLAYER_ATTACK_GROUND_1_Y_KNOCKBACK        0
#define PLAYER_ATTACK_GROUND_1_KNOCKBACK_DECAY    0.05
#define PLAYER_ATTACK_GROUND_1_DAMAGE             1
#define PLAYER_ATTACK_GROUND_1_HITSTOP_FRAMES     10
#define PLAYER_ATTACK_GROUND_1_HITSTUN_FRAMES     20
#define PLAYER_ATTACK_GROUND_1_SCREENSHAKE_FRAMES 10
#define PLAYER_ATTACK_GROUND_1_SCREENSHAKE_SEVERITY STRONG_SHAKE

#define PLAYER_ATTACK_AIR_1_X_OFFSET           24
#define PLAYER_ATTACK_AIR_1_Y_OFFSET           0
#define PLAYER_ATTACK_AIR_1_HB_WIDTH           32
#define PLAYER_ATTACK_AIR_1_HB_HEIGHT          32
#define PLAYER_ATTACK_AIR_1_CREATE_HB_FRAME    18
#define PLAYER_ATTACK_AIR_1_HB_LIFESPAN_FRAMES 8
#define PLAYER_ATTACK_AIR_1_X_KNOCKBACK        10
#define PLAYER_ATTACK_AIR_1_Y_KNOCKBACK        0
#define PLAYER_ATTACK_AIR_1_KNOCKBACK_DECAY    0.05
#define PLAYER_ATTACK_AIR_1_DAMAGE             1
#define PLAYER_ATTACK_AIR_1_HITSTOP_FRAMES     10
#define PLAYER_ATTACK_AIR_1_HITSTUN_FRAMES     20
#define PLAYER_ATTACK_AIR_1_SCREENSHAKE_FRAMES 10
#define PLAYER_ATTACK_AIR_1_SCREENSHAKE_SEVERITY STRONG_SHAKE

#define PLAYER_V_COLLISION_MAX_GRACE_FRAMES 4
#define PLAYER_LATE_JUMP_GRACE_FRAMES       6
#define PLAYER_LATE_ROLL_JUMP_GRACE_FRAMES  8

#define PLAYER_ROLL_X_FORCE 2
#define PLAYER_ROLL_DECAY   0.75
#define PLAYER_ROLL_X_SPEED 1

#define PLAYER_ROLL_JUMP_X_FORCE  4
#define PLAYER_ROLL_JUMP_Y_FORCE -8
#define PLAYER_ROLL_JUMP_DECAY   0.04

#define PLAYER_MIN_PASSAGE_SPEED PLAYER_GRAVITY + PLAYER_PROLONGED_GRAVITY + PLAYER_FAST_FALL_GRAVITY

#define PLAYER_NEUTRAL_FRAME 0
#define PLAYER_AIR_FRAME_1   19
#define PLAYER_AIR_FRAME_2   20
#define PLAYER_CROUCH_FRAME  44
#define PLAYER_UP_FRAME      45

#define PLAYER_X_LEFT_FORCE  	     Force(bn::fixed_point_t<12>(-x_speed, 0), PLAYER_X_DECAY)
#define PLAYER_X_RIGHT_FORCE 	     Force(bn::fixed_point_t<12>( x_speed, 0), PLAYER_X_DECAY)
#define PLAYER_X_LEFT_DECAY_FORCE    Force(bn::fixed_point_t<12>(-x_speed, 0), X_SPEED_DECAY_RATE)
#define PLAYER_X_RIGHT_DECAY_FORCE   Force(bn::fixed_point_t<12> (x_speed, 0), X_SPEED_DECAY_RATE)

#define PLAYER_JUMP_FORCE              Force(bn::fixed_point_t<12>(0, PLAYER_BASE_JUMP_FORCE), PLAYER_JUMP_DECAY)
#define PLAYER_SECONDARY_JUMP_FORCE    Force(bn::fixed_point_t<12>(0, PLAYER_SECOND_JUMP_Y_FORCE), PLAYER_SECONDARY_JUMP_DECAY)
#define PLAYER_TERTIARY_JUMP_FORCE     Force(bn::fixed_point_t<12>(0, PLAYER_TERTIARY_JUMP_Y_FORCE), PLAYER_TERTIARY_JUMP_DECAY)
#define PLAYER_WALL_JUMP_FORCE         Force(bn::fixed_point_t<12>(PLAYER_WALL_JUMP_X_FORCE * (int32)x_dir, PLAYER_WALL_JUMP_Y_FORCE), PLAYER_WALL_JUMP_DECAY)

#define PLAYER_ROLL_FORCE              Force(bn::fixed_point_t<12>(PLAYER_ROLL_X_FORCE * (int32)x_dir, 0), PLAYER_ROLL_DECAY)
#define PLAYER_ROLL_JUMP_FORCE         Force(bn::fixed_point_t<12>(PLAYER_ROLL_JUMP_X_FORCE * (int32)x_dir, PLAYER_ROLL_JUMP_Y_FORCE), PLAYER_ROLL_JUMP_DECAY)

#define PLAYER_GRAVITY_FORCE           Force(bn::fixed_point_t<12>(0, PLAYER_GRAVITY), 			 PLAYER_GRAVITY_DECAY)
#define PLAYER_PROLONGED_GRAVITY_FORCE Force(bn::fixed_point_t<12>(0, PLAYER_PROLONGED_GRAVITY), PLAYER_GRAVITY_DECAY)
#define PLAYER_FAST_GRAVITY_FORCE      Force(bn::fixed_point_t<12>(0, PLAYER_FAST_FALL_GRAVITY), PLAYER_GRAVITY_DECAY)
#define PLAYER_WALL_GRAVITY_FORCE      Force(bn::fixed_point_t<12>(0, PLAYER_WALL_RIDE_GRAVITY), PLAYER_GRAVITY_DECAY)
#define PLAYER_ATTACK_GRAVITY_FORCE    Force(bn::fixed_point_t<12>(0, PLAYER_ATTACK_GRAVITY),    PLAYER_GRAVITY_DECAY)

#define PLAYER_PHASE_STEP_EXIT_FORCE_UP    Force(bn::fixed_point_t<12>(0, -PLAYER_PHASE_STEP_EXIT_Y_FORCE), PLAYER_PHASE_STEP_EXIT_DECAY)
#define PLAYER_PHASE_STEP_EXIT_FORCE_DOWN  Force(bn::fixed_point_t<12>(0,  PLAYER_PHASE_STEP_EXIT_Y_FORCE), PLAYER_PHASE_STEP_EXIT_DECAY)
#define PLAYER_PHASE_STEP_EXIT_FORCE_LEFT  Force(bn::fixed_point_t<12>(-PLAYER_PHASE_STEP_EXIT_X_FORCE, 0), PLAYER_PHASE_STEP_EXIT_DECAY)
#define PLAYER_PHASE_STEP_EXIT_FORCE_RIGHT Force(bn::fixed_point_t<12>(PLAYER_PHASE_STEP_EXIT_X_FORCE,  0), PLAYER_PHASE_STEP_EXIT_DECAY)

enum PhaseDir
{
	PHASE_UP = 0,
	PHASE_DOWN,
	PHASE_LEFT,
	PHASE_RIGHT
};

struct Player : GameObject {
	
	///////////////////
	// Struct Player //
	///////////////////

	bn::fixed       x_speed;
	bn::fixed_point phase_destination;

	int32 attack_buffered_frames;
	int32 roll_buffered_frames;
	int32 jump_buffered_frames;
	int32 remaining_jump_input_frames;
	int32 remaining_x_drift_lockout_frames;
	int32 air_frames_elapsed;
	int32 v_collision_grace_frames;
	int32 late_jump_grace_frames;
	int32 late_roll_jump_grace_frames;
	int32 current_phase_frame;
	int32 hitstop_frames;
	
	bool wall_right_detected;
    bool wall_left_detected;
    bool grounded_detected;
	bool grounded_owp_detected;
	bool left_wj_eligible;
	bool right_wj_eligible;

	Collider test_collider;
	Collider test_collider_right;
	Collider test_collider_left;

	Hitbox* hitbox_1_ptr;
	Hitbox* hitbox_2_ptr;
	Hitbox* hitbox_3_ptr;

	bn::optional<bn::sprite_ptr> pm_sprite_ptr;
	PhaseDir phase_dir;

	bn::optional<bn::sprite_ptr>                                         jump_effect_sprite_ptr;
	bn::optional<bn::sprite_animate_action<GAME_OBJECT_MAX_ANIM_FRAMES>> jump_effect_anim_ptr;
	
	Player();
	Player(const Player& other);
	~Player();

	Player& operator =(const Player& other);

	void jump();
	void rollJump();
	void wallJump();
	void fastFall();
	void createGroundedAttackHitboxes(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects, 
		                              const bn::camera_ptr&                      camera);
	void createAirAttack1Hitboxes(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects, 
								  const bn::camera_ptr&                      camera);
	void createJumpEffect();
	void createAirJumpEffect();
	void createWallJumpEffect();
	void createLandEffect();

	//////////////////////////
	// GameObject Overrides //
	//////////////////////////

	void updateHitboxes(const RoomBounds& 							   room_bounds,
                        bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                        const bn::regular_bg_ptr&                      bg_ptr, 
                        const bn::span<const bn::regular_bg_map_cell>& cells,
                        const bn::regular_bg_item&                     bg_item,
                        const bn::camera_ptr&                          camera) override;

	void updateTimers() override;

	void draw() override;

	void setCamera(const bn::camera_ptr& camera) override;

	//////////////////////////////
	// State Function Overrides //
	//////////////////////////////

	void updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
							const bn::regular_bg_ptr&                      bg_ptr, 
							const bn::span<const bn::regular_bg_map_cell>& cells,
							const bn::regular_bg_item&                     bg_item,
							const bn::camera_ptr&                          camera) override;

	void updateState(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
					 const bn::regular_bg_ptr&                      bg_ptr,
					 const bn::span<const bn::regular_bg_map_cell>& cells,
					 const bn::regular_bg_item&                     bg_item) override;

	void getStateFromTiles(const bn::regular_bg_ptr&                      bg_ptr, 
		                   const bn::span<const bn::regular_bg_map_cell>& cells,
					       const bn::regular_bg_item&                     bg_item) override;

	void getStateFromObjects(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects) override;

	void setState(ObjectState new_state) override;

	/////////////////////////
	// Collision Overrides //
	/////////////////////////

	// Level Objects
	void resolveTilePassageCollision(GameObject& object)         override;
	void resolvePhaseOrbUpCollision(GameObject& object)          override;
	void resolvePhaseOrbDownCollision(GameObject& object)        override;
	void resolvePhaseOrbLeftCollision(GameObject& object)        override;
	void resolvePhaseOrbRightCollision(GameObject& object)       override;
	void resolveFallingPlatformWideCollision(GameObject& object) override;
	void resolveFallingPlatformThinCollision(GameObject& object) override;

	// Level Enemies
	void resolveThornColumnCollision(GameObject& object) override; 
	void resolveThornBarCollision(GameObject& object)    override; 
	void resolveGroundGhoulCollision(GameObject& object) override;

	// Special Objects
	void resolveHitboxAttackGround1Collision(GameObject& object) override;
	void resolveHitboxAir1Collision(GameObject& object)          override; 
	void resolveHitboxWallSplatCollision(GameObject& object)     override;
	void resolvePlayerCollision(GameObject& object)              override;

	// Tiles
	void resolveTileCollision(const bn::regular_bg_ptr&                      bg_ptr, 
							  const bn::span<const bn::regular_bg_map_cell>& cells,
							  const bn::regular_bg_item&                     bg_item) override;

	void resolveHardBlockCollision(const Collider& other_collider)   override;
	void resolveOneWayBlockCollision(const Collider& other_collider) override;
};

#endif
