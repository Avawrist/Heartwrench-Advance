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

#define PLAYER_COLLIDER_WIDTH  8
#define PLAYER_COLLIDER_HEIGHT 16

#define PLAYER_COLLIDER_OFFSET_X 0
#define PLAYER_COLLIDER_OFFSET_Y 7

#define PLAYER_WALL_TEST_RAY_LENGTH 1

#define PLAYER_FALL_STRETCH_V 1.5
#define PLAYER_FALL_STRETCH_H 0.75

#define PLAYER_MAX_HITPOINTS      3
#define PLAYER_STARTING_HITPOINTS 3

#define PLAYER_OD_1_HP_REQUIRED   2
#define PLAYER_OD_2_HP_REQUIRED   3

#define PLAYER_GET_HP_HITSTOP_FRAMES   5
#define PLAYER_GET_MOON_HITSTOP_FRAMES 0
#define PLAYER_GET_STAR_HITSTOP_FRAMES 0

#define PLAYER_HIT_INVULNERABILITY_FRAMES 80

#define PLAYER_ATTACK_BUFFER_FRAMES 12
#define PLAYER_SPIN_BUFFER_FRAMES   12
#define PLAYER_JUMP_BUFFER_FRAMES   12

#define PLAYER_MIN_X_SPEED    0
#define PLAYER_MAX_X_SPEED    2
#define PLAYER_X_DECAY        1
#define X_SPEED_ACC_RATE      0.1
#define X_SPEED_DECAY_RATE    0.075

#define PLAYER_CLIMB_SPEED 1
#define PLAYER_CLIMB_DECAY 1

#define PLAYER_BASE_JUMP_FORCE -7 // 2 block height at 0.033 decay rate
#define PLAYER_JUMP_DECAY       0.033

#define PLAYER_SECOND_JUMP_Y_FORCE  -1 // 1 extra block height if jump held all possible frames
#define PLAYER_SECONDARY_JUMP_DECAY  1

#define PLAYER_TERTIARY_JUMP_Y_FORCE -2 // Slows fall if jump held
#define PLAYER_TERTIARY_JUMP_DECAY    1

//#define PLAYER_WALL_JUMP_X_FORCE           2
//#define PLAYER_WALL_JUMP_Y_FORCE          -7
//#define PLAYER_WALL_JUMP_DECAY             0.033

#define PLAYER_MAX_JUMP_INPUT_FRAMES 20

#define PLAYER_HEAD_BONK_H_STRETCH 1.6
#define PLAYER_HEAD_BONK_V_STRETCH 0.7

#define PLAYER_GRAVITY       	             3
#define PLAYER_PROLONGED_GRAVITY             2
#define PLAYER_PROLONGED_AIR_FRAMES_REQUIRED 20
#define PLAYER_FAST_FALL_GRAVITY             2
#define PLAYER_MIN_FAST_FALL_FRAMES          3
#define PLAYER_WALL_RIDE_GRAVITY             1
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

#define PLAYER_SPIN_X_FORCE 4
#define PLAYER_SPIN_DECAY   0.02

#define PLAYER_MAX_SPIN_EFFECT_FRAMES 35

#define PLAYER_SPIN_1_X_OFFSET           16
#define PLAYER_SPIN_1_Y_OFFSET           -8
#define PLAYER_SPIN_1_HB_WIDTH           32
#define PLAYER_SPIN_1_HB_HEIGHT          22
#define PLAYER_SPIN_1_CREATE_HB_FRAME    1
#define PLAYER_SPIN_1_HB_LIFESPAN_FRAMES 5
#define PLAYER_SPIN_1_X_KNOCKBACK        8
#define PLAYER_SPIN_1_Y_KNOCKBACK        0
#define PLAYER_SPIN_1_KNOCKBACK_DECAY    0.05
#define PLAYER_SPIN_1_DAMAGE             1
#define PLAYER_SPIN_1_HITSTOP_FRAMES     12
#define PLAYER_SPIN_1_HITSTUN_FRAMES     30
#define PLAYER_SPIN_1_SCREENSHAKE_FRAMES 12
#define PLAYER_SPIN_1_SCREENSHAKE_SEVERITY STRONG_SHAKE

