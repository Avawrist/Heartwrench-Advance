#ifndef THORN_BAR_H
#define THORN_BAR_H

// Butano
#include "bn_optional.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_math.h"

// My Libs
#include "utility.h"
#include "collider.h"

// Base Class
#include "enemy.h"

#define THORN_BAR_COLLIDER_WIDTH    64
#define THORN_BAR_COLLIDER_HEIGHT   10
#define THORN_BAR_COLLIDER_OFFSET_X 0
#define THORN_BAR_COLLIDER_OFFSET_Y 0

#define THORN_BAR_HITPOINTS 0

struct ThornBar : Enemy {

    /////////////////////
    // Struct ThornBar //
    /////////////////////

    ThornBar();
    ThornBar(const ThornBar& other);
    ~ThornBar();

    ThornBar& operator =(const ThornBar& other);

    //////////////////////////
    // GameObject Overrides //
    //////////////////////////

    void updatePhysics() override;

    //////////////////////////////
	// State Function Overrides //
	//////////////////////////////

    void updateState(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                     const bn::regular_bg_ptr&                      bg_ptr,
                     const bn::span<const bn::regular_bg_map_cell>& cells,
                     const bn::regular_bg_item&                     bg_item) override;

    /////////////////////////
	// Collision Overrides //
	/////////////////////////

    void resolveCollision(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                          const bn::regular_bg_ptr&                      bg_ptr, 
                          const bn::span<const bn::regular_bg_map_cell>& cells,
                          const bn::regular_bg_item&                     bg_item) override;

};

#endif