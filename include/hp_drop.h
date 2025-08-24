#ifndef HP_DROP_H
#define HP_DROP_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "game_object.h"

///////////////////
// Struct HPDrop //
///////////////////

#define HP_DROP_COLLIDER_WIDTH  22
#define HP_DROP_COLLIDER_HEIGHT 18

#define HP_DROP_COLLIDER_OFFSET_X 0
#define HP_DROP_COLLIDER_OFFSET_Y -1

#define HP_DROP_HITPOINTS 20

#define HP_DROP_30_FPS false

#define HP_DROP_VALUE         1
#define HP_DROP_MIN_HIT_SPEED 1

#define HP_DROP_STARTING_INVULN_FRAMES 20

#define HP_DROP_SPAWN_Y_FORCE    -8
#define HP_DROP_SPAWN_FORCE_DECAY 0.1

#define HP_DROP_SPAWN_FORCE Force(bn::fixed_point_t<12>(0, HP_DROP_SPAWN_Y_FORCE), HP_DROP_SPAWN_FORCE_DECAY)

struct HPDrop : GameObject {

    ///////////////////
    // Struct HPDrop //
    ///////////////////

    HPDrop();
    HPDrop(const HPDrop& other);
    ~HPDrop();

    HPDrop& operator =(const HPDrop& other);

    //////////////////////////
    // GameObject Overrides //
    //////////////////////////

    void update(const RoomBounds& 							   room_bounds,
                bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                const bn::regular_bg_ptr&                      bg_ptr, 
                const bn::span<const bn::regular_bg_map_cell>& cells,
                const bn::regular_bg_item&                     bg_item,
                const bn::camera_ptr&                          camera) override;

    void setState(ObjectState new_state) override;

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

    // Level Objects
	void resolveTilePassageCollision(GameObject& object)         override;
	void resolveFallingPlatformWideCollision(GameObject& object) override;
	void resolveFallingPlatformThinCollision(GameObject& object) override;
	void resolvePushBlockCollision(GameObject& object)           override;
	void resolvePushBlockMiniCollision(GameObject& object)       override;
	void resolveAutoPlatformCollision(GameObject& object)        override;
	void resolveSmashBlockLargeCollision(GameObject& object)     override;
	void resolveSmashBlockMiniCollision(GameObject& object)      override;
    void resolveSmashBlockZigguratLCollision(GameObject& object) override;
    void resolveSmashBlockZigguratCCollision(GameObject& object) override;
    void resolveSmashBlockZigguratRCollision(GameObject& object) override;
	void resolveLargeVaseCollision(GameObject& object)           override;
	void resolveSmallVaseCollision(GameObject& object)           override;
	void resolveHPTotemCollision(GameObject& object)             override;

	// Level Enemies
	void resolveThornColumnCollision(GameObject& object) override; 
	void resolveThornBarCollision(GameObject& object)    override; 
	void resolveGroundGhoulCollision(GameObject& object) override;
	void resolveBellTrollCollision(GameObject& object)   override;


};

#endif