#define PLAYER_SPIN_2_X_OFFSET           16
#define PLAYER_SPIN_2_Y_OFFSET           -8
#define PLAYER_SPIN_2_HB_WIDTH           32
#define PLAYER_SPIN_2_HB_HEIGHT          22
#define PLAYER_SPIN_2_CREATE_HB_FRAME    21
#define PLAYER_SPIN_2_HB_LIFESPAN_FRAMES 5
#define PLAYER_SPIN_2_X_KNOCKBACK        8
#define PLAYER_SPIN_2_Y_KNOCKBACK        0
#define PLAYER_SPIN_2_KNOCKBACK_DECAY    0.05
#define PLAYER_SPIN_2_DAMAGE             1
#define PLAYER_SPIN_2_HITSTOP_FRAMES     12
#define PLAYER_SPIN_2_HITSTUN_FRAMES     30
#define PLAYER_SPIN_2_SCREENSHAKE_FRAMES 12
#define PLAYER_SPIN_2_SCREENSHAKE_SEVERITY STRONG_SHAKE

#define PLAYER_V_COLLISION_MAX_GRACE_FRAMES 4
#define PLAYER_LATE_JUMP_GRACE_FRAMES       6

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
//#define PLAYER_WALL_JUMP_FORCE         Force(bn::fixed_point_t<12>(PLAYER_WALL_JUMP_X_FORCE * (int32)x_dir, PLAYER_WALL_JUMP_Y_FORCE), PLAYER_WALL_JUMP_DECAY)

#define PLAYER_GRAVITY_FORCE           Force(bn::fixed_point_t<12>(0, PLAYER_GRAVITY), 			 PLAYER_GRAVITY_DECAY)
#define PLAYER_PROLONGED_GRAVITY_FORCE Force(bn::fixed_point_t<12>(0, PLAYER_PROLONGED_GRAVITY), PLAYER_GRAVITY_DECAY)
#define PLAYER_FAST_GRAVITY_FORCE      Force(bn::fixed_point_t<12>(0, PLAYER_FAST_FALL_GRAVITY), PLAYER_GRAVITY_DECAY)
#define PLAYER_WALL_GRAVITY_FORCE      Force(bn::fixed_point_t<12>(0, PLAYER_WALL_RIDE_GRAVITY), PLAYER_GRAVITY_DECAY)

#define PLAYER_PHASE_STEP_EXIT_FORCE_UP    Force(bn::fixed_point_t<12>(0, -PLAYER_PHASE_STEP_EXIT_Y_FORCE), PLAYER_PHASE_STEP_EXIT_DECAY)
#define PLAYER_PHASE_STEP_EXIT_FORCE_DOWN  Force(bn::fixed_point_t<12>(0,  PLAYER_PHASE_STEP_EXIT_Y_FORCE), PLAYER_PHASE_STEP_EXIT_DECAY)
#define PLAYER_PHASE_STEP_EXIT_FORCE_LEFT  Force(bn::fixed_point_t<12>(-PLAYER_PHASE_STEP_EXIT_X_FORCE, 0), PLAYER_PHASE_STEP_EXIT_DECAY)
#define PLAYER_PHASE_STEP_EXIT_FORCE_RIGHT Force(bn::fixed_point_t<12>(PLAYER_PHASE_STEP_EXIT_X_FORCE,  0), PLAYER_PHASE_STEP_EXIT_DECAY)

#define PLAYER_CLIMB_UP_FORCE Force(bn::fixed_point_t<12>(0, -PLAYER_CLIMB_SPEED), PLAYER_CLIMB_DECAY)
#define PLAYER_CLIMB_DOWN_FORCE Force(bn::fixed_point_t<12>(0, PLAYER_CLIMB_SPEED), PLAYER_CLIMB_DECAY)
#define PLAYER_CLIMB_LEFT_FORCE Force(bn::fixed_point_t<12>(-PLAYER_CLIMB_SPEED, 0), PLAYER_CLIMB_DECAY)
#define PLAYER_CLIMB_RIGHT_FORCE Force(bn::fixed_point_t<12>(PLAYER_CLIMB_SPEED, 0), PLAYER_CLIMB_DECAY)

#define PLAYER_SPIN_FORCE Force(bn::fixed_point_t<12>(PLAYER_SPIN_X_FORCE * (int32)(x_dir), 0), PLAYER_SPIN_DECAY)

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

	int32 spin_buffered_frames;
	int32 jump_buffered_frames;
	int32 remaining_jump_input_frames;
	int32 air_frames_elapsed;
	int32 v_collision_grace_frames;
	int32 late_jump_grace_frames;
	int32 late_spin_jump_grace_frames;
	int32 current_phase_frame;
	int32 hitstop_frames;
	int32 spin_effect_frames;
	int32 spin_effect_offset_multiplier;
	int32 currency_collected;

	bool wall_right_detected;
    bool wall_left_detected;
	bool climbable_detected;
	bool grounded_owp_detected;
	bool left_wj_eligible;
	bool right_wj_eligible;
	bool a_requested;
	bool b_requested;
	bool r_requested;

	Collider test_collider_right;
	Collider test_collider_left;

	Hitbox* hitbox_1_ptr;
	Hitbox* hitbox_2_ptr;
	Hitbox* hitbox_3_ptr;

	bn::optional<bn::sprite_ptr> pm_sprite_ptr;
	PhaseDir phase_dir;

	bn::optional<bn::sprite_ptr>                                         jump_effect_sprite_ptr;
	bn::optional<bn::sprite_animate_action<GAME_OBJECT_MAX_ANIM_FRAMES>> jump_effect_anim_ptr;

	bn::optional<bn::sprite_ptr>                                         spin_effect_sprite_1_ptr;
	bn::optional<bn::sprite_ptr>                                         spin_effect_sprite_2_ptr;
	bn::optional<bn::sprite_ptr>                                         spin_effect_sprite_3_ptr;

	Player();
	Player(const Player& other);
	~Player();

	Player& operator =(const Player& other);

	void jump();
	void spinJump();
	//void wallJump();
	void fastFall();
	void createSpinAttack1Hitbox(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects, 
								 const bn::camera_ptr&                      camera);
	void createSpinAttack2Hitbox(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects, 
								const bn::camera_ptr&                      camera);
	void createWallJumpEffect();
	void drawSpinEffect();

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

	void updateHitFlash() override;

	void updateTestColliders() override;

	void draw() override;

	void hideSprites() override;
	
	void revealSprites() override;

	void setCamera(const bn::camera_ptr& camera) override;

	void setHitFlash() override;

	void setHitFlash(int32 frames) override;

	void applyHit(int32 _damage, int32 knockback_x_dir, int32 knockback_y_dir) override;

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
	void resolvePushBlockCollision(GameObject& object)           override;
	void resolvePushBlockMiniCollision(GameObject& object)       override;
	void resolveAutoPlatformCollision(GameObject& object)        override;
	void resolveSmashBlockLargeCollision(GameObject& object)     override;
	void resolveSmashBlockMiniCollision(GameObject& object)      override;
	void resolveHPTotemCollision(GameObject& object)             override;
	void resolveHPDropCollision(GameObject& object)              override;
	void resolveMoonDropCollision(GameObject& object)            override;
	void resolveStarDropCollision(GameObject& object)            override;

	// Level Enemies
	void resolveThornColumnCollision(GameObject& object) override; 
	void resolveThornBarCollision(GameObject& object)    override; 
	void resolveGroundGhoulCollision(GameObject& object) override;

	// Tiles
	void resolveTileCollision(const bn::regular_bg_ptr&                      bg_ptr, 
							  const bn::span<const bn::regular_bg_map_cell>& cells,
							  const bn::regular_bg_item&                     bg_item) override;

	void resolveHardBlockCollision(const Collider& other_collider)   override;
	void resolveClimbableCollision(const Collider& other_collider)   override;
	void resolveOneWayBlockCollision(const Collider& other_collider) override;
};

#endif
