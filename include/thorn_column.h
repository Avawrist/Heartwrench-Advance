#ifndef THORN_COLUMN_H
#define THORN_COLUMN_H

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

#define THORN_COLUMN_COLLIDER_WIDTH    16
#define THORN_COLUMN_COLLIDER_HEIGHT   64
#define THORN_COLUMN_COLLIDER_OFFSET_X 0
#define THORN_COLUMN_COLLIDER_OFFSET_Y 0

#define THORN_COLUMN_HITPOINTS 0

struct ThornColumn : Enemy {

    ////////////////////////
    // Struct ThornColumn //
    ////////////////////////

    ThornColumn();
    ThornColumn(const ThornColumn& other);
    ~ThornColumn();

    ThornColumn& operator =(const ThornColumn& other);

    //////////////////////////
    // GameObject Overrides //
    //////////////////////////

    void checkIfDead()   override;
    void updatePhysics() override;

    //////////////////////////////
	// State Function Overrides //
	//////////////////////////////

    void updateState(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                     const bn::regular_bg_ptr&                      bg_ptr,
                     const bn::span<const bn::regular_bg_map_cell>& cells,
                     const bn::regular_bg_item&                     bg_item) override;

    void getStateFromTiles(const bn::regular_bg_ptr&                      bg_ptr,
                           const bn::span<const bn::regular_bg_map_cell>& cells,
                           const bn::regular_bg_item&                     bg_item) override;

	void getStateFromTilesLarge(const bn::regular_bg_ptr&                      bg_ptr,
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