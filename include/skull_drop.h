#ifndef SKULL_DROP_H
#define SKULL_DROP_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "game_object.h"

/////////////////////
// Struct SkullDrop //
/////////////////////

#define SKULL_DROP_COLLIDER_WIDTH  16
#define SKULL_DROP_COLLIDER_HEIGHT 12

#define SKULL_DROP_COLLIDER_OFFSET_X 0
#define SKULL_DROP_COLLIDER_OFFSET_Y 0

#define SKULL_DROP_HITPOINTS 5

#define SKULL_DROP_30_FPS false

#define SKULL_DROP_VALUE 3

#define SKULL_DROP_STARTING_INVULN_FRAMES 20

#define SKULL_DROP_SPAWN_Y_FORCE    -6
#define SKULL_DROP_SPAWN_FORCE_DECAY 0.05

#define SKULL_DROP_SPAWN_FORCE Force(bn::fixed_point_t<12>(0, SKULL_DROP_SPAWN_Y_FORCE), SKULL_DROP_SPAWN_FORCE_DECAY)

struct SkullDrop : GameObject {

    /////////////////////
    // Struct SkullDrop //
    /////////////////////

    SkullDrop();
    SkullDrop(const SkullDrop& other);
    ~SkullDrop();

    SkullDrop& operator =(const SkullDrop& other);

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
	void resolveTilePassageCollision(GameObject& object);
	void resolveFallingPlatformWideCollision(GameObject& object);
	void resolveFallingPlatformThinCollision(GameObject& object);
	void resolvePushBlockCollision(GameObject& object);
	void resolvePushBlockMiniCollision(GameObject& object);
	void resolveAutoPlatformCollision(GameObject& object);
	void resolveSmashBlockLargeCollision(GameObject& object);
	void resolveSmashBlockMiniCollision(GameObject& object);
	void resolveHPTotemCollision(GameObject& object);

    // Tiles
    void resolveSpikeCollision(const Collider& other_collider) override;

};

#endif