#ifndef WINGED_TROLL_L_H
#define WINGED_TROLL_L_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "enemy.h"

// Assets
#include "bn_sprite_items_winged_troll.h"

/////////////////////////
// Struct WingedTrollL //
/////////////////////////

#define WINGED_TROLL_L_COLLIDER_WIDTH  30
#define WINGED_TROLL_L_COLLIDER_HEIGHT 20

#define WINGED_TROLL_L_COLLIDER_OFFSET_X 0
#define WINGED_TROLL_L_COLLIDER_OFFSET_Y 0

#define WINGED_TROLL_L_HITPOINTS 1

#define WINGED_TROLL_L_30_FPS false

#define WINGED_TROLL_L_X_SPEED 1
#define WINGED_TROLL_L_X_DECAY 1

#define WINGED_TROLL_L_FLIGHT_RANGE 96

#define WINGED_TROLL_L_FLIGHT_FORCE Force(bn::fixed_point_t<12>(WINGED_TROLL_L_X_SPEED * (int32)(x_dir), 0), WINGED_TROLL_L_X_DECAY)

struct WingedTrollL : Enemy {

    /////////////////////////
    // Struct WingedTrollL //
    /////////////////////////

    bool first;

    int32 left_bound;
    int32 right_bound;

    WingedTrollL();
    WingedTrollL(const WingedTrollL& other);
    ~WingedTrollL();

    WingedTrollL& operator =(const WingedTrollL& other);

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