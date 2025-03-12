#ifndef TEST_ENEMY_H
#define TEST_ENEMY_H

// Butano
#include "bn_math.h"

// Assets
#include "bn_sprite_items_test_enemy.h"

// Base Class
#include "game_object.h"

//////////////////////
// Struct TestEnemy //
//////////////////////

#define TEST_ENEMY_COLLIDER_WIDTH  16
#define TEST_ENEMY_COLLIDER_HEIGHT 16

#define TEST_ENEMY_GRAVITY         3
#define TEST_ENEMY_GRAVITY_DECAY   1

#define TEST_ENEMY_GRAVITY_FORCE   Force(bn::fixed_point_t<12>(0, TEST_ENEMY_GRAVITY), TEST_ENEMY_GRAVITY_DECAY)

struct TestEnemy : GameObject {

    TestEnemy();
	TestEnemy(const TestEnemy& other);
	~TestEnemy();

	TestEnemy& operator =(const TestEnemy& other);

	void update(const RoomBounds&                              room_bounds,
				bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
				const bn::regular_bg_ptr&                      bg_ptr, 
                const bn::span<const bn::regular_bg_map_cell>& cells,
                const bn::regular_bg_item&                     bg_item,
				const bn::camera_ptr&                          camera) override;

};

#endif