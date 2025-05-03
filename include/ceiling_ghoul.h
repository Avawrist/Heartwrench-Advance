#ifndef CEILING_GHOUL_H
#define CEILING_GHOUL_H

// Butano
#include "bn_math.h"

// Base Class
#include "game_object.h"

//////////////////////
// Struct TestEnemy //
//////////////////////

#define CEILING_GHOUL_COLLIDER_WIDTH  16
#define CEILING_GHOUL_COLLIDER_HEIGHT 16

#define CEILING_GHOUL_COLLIDER_OFFSET_X 0
#define CEILING_GHOUL_COLLIDER_OFFSET_Y 0

#define CEILING_GHOUL_CRAWL_SPEED 1
#define CEILING_GHOUL_CRAWL_DECAY 1

#define CEILING_GHOUL_CRAWL_FORCE   Force(bn::fixed_point_t<12>(CEILING_GHOUL_CRAWL_SPEED * x_dir, 0), CEILING_GHOUL_CRAWL_DECAY)

#define CEILING_GHOUL_HITPOINTS 1

struct CeilingGhoul : GameObject {

    CeilingGhoul();
	CeilingGhoul(const CeilingGhoul& other);
	~CeilingGhoul();

	CeilingGhoul& operator =(const CeilingGhoul& other);

	void update(const RoomBounds&                              room_bounds,
				bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
				const bn::regular_bg_ptr&                      bg_ptr, 
                const bn::span<const bn::regular_bg_map_cell>& cells,
                const bn::regular_bg_item&                     bg_item,
				const bn::camera_ptr&                          camera) override;
	void setState(ObjectState new_state) override;

};

#endif