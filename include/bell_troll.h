#ifndef BELL_TROLL_H
#define BELL_TROLL_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "enemy.h"

//////////////////////
// Struct BellTroll //
//////////////////////

#define BELL_TROLL_COLLIDER_WIDTH  32
#define BELL_TROLL_COLLIDER_HEIGHT 32

#define BELL_TROLL_COLLIDER_OFFSET_X 0
#define BELL_TROLL_COLLIDER_OFFSET_Y 6

#define BELL_TROLL_HITPOINTS 3

#define BELL_TROLL_30_FPS false

#define BELL_TROLL_RUN_SPEED        2
#define BELL_TROLL_FROZEN_FRAMES    300 // 5 seconds
#define BELL_TROLL_TRANSITION_FRAME 90 //  1.5 seconds

#define BELL_TROLL_RUN_FORCE Force(bn::fixed_point_t<12>(BELL_TROLL_RUN_SPEED * (int32)(x_dir), 0), 1)

struct BellTroll : Enemy {

    //////////////////////
    // Struct BellTroll //
    //////////////////////

    uint32 frozen_frames;

    BellTroll();
    BellTroll(const BellTroll& other);
    ~BellTroll();

    BellTroll& operator =(const BellTroll& other);

    //////////////////////////
    // GameObject Overrides //
    //////////////////////////

    void updateHPBar()              override;
	void updateTimers()             override;
    void applyDamage(int32 _damage) override;
    void updateHitFlash()           override;
    void setHitFlash()              override;
	void setHitFlash(int32 frames)  override;

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

    void resolveXAxisCollision(const Collider& other_collider) override;

    // Objects
    void resolveBounceBellCollision(GameObject& object)     override;
	void resolveAutoBounceBellCollision(GameObject& object) override; 

    // Tiles
	void resolveTileCollision(const bn::regular_bg_ptr&                      bg_ptr, 
                              const bn::span<const bn::regular_bg_map_cell>& cells,
                              const bn::regular_bg_item&                     bg_item) override;

    void resolveSpikeCollision(const Collider& other_collider) override;

};

#endif