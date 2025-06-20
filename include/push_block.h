#ifndef PUSH_BLOCK_H
#define PUSH_BLOCK_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "game_object.h"

//////////////////////
// Struct PushBlock //
//////////////////////

#define PUSH_BLOCK_COLLIDER_WIDTH  32
#define PUSH_BLOCK_COLLIDER_HEIGHT 32

#define PUSH_BLOCK_COLLIDER_OFFSET_X 0
#define PUSH_BLOCK_COLLIDER_OFFSET_Y 0

#define PUSH_BLOCK_HITPOINTS 0

#define PUSH_BLOCK_MOMENTUM_TRANSFER_X_FORCE 6
#define PUSH_BLOCK_MOMENTUM_TRANSFER_Y_FORCE 6
#define PUSH_BLOCK_MOMENTUM_TRANSFER_DECAY   0.05

#define PUSH_BLOCK_MOMENTUM_TRANSFER_H_FORCE Force(bn::fixed_point_t<12>(PUSH_BLOCK_MOMENTUM_TRANSFER_X_FORCE * rigidbody.normalized_dir.x(), 0), PUSH_BLOCK_MOMENTUM_TRANSFER_DECAY)
#define PUSH_BLOCK_MOMENTUM_TRANSFER_V_FORCE Force(bn::fixed_point_t<12>(0, PUSH_BLOCK_MOMENTUM_TRANSFER_Y_FORCE * rigidbody.normalized_dir.y()), PUSH_BLOCK_MOMENTUM_TRANSFER_DECAY)

struct PushBlock : GameObject {

    //////////////////////
    // Struct PushBlock //
    //////////////////////

    bn::fixed_point frame_start_pos;

    bool hit_h_wall;
    bool hit_v_wall;

    PushBlock();
    PushBlock(const PushBlock& other);
    ~PushBlock();

    PushBlock& operator =(const PushBlock& other);

    //////////////////////////
    // GameObject Overrides //
    //////////////////////////

    void updatePhysics() override;
    void checkIfDead() override;

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

    void getStateFromObjects(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects) override;

    void getStateFromTiles(const bn::regular_bg_ptr&                      bg_ptr,
                           const bn::span<const bn::regular_bg_map_cell>& cells,
                           const bn::regular_bg_item&                     bg_item) override;

    /////////////////////////
    // Collision Overrides //
    /////////////////////////

    void resolveObjectCollision(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects) override;

    // Level Objects
	void resolveTilePassageCollision(GameObject& object)         override;
	void resolveFallingPlatformWideCollision(GameObject& object) override;
	void resolveFallingPlatformThinCollision(GameObject& object) override;
	void resolvePushBlockCollision(GameObject& object)           override;

	// Special Objects
	void resolvePlayerCollision(GameObject& object)              override;

    // Tiles
    void resolveTileCollision(const bn::regular_bg_ptr&                      bg_ptr, 
                              const bn::span<const bn::regular_bg_map_cell>& cells,
                              const bn::regular_bg_item&                     bg_item) override;

    void resolveHGearLeftCollision(const Collider& other_collider)  override;
	void resolveHGearMidCollision(const Collider& other_collider)   override;
	void resolveHGearRightCollision(const Collider& other_collider) override;

    void resolveXAxisCollision(const Collider& other_collider) override;
    void resolveYAxisCollision(const Collider& other_collider) override;

};

#endif