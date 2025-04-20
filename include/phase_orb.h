#ifndef PHASE_ORB_H
#define PHASE_ORB_H

// Butano
#include "bn_math.h"

// Assets
#include "bn_sprite_items_phase_orb.h"

// Base Class
#include "game_object.h"

//////////////////////
// Struct TestEnemy //
//////////////////////

#define PHASE_ORB_Z_ORDER 1

#define PHASE_ORB_COLLIDER_WIDTH  16
#define PHASE_ORB_COLLIDER_HEIGHT 16

#define PHASE_ORB_COLLIDER_OFFSET_X 0
#define PHASE_ORB_COLLIDER_OFFSET_Y 0

#define PHASE_ORB_MOVE_SPEED 4
#define PHASE_ORB_MOVE_SPEED_DECAY 1

#define PHASE_ORB_MOVE_FORCE Force(bn::fixed_point_t<12>(PHASE_ORB_MOVED_SPEED * x_dir, PHASE_ORB_MOVE_SPEED * y_dir), PHASE_ORB_MOVE_SPEED_DECAY)

struct PhaseOrb : GameObject {

	int32 x_dir;
	int32 y_dir;

    PhaseOrb();
	PhaseOrb(const PhaseOrb& other);
	~PhaseOrb();

	PhaseOrb& operator =(const PhaseOrb& other);

	void update(const RoomBounds&                              room_bounds,
				bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
				const bn::regular_bg_ptr&                      bg_ptr, 
                const bn::span<const bn::regular_bg_map_cell>& cells,
                const bn::regular_bg_item&                     bg_item,
				const bn::camera_ptr&                          camera) override;

};

#endif