#ifndef WINGED_TROLL_H
#define WINGED_TROLL_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "enemy.h"

//////////////////////
// Struct WingedTroll //
//////////////////////

#define WINGED_TROLL_COLLIDER_WIDTH  30
#define WINGED_TROLL_COLLIDER_HEIGHT 20

#define WINGED_TROLL_COLLIDER_OFFSET_X 0
#define WINGED_TROLL_COLLIDER_OFFSET_Y 0

#define WINGED_TROLL_HITPOINTS 1

#define WINGED_TROLL_30_FPS false

#define WINGED_TROLL_X_SPEED 1
#define WINGED_TROLL_X_DECAY 1

#define WINGED_TROLL_FLIGHT_RANGE 96

#define WINGED_TROLL_FLIGHT_FORCE Force(bn::fixed_point_t<12>(WINGED_TROLL_X_SPEED * (int32)(x_dir), 0), WINGED_TROLL_X_DECAY)

struct WingedTroll : Enemy {

    ////////////////////////
    // Struct WingedTroll //
    ////////////////////////

    bool first;

    int32 left_bound;
    int32 right_bound;

    WingedTroll();
    WingedTroll(const WingedTroll& other);
    ~WingedTroll();

    WingedTroll& operator =(const WingedTroll& other);

	//////////////////////////
	// GameObject Overrides //
	//////////////////////////

    // None..

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

    void resolveCollision(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                          const bn::regular_bg_ptr&                      bg_ptr, 
                          const bn::span<const bn::regular_bg_map_cell>& cells,
                          const bn::regular_bg_item&                     bg_item) override;

};

#endif