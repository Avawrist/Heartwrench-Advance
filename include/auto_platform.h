#ifndef AUTO_PLATFORM_H
#define AUTO_PLATFORM_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "game_object.h"

/////////////////////////
// Struct AutoPlatform //
/////////////////////////

#define AUTO_PLATFORM_COLLIDER_WIDTH  32
#define AUTO_PLATFORM_COLLIDER_HEIGHT 1

#define AUTO_PLATFORM_COLLIDER_OFFSET_X  0
#define AUTO_PLATFORM_COLLIDER_OFFSET_Y  0

#define AUTO_PLATFORM_HITPOINTS 0

#define AUTO_PLATFORM_ROOF_OFFSET -8
#define AUTO_PLATFORM_ROOF_COLLIDER_HEIGHT 16

#define AUTO_PLATFORM_SPEED 1
#define AUTO_PLATFORM_ROLL_FORCE (Force(bn::fixed_point_t<12>(AUTO_PLATFORM_SPEED * (int32)x_dir, AUTO_PLATFORM_SPEED * (int32)y_dir), 1))

struct AutoPlatform : GameObject {

    bool changed_x_dir;

    /////////////////////////
    // Struct AutoPlatform //
    /////////////////////////

    AutoPlatform();
    AutoPlatform(const AutoPlatform& other);
    ~AutoPlatform();

    AutoPlatform& operator =(const AutoPlatform& other);

    //////////////////////////
    // GameObject Overrides //
    //////////////////////////

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

    /////////////////////////
    // Collision Overrides //
    /////////////////////////

    // Level Enemies
    void resolveGroundGhoulCollision(GameObject& object) override;

    // Special Objects
    void resolvePlayerCollision(GameObject& object) override;

    // Tiles
    void resolveHardBlockCollision(const Collider& other_collider) override;
	void resolveHGearLeftCollision(const Collider& other_collider) override;
	void resolveHGearMidCollision(const Collider& other_collider) override;
	void resolveHGearRightCollision(const Collider& other_collider) override;
	void resolveVGearTopCollision(const Collider& other_collider) override;
	void resolveVGearMidCollision(const Collider& other_collider) override;
	void resolveVGearBottomCollision(const Collider& other_collider) override;
	void resolveUpSpikeCollision(const Collider& other_collider) override;
	void resolveDownSpikeCollision(const Collider& other_collider) override;
	void resolveLeftSpikeCollision(const Collider& other_collider) override;
	void resolveRightSpikeCollision(const Collider& other_collider) override;
	void resolveLeftShallowSlope1Collision(const Collider& other_collider, int32 world_y) override;
	void resolveLeftShallowSlope2Collision(const Collider& other_collider, int32 world_y) override;
	void resolveLeftShallowSlope3Collision(const Collider& other_collider, int32 world_y) override;
	void resolveLeftShallowSlope4Collision(const Collider& other_collider, int32 world_y) override;
	void resolveLeftSteepSlope1Collision(const Collider& other_collider, int32 world_y) override;
	void resolveLeftSteepSlope2Collision(const Collider& other_collider, int32 world_y) override;
	void resolveRightShallowSlope1Collision(const Collider& other_collider, int32 world_y) override;
	void resolveRightShallowSlope2Collision(const Collider& other_collider, int32 world_y) override;
	void resolveRightShallowSlope3Collision(const Collider& other_collider, int32 world_y) override;
	void resolveRightShallowSlope4Collision(const Collider& other_collider, int32 world_y) override;
	void resolveRightSteepSlope1Collision(const Collider& other_collider, int32 world_y) override;
	void resolveRightSteepSlope2Collision(const Collider& other_collider, int32 world_y) override;
	void resolveOneWayBlockCollision(const Collider& other_collider) override;

};

#endif