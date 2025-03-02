#ifndef PHYSICS_H
#define PHYSICS_H

// Butano
#include "bn_vector.h"
#include "bn_optional.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"

#include "bn_log.h"

// My Libs
#include "utility.h"
#include "math.h"

//////////////////
// Struct Force //
//////////////////

struct Force {
	bn::fixed_point force;
	bn::fixed_point decayed_force;
	bn::fixed timer;
	bn::fixed decay_rate;

	Force();
	Force(bn::fixed_point _force, bn::fixed _decay_rate);
	void update();
	const bn::fixed x();
	const bn::fixed y();
    bool isDecayed();
};

//////////////////////
// Struct RigidBody //
//////////////////////

#define MAX_FORCES 10

struct RigidBody {
	bn::vector<Force*, MAX_FORCES> forces;
	bn::fixed_point normalized_dir;
	bn::fixed_point final_dir;

	RigidBody();
	RigidBody(const RigidBody& other);

	RigidBody& operator =(const RigidBody& other);

	void   applyDecay();
    uint32 addForce(Force* force_p);
	void   removeForces();
};

#endif
