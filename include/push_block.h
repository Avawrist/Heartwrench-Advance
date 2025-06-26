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
#define PUSH_BLOCK_DAMAGE    10

#define PUSH_BLOCK_ROOF_OFFSET          ((PUSH_BLOCK_COLLIDER_HEIGHT / 2) * -1)
#define PUSH_BLOCK_ROOF_COLLIDER_HEIGHT 64

#define PUSH_BLOCK_HIT_H_WALL_FRAMES 5
#define PUSH_BLOCK_HIT_V_WALL_FRAMES 5

#define PUSH_BLOCK_MOMENTUM_TRANSFER_X_FORCE 4
#define PUSH_BLOCK_MOMENTUM_TRANSFER_Y_FORCE 4
#define PUSH_BLOCK_MOMENTUM_TRANSFER_DECAY   0.03

#define PUSH_BLOCK_TRACK_DECAY 0.01
#define PUSH_BLOCK_TRACK_FRICTION_H_FORCE Force(bn::fixed_point_t<12>(rigidbody.final_dir.x() / 3, 0), PUSH_BLOCK_TRACK_DECAY)

#define PUSH_BLOCK_MOMENTUM_TRANSFER_H_FORCE Force(bn::fixed_point_t<12>(PUSH_BLOCK_MOMENTUM_TRANSFER_X_FORCE * (int32)x_dir * -1, 0), PUSH_BLOCK_MOMENTUM_TRANSFER_DECAY)
#define PUSH_BLOCK_MOMENTUM_TRANSFER_V_FORCE Force(bn::fixed_point_t<12>(0, PUSH_BLOCK_MOMENTUM_TRANSFER_Y_FORCE * (int32)y_dir) * -1, PUSH_BLOCK_MOMENTUM_TRANSFER_DECAY)

struct PushBlock : GameObject {

    //////////////////////
    // Struct PushBlock //
    //////////////////////

    bn::fixed_point frame_start_pos;

    int32 hit_h_wall;
    int32 hit_v_wall;
    bool received_track_force;

    PushBlock();
    PushBlock(const PushBlock& other);
    ~PushBlock();

    PushBlock& operator =(const PushBlock& other);

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

    // Get State From Objects
    void getStateFromTilePassage(GameObject& object)         override;
    void getStateFromFallingPlatformWide(GameObject& object) override;
	void getStateFromFallingPlatformThin(GameObject& object) override;

    // Get State From Tiles
    void getStateFromHardBlock(int32 world_x, int32 world_y)          override;
    void getStateFromUpSpike(int32 world_x, int32 world_y)            override;
	void getStateFromDownSpike(int32 world_x, int32 world_y)          override;
	void getStateFromLeftSpike(int32 world_x, int32 world_y)          override;
	void getStateFromRightSpike(int32 world_x, int32 world_y)         override;
	void getStateFromLeftShallowSlope1(int32 world_x, int32 world_y)  override;
	void getStateFromLeftShallowSlope2(int32 world_x, int32 world_y)  override;
	void getStateFromLeftShallowSlope3(int32 world_x, int32 world_y)  override;
	void getStateFromLeftShallowSlope4(int32 world_x, int32 world_y)  override;
	void getStateFromLeftSteepSlope1(int32 world_x, int32 world_y)    override;
	void getStateFromLeftSteepSlope2(int32 world_x, int32 world_y)    override;
	void getStateFromRightShallowSlope1(int32 world_x, int32 world_y) override;
	void getStateFromRightShallowSlope2(int32 world_x, int32 world_y) override;
	void getStateFromRightShallowSlope3(int32 world_x, int32 world_y) override;
	void getStateFromRightShallowSlope4(int32 world_x, int32 world_y) override;
	void getStateFromRightSteepSlope1(int32 world_x, int32 world_y)   override;
	void getStateFromRightSteepSlope2(int32 world_x, int32 world_y)   override;
	void getStateFromOneWayBlock(int32 world_x, int32 world_y)        override;

    /////////////////////////
    // Collision Overrides //
    /////////////////////////

    void resolveObjectCollision(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects) override;

    // Level Objects
	void resolveTilePassageCollision(GameObject& object)         override;
	void resolveFallingPlatformWideCollision(GameObject& object) override;
	void resolveFallingPlatformThinCollision(GameObject& object) override;

	// Special Objects
	void resolvePlayerCollision(GameObject& object) override;

    // Tiles
    void resolveTileCollision(const bn::regular_bg_ptr&                      bg_ptr, 
                              const bn::span<const bn::regular_bg_map_cell>& cells,
                              const bn::regular_bg_item&                     bg_item) override;

    void resolveHGearLeftCollision(const Collider& other_collider)  override;
	void resolveHGearMidCollision(const Collider& other_collider)   override;
	void resolveHGearRightCollision(const Collider& other_collider) override;

    void resolveVGearTopCollision(const Collider& other_collider)    override;
	void resolveVGearMidCollision(const Collider& other_collider)    override;
	void resolveVGearBottomCollision(const Collider& other_collider) override;    

    void resolveXAxisCollision(const Collider& other_collider) override;
    void resolveYAxisCollision(const Collider& other_collider) override;

};

#endif