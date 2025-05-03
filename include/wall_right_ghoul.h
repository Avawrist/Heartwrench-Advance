#ifndef WALL_RIGHT_GHOUL_H
#define WALL_RIGHT_GHOUL_H

// Butano
#include "bn_math.h"

// Base Class
#include "game_object.h"

//////////////////////
// Struct TestEnemy //
//////////////////////

#define WALL_RIGHT_GHOUL_COLLIDER_WIDTH  16
#define WALL_RIGHT_GHOUL_COLLIDER_HEIGHT 16

#define WALL_RIGHT_GHOUL_COLLIDER_OFFSET_X 0
#define WALL_RIGHT_GHOUL_COLLIDER_OFFSET_Y 0

#define WALL_RIGHT_GHOUL_CRAWL_SPEED 1
#define WALL_RIGHT_GHOUL_CRAWL_DECAY 1

#define WALL_RIGHT_GHOUL_CRAWL_FORCE   Force(bn::fixed_point_t<12>(0, WALL_RIGHT_GHOUL_CRAWL_SPEED * y_dir), WALL_RIGHT_GHOUL_CRAWL_DECAY)

#define WALL_RIGHT_GHOUL_HITPOINTS 3

struct WallRightGhoul : GameObject {

    WallRightGhoul();
	WallRightGhoul(const WallRightGhoul& other);
	~WallRightGhoul();

	WallRightGhoul& operator =(const WallRightGhoul& other);

	void update(const RoomBounds&                              room_bounds,
				bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
				const bn::regular_bg_ptr&                      bg_ptr, 
                const bn::span<const bn::regular_bg_map_cell>& cells,
                const bn::regular_bg_item&                     bg_item,
				const bn::camera_ptr&                          camera) override;
	void setState(ObjectState new_state) override;

};

#endif