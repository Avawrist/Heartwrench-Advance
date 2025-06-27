#ifndef PHASE_ORB_H
#define PHASE_ORB_H

// Butano
#include "bn_math.h"

// Base Class
#include "game_object.h"

/////////////////////
// PhaseOrb Macros //
/////////////////////

#define PHASE_ORB_Z_ORDER 1

#define PHASE_ORB_COLLIDER_WIDTH  10
#define PHASE_ORB_COLLIDER_HEIGHT 10

#define PHASE_ORB_COLLIDER_OFFSET_X 0
#define PHASE_ORB_COLLIDER_OFFSET_Y 0

#define PHASE_DISTANCE 156

struct PhaseOrb : GameObject {

	/////////////////////
	// Struct PhaseOrb //
	/////////////////////

	bn::fixed_point phase_destination;

    PhaseOrb();
	PhaseOrb(const PhaseOrb& other);
	~PhaseOrb();

	PhaseOrb& operator =(const PhaseOrb& other);

	virtual void setDestination() = 0;

	//////////////////////////////
	// State Function Overrides //
	//////////////////////////////

	void updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&        game_objects,
							const bn::regular_bg_ptr&                         bg_ptr, 
							const bn::span<const bn::regular_bg_map_cell>&    cells,
							const bn::regular_bg_item&                        bg_item,
							const bn::camera_ptr&                             camera) override;

	/////////////////////////
	// Collision Overrides //
	/////////////////////////

	void resolveCollision(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                          const bn::regular_bg_ptr&                      bg_ptr, 
                          const bn::span<const bn::regular_bg_map_cell>& cells,
                          const bn::regular_bg_item&                     bg_item) override;

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