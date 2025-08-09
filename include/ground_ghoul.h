#ifndef GROUND_GHOUL_H
#define GROUND_GHOUL_H

// Butano
#include "bn_math.h"

// Base Class
#include "enemy.h"
#include "tile_passage.h"

//////////////////////
// Struct TestEnemy //
//////////////////////

#define GROUND_GHOUL_COLLIDER_WIDTH  12
#define GROUND_GHOUL_COLLIDER_HEIGHT 16

#define GROUND_GHOUL_COLLIDER_OFFSET_X 0
#define GROUND_GHOUL_COLLIDER_OFFSET_Y 0

#define GROUND_GHOUL_CRAWL_X_FORCE 1
#define GROUND_GHOUL_CRAWL_DECAY   1

#define GROUND_GHOUL_ACTION_TIMER 20
#define GROUND_GHOUL_CRAWL_FRAME  18

#define GROUND_GHOUL_CRAWL_FORCE Force(bn::fixed_point_t<12>(GROUND_GHOUL_CRAWL_X_FORCE * x_dir, 0), GROUND_GHOUL_CRAWL_DECAY)

#define GROUND_GHOUL_HITPOINTS 2

#define GROUND_GHOUL_30_FPS false

struct GroundGhoul : Enemy {

	////////////////////////
	// Struct GroundGhoul //
	////////////////////////

	int32      action_timer;
	XDirection next_crawl_dir;

	Collider test_collider;

    GroundGhoul();
	GroundGhoul(const GroundGhoul& other);
	~GroundGhoul();

	GroundGhoul& operator =(const GroundGhoul& other);

	//////////////////////////
	// GameObject Overrides //
	//////////////////////////

	void updateTimers() override;

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

	// None..

};

#endif