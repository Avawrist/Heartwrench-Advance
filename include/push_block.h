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

struct PushBlock : GameObject {

    //////////////////////
    // Struct PushBlock //
    //////////////////////

    PushBlock();
    PushBlock(const PushBlock& other);
    ~PushBlock();

    PushBlock& operator =(const PushBlock& other);

    //////////////////////////
    // GameObject Overrides //
    //////////////////////////

    void checkIfDead() override;

    //////////////////////////////
    // State Function Overrides //
    //////////////////////////////

    void updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                            const bn::regular_bg_ptr&                      bg_ptr, 
                            const bn::span<const bn::regular_bg_map_cell>& cells,
                            const bn::regular_bg_item&                     bg_item,
                            const bn::camera_ptr&                          camera) override;

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

    void resolveUpSpikeCollision(const Collider& other_collider)    override;
    void resolveDownSpikeCollision(const Collider& other_collider)  override;
    void resolveLeftSpikeCollision(const Collider& other_collider)  override;
    void resolveRightSpikeCollision(const Collider& other_collider) override;

};

#endif