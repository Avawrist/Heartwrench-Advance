#ifndef SMALL_VASE_H
#define SMALL_VASE_H

// Butano

// My Libs
#include "utility.h"
#include "game_object.h"
#include "hitbox.h"

//////////////////////
// Struct SmallVase //
//////////////////////

#define SMALL_VASE_COLLIDER_WIDTH  16
#define SMALL_VASE_COLLIDER_HEIGHT 16

#define SMALL_VASE_COLLIDER_OFFSET_X 0
#define SMALL_VASE_COLLIDER_OFFSET_Y 8

#define SMALL_VASE_HITPOINTS 1

#define SMALL_VASE_30_FPS false

struct SmallVase : GameObject {

    //////////////////////
    // Struct SmallVase //
    //////////////////////

    SmallVase();
    SmallVase(const SmallVase& other);
    ~SmallVase();

    SmallVase& operator =(const SmallVase& other);

    void wallSplatCheck();

    //////////////////////////
    // GameObject Overrides //
    //////////////////////////

    void update(const RoomBounds& 							   room_bounds,
                bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                const bn::regular_bg_ptr&                      bg_ptr, 
                const bn::span<const bn::regular_bg_map_cell>& cells,
                const bn::regular_bg_item&                     bg_item,
                const bn::camera_ptr&                          camera) override;

    //////////////////////////////
    // State Function Overrides //
    //////////////////////////////

	void updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                            const bn::regular_bg_ptr&                      bg_ptr, 
                            const bn::span<const bn::regular_bg_map_cell>& cells,
                            const bn::regular_bg_item&                     bg_item,
                            const bn::camera_ptr&                          camera) override;

    void setState(ObjectState new_state) override;

    /////////////////////////
    // Collision Overrides //
    /////////////////////////

    // Level Objects
    void resolveTilePassageCollision(GameObject& object)         override;
    void resolveFallingPlatformWideCollision(GameObject& object) override;
    void resolveFallingPlatformThinCollision(GameObject& object) override;
    void resolvePushBlockCollision(GameObject& object)           override;
    void resolvePushBlockMiniCollision(GameObject& object)       override;
    void resolveAutoPlatformCollision(GameObject& object)        override;
    void resolveSmashBlockLargeCollision(GameObject& object)     override;
    void resolveSmashBlockMiniCollision(GameObject& object)      override;
    void resolveLargeVaseCollision(GameObject& object)           override;
    void resolveSmallVaseCollision(GameObject& object)           override;
    void resolveHPTotemCollision(GameObject& object)             override;

    // Level Enemies
    void resolveThornColumnCollision(GameObject& object) override;
    void resolveThornBarCollision(GameObject& object)    override;
	void resolveGroundGhoulCollision(GameObject& object) override;

    // Tiles
    void resolveSpikeCollision(const Collider& other_collider) override;

    void resolveXAxisCollision(const Collider& other_collider) override;

};

#endif