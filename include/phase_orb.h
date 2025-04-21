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

#define PHASE_DISTANCE 256

/////////////////////
// Struct PhaseOrb //
/////////////////////

struct PhaseOrb : GameObject {

	bn::fixed_point phase_destination;

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

	virtual void setDestination() = 0;
};

//////////////////////////////
// Struct PhaseOrb Children //
//////////////////////////////

struct PhaseOrbUp : PhaseOrb 
{
	PhaseOrbUp();
	void setDestination() override {phase_destination = bn::fixed_point(x(), y() - PHASE_DISTANCE);};
};

struct PhaseOrbDown : PhaseOrb 
{
	PhaseOrbDown();
	void setDestination() override {phase_destination = bn::fixed_point(x(), y() + PHASE_DISTANCE);};
};

struct PhaseOrbLeft : PhaseOrb 
{
	PhaseOrbLeft();
	void setDestination() override {phase_destination = bn::fixed_point(x() - PHASE_DISTANCE, y());};
};

struct PhaseOrbRight : PhaseOrb 
{
	PhaseOrbRight();
	void setDestination() override {phase_destination = bn::fixed_point(x() + PHASE_DISTANCE, y());};
};

#endif