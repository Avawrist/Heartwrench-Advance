#ifndef PHASE_ORB_H
#define PHASE_ORB_H

// Butano
#include "bn_math.h"

// Assets
#include "bn_sprite_items_phase_orb_up.h"
#include "bn_sprite_items_phase_orb_down.h"
#include "bn_sprite_items_phase_orb_left.h"
#include "bn_sprite_items_phase_orb_right.h"

// Base Class
#include "game_object.h"

/////////////////////
// PhaseOrb Macros //
/////////////////////

#define PHASE_ORB_Z_ORDER 1

#define PHASE_ORB_COLLIDER_WIDTH  16
#define PHASE_ORB_COLLIDER_HEIGHT 16

#define PHASE_ORB_COLLIDER_OFFSET_X 0
#define PHASE_ORB_COLLIDER_OFFSET_Y 0

#define PHASE_DISTANCE 128

///////////////////////
// Struct PhaseOrbUp //
///////////////////////

struct PhaseOrbUp : GameObject {

	bn::fixed_point phase_destination;

    PhaseOrbUp();
	PhaseOrbUp(const PhaseOrbUp& other);
	~PhaseOrbUp();

	PhaseOrbUp& operator =(const PhaseOrbUp& other);

	void update(const RoomBounds&                              room_bounds,
				bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
				const bn::regular_bg_ptr&                      bg_ptr, 
                const bn::span<const bn::regular_bg_map_cell>& cells,
                const bn::regular_bg_item&                     bg_item,
				const bn::camera_ptr&                          camera) override;

};

/////////////////////////
// Struct PhaseOrbDown //
/////////////////////////

struct PhaseOrbDown : GameObject {

	bn::fixed_point phase_destination;

    PhaseOrbDown();
	PhaseOrbDown(const PhaseOrbDown& other);
	~PhaseOrbDown();

	PhaseOrbDown& operator =(const PhaseOrbDown& other);

	void update(const RoomBounds&                              room_bounds,
				bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
				const bn::regular_bg_ptr&                      bg_ptr, 
                const bn::span<const bn::regular_bg_map_cell>& cells,
                const bn::regular_bg_item&                     bg_item,
				const bn::camera_ptr&                          camera) override;

};

/////////////////////////
// Struct PhaseOrbLeft //
/////////////////////////

struct PhaseOrbLeft : GameObject {

	bn::fixed_point phase_destination;

    PhaseOrbLeft();
	PhaseOrbLeft(const PhaseOrbLeft& other);
	~PhaseOrbLeft();

	PhaseOrbLeft& operator =(const PhaseOrbLeft& other);

	void update(const RoomBounds&                              room_bounds,
				bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
				const bn::regular_bg_ptr&                      bg_ptr, 
                const bn::span<const bn::regular_bg_map_cell>& cells,
                const bn::regular_bg_item&                     bg_item,
				const bn::camera_ptr&                          camera) override;

};

//////////////////////////
// Struct PhaseOrbRight //
//////////////////////////

struct PhaseOrbRight : GameObject {

	bn::fixed_point phase_destination;

    PhaseOrbRight();
	PhaseOrbRight(const PhaseOrbRight& other);
	~PhaseOrbRight();

	PhaseOrbRight& operator =(const PhaseOrbRight& other);

	void update(const RoomBounds&                              room_bounds,
				bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
				const bn::regular_bg_ptr&                      bg_ptr, 
                const bn::span<const bn::regular_bg_map_cell>& cells,
                const bn::regular_bg_item&                     bg_item,
				const bn::camera_ptr&                          camera) override;

};

#endif