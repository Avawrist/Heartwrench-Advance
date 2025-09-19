#ifndef STAR_JAR_H
#define STAR_JAR_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "game_object.h"

// Assets
#include "bn_sprite_items_star_jar.h"

/////////////////////
// Struct StarJar //
/////////////////////

#define STAR_JAR_COLLIDER_WIDTH  16
#define STAR_JAR_COLLIDER_HEIGHT 16

#define STAR_JAR_COLLIDER_OFFSET_X 0
#define STAR_JAR_COLLIDER_OFFSET_Y 0

#define STAR_JAR_HITPOINTS 5

#define STAR_JAR_30_FPS true

#define STAR_JAR_VALUE 0

#define STAR_JAR_STARTING_INVULN_FRAMES 10

#define STAR_JAR_SPAWN_Y_FORCE    -6
#define STAR_JAR_SPAWN_FORCE_DECAY 0.05

#define STAR_JAR_SPAWN_FORCE Force(bn::fixed_point_t<12>(0, STAR_JAR_SPAWN_Y_FORCE), STAR_JAR_SPAWN_FORCE_DECAY)

struct StarJar : GameObject {

    /////////////////////
    // Struct StarJar //
    /////////////////////

    StarJar();
    StarJar(const StarJar& other);
    ~StarJar();

    StarJar& operator =(const StarJar& other);

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
	void resolveHPTotemCollision(GameObject& object)             override;

    // Enemies
    void resolveBellTrollCollision(GameObject& object) override;

    // Tiles
    void resolveSpikeCollision(const Collider& other_collider) override;

};

#endif