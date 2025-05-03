#ifndef GROUND_GHOUL_H
#define GROUND_GHOUL_H

// Butano
#include "bn_math.h"

// Base Class
#include "game_object.h"
#include "tile_passage.h"

//////////////////////
// Struct TestEnemy //
//////////////////////

#define GROUND_GHOUL_COLLIDER_WIDTH  16
#define GROUND_GHOUL_COLLIDER_HEIGHT 16

#define GROUND_GHOUL_COLLIDER_OFFSET_X 0
#define GROUND_GHOUL_COLLIDER_OFFSET_Y 0

#define GROUND_GHOUL_CRAWL_SPEED 1
#define GROUND_GHOUL_CRAWL_DECAY 1

#define GROUND_GHOUL_GRAVITY       2
#define GROUND_GHOUL_GRAVITY_DECAY 1

#define GROUND_GHOUL_GRAVITY_FORCE Force(bn::fixed_point_t<12>(0, GROUND_GHOUL_GRAVITY), GROUND_GHOUL_GRAVITY_DECAY)
#define GROUND_GHOUL_CRAWL_FORCE   Force(bn::fixed_point_t<12>(GROUND_GHOUL_CRAWL_SPEED * x_dir, 0), GROUND_GHOUL_CRAWL_DECAY)

#define GROUND_GHOUL_HITPOINTS 1

struct GroundGhoul : GameObject {

	bool grounded_detected;

	Collider test_collider;
	Collider test_collider_right;
	Collider test_collider_left;

    GroundGhoul();
	GroundGhoul(const GroundGhoul& other);
	~GroundGhoul();

	GroundGhoul& operator =(const GroundGhoul& other);

	void update(const RoomBounds&                              room_bounds,
				bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
				const bn::regular_bg_ptr&                      bg_ptr, 
                const bn::span<const bn::regular_bg_map_cell>& cells,
                const bn::regular_bg_item&                     bg_item,
				const bn::camera_ptr&                          camera) override;
	void setState(ObjectState new_state) override;

};

#endif