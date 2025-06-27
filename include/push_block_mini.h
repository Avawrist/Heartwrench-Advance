#ifndef PUSH_BLOCK_MINI_H
#define PUSH_BLOCK_MINI_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "game_object.h"

//////////////////////////
// Struct PushBlockMini //
//////////////////////////

#define PUSH_BLOCK_MINI_COLLIDER_WIDTH  16
#define PUSH_BLOCK_MINI_COLLIDER_HEIGHT 16

#define PUSH_BLOCK_MINI_COLLIDER_OFFSET_X 0
#define PUSH_BLOCK_MINI_COLLIDER_OFFSET_Y 0

#define PUSH_BLOCK_MINI_HITPOINTS 0
#define PUSH_BLOCK_MINI_DAMAGE    10

#define PUSH_BLOCK_MINI_ROOF_OFFSET          ((PUSH_BLOCK_MINI_COLLIDER_HEIGHT / 2) * -1)
#define PUSH_BLOCK_MINI_ROOF_COLLIDER_HEIGHT 32

#define PUSH_BLOCK_MINI_HIT_H_WALL_FRAMES 5
#define PUSH_BLOCK_MINI_HIT_V_WALL_FRAMES 5

#define PUSH_BLOCK_MINI_MOMENTUM_TRANSFER_X_FORCE 4
#define PUSH_BLOCK_MINI_MOMENTUM_TRANSFER_Y_FORCE 4
#define PUSH_BLOCK_MINI_MOMENTUM_TRANSFER_DECAY   0.03

#define PUSH_BLOCK_MINI_TRACK_DECAY 0.01
#define PUSH_BLOCK_MINI_TRACK_FRICTION_H_FORCE Force(bn::fixed_point_t<12>(rigidbody.final_dir.x() / 3, 0), PUSH_BLOCK_MINI_TRACK_DECAY)

#define PUSH_BLOCK_MINI_MOMENTUM_TRANSFER_H_FORCE Force(bn::fixed_point_t<12>(PUSH_BLOCK_MINI_MOMENTUM_TRANSFER_X_FORCE * (int32)x_dir * -1, 0), PUSH_BLOCK_MINI_MOMENTUM_TRANSFER_DECAY)
#define PUSH_BLOCK_MINI_MOMENTUM_TRANSFER_V_FORCE Force(bn::fixed_point_t<12>(0, PUSH_BLOCK_MINI_MOMENTUM_TRANSFER_Y_FORCE * (int32)y_dir) * -1, PUSH_BLOCK_MINI_MOMENTUM_TRANSFER_DECAY)

struct PushBlockMini : GameObject {

    //////////////////////
    // Struct PushBlockMini //
    //////////////////////

    bn::fixed_point frame_start_pos;

    int32 hit_h_wall;
    int32 hit_v_wall;
    bool received_track_force;

    PushBlockMini();
    PushBlockMini(const PushBlockMini& other);
    ~PushBlockMini();

    PushBlockMini& operator =(const PushBlockMini& other);

    //////////////////////////
    // GameObject Overrides //
    //////////////////////////

    void updatePhysics()         override;
    void checkIfDead()           override;
    void updateSpriteDirection() override;
    void updateTimers()          override;

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
	void resolveFallingPlatformWideCollision(GameObject& object) override;
	void resolveFallingPlatformThinCollision(GameObject& object) override;

    // Enemy Objects
    void resolveGroundGhoulCollision(GameObject& object) override;

	// Special Objects
	void resolvePlayerCollision(GameObject& object) override;

    // Tiles
    void resolveTileCollision(const bn::regular_bg_ptr&                      bg_ptr, 
                              const bn::span<const bn::regular_bg_map_cell>& cells,
                              const bn::regular_bg_item&                     bg_item) override;

    void resolveXAxisCollision(const Collider& other_collider) override;
    void resolveYAxisCollision(const Collider& other_collider) override;

};

#endif