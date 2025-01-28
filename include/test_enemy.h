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

#define TEST_ENEMY_COLLIDER_WIDTH  24
#define TEST_ENEMY_COLLIDER_HEIGHT 24

struct TestEnemy : GameObject {

    TestEnemy();
	TestEnemy(const TestEnemy& other);
	~TestEnemy();

	void update(RoomBounds room_bounds,
				bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects,
				bn::regular_bg_ptr                         bg_ptr, 
                bn::span<const bn::regular_bg_map_cell>    cells,
                bn::regular_bg_item                        bg_item,
				bn::camera_ptr                             camera) override;

};

#endif