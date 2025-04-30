#ifndef WALL_LEFT_GHOUL_H
#define WALL_LEFT_GHOUL_H

// Butano
#include "bn_math.h"

// Assets
#include "bn_sprite_items_wall_left_ghoul.h"

// Base Class
#include "game_object.h"

//////////////////////
// Struct TestEnemy //
//////////////////////

#define WALL_LEFT_GHOUL_COLLIDER_WIDTH  16
#define WALL_LEFT_GHOUL_COLLIDER_HEIGHT 16

#define WALL_LEFT_GHOUL_COLLIDER_OFFSET_X 0
#define WALL_LEFT_GHOUL_COLLIDER_OFFSET_Y 0

#define WALL_LEFT_GHOUL_CRAWL_SPEED 1
#define WALL_LEFT_GHOUL_CRAWL_DECAY 1

#define WALL_LEFT_GHOUL_CRAWL_FORCE   Force(bn::fixed_point_t<12>(0, WALL_LEFT_GHOUL_CRAWL_SPEED * dir), WALL_LEFT_GHOUL_CRAWL_DECAY)

enum WallLeftGhoulState
{
	WALL_LEFT_GHOUL_IDLE_STATE,
	WALL_LEFT_GHOUL_CRAWL_STATE,
	WALL_LEFT_GHOUL_HITSTUN_STATE,
	WALL_LEFT_GHOUL_DEATH_STATE,
};

struct WallLeftGhoul : GameObject {

	WallLeftGhoulState state;

    WallLeftGhoul();
	WallLeftGhoul(const WallLeftGhoul& other);
	~WallLeftGhoul();

	WallLeftGhoul& operator =(const WallLeftGhoul& other);

	void update(const RoomBounds&                              room_bounds,
				bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
				const bn::regular_bg_ptr&                      bg_ptr, 
                const bn::span<const bn::regular_bg_map_cell>& cells,
                const bn::regular_bg_item&                     bg_item,
				const bn::camera_ptr&                          camera) override;
	void setState(WallLeftGhoulState new_state);

};

#endif