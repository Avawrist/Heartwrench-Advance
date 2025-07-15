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

#define HP_DROP_HITPOINTS 5

#define HP_DROP_30_FPS true

#define HP_DROP_VALUE 1

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
	void resolveLargeVaseCollision(GameObject& object);
	void resolveSmallVaseCollision(GameObject& object);
	void resolveHPTotemCollision(GameObject& object);

	// Level Enemies
	void resolveThornColumnCollision(GameObject& object); 
	void resolveThornBarCollision(GameObject& object); 
	void resolveGroundGhoulCollision(GameObject& object);


};

#endif