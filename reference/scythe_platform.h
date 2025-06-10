#ifndef SCYTHE_PLATFORM_H
#define SCYTHE_PLATFORM_H

// Butano
#include "bn_keypad.h"
#include "bn_math.h"

// My Libs
#include "math.h"

// Base Class
#include "game_object.h"

////////////////////////////
// Struct ScythePlatform //
////////////////////////////

// The scythe platform can lodge in blocks and carry the player. 

// -The scythe platform must never push, raise, lower or otherwise "pinch" the player 
//  into another collidable surface, including another platform. 
//  This will always result in a player death.

// -Carrying the player into a collidable surface is OK :) 

// -Scythe movement is 2 directional.

/*
enum ScytheState {
	SCYTHE_PLATFORM_THROWN = 0,
};
*/

#define SCYTHE_PLATFORM_COLLIDER_WIDTH  24
#define SCYTHE_PLATFORM_COLLIDER_HEIGHT 2

#define SCYTHE_MIN_STRETCH_V 0.5
#define SCYTHE_MAX_STRETCH_V 2.0
#define SCYTHE_MIN_STRETCH_H 0.5
#define SCYTHE_MAX_STRETCH_H 2.0

#define SCYTHE_PLATFORM_DRIFT_DOWN_Y 2
#define SCYTHE_PLATFORM_DRIFT_UP_Y  -2
#define SCYTHE_PLATFORM_DRIFT_DECAY  0.5

#define SCYTHE_PLAYER_X_OFFSET 20 
#define SCYTHE_PLAYER_Y_OFFSET 0

#define SCYTHE_PLATFORM_THROW_X_FORCE 8
#define SCYTHE_PLATFORM_DECAY         0.1

#define SCYTHE_PLATFORM_GRAVITY 1
#define SCYTHE_PLATFORM_GRAVITY_DECAY 1

#define SCYTHE_PLATFORM_THROW_FORCE      Force(bn::fixed_point_t<12>(SCYTHE_PLATFORM_THROW_X_FORCE * x_dir, 0), SCYTHE_PLATFORM_DECAY)
#define SCYTHE_PLATFORM_DRIFT_DOWN_FORCE Force(bn::fixed_point_t<12>(0, SCYTHE_PLATFORM_DRIFT_DOWN_Y), SCYTHE_PLATFORM_DRIFT_DECAY)
#define SCYTHE_PLATFORM_DRIFT_UP_FORCE   Force(bn::fixed_point_t<12>(0, SCYTHE_PLATFORM_DRIFT_UP_Y), SCYTHE_PLATFORM_DRIFT_DECAY)
#define SCYTHE_PLATFORM_GRAVITY_FORCE    Force(bn::fixed_point_t<12>(0, SCYTHE_PLATFORM_GRAVITY), SCYTHE_PLATFORM_GRAVITY_DECAY)

struct ScythePlatform : GameObject {

	bool  player_was_riding;
    int32 update_counter;

    ScythePlatform(XDirection _x_dir, bn::fixed_point _p);
    ~ScythePlatform();

    void update(const RoomBounds&                              room_bounds,
                bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
				const bn::regular_bg_ptr&                      bg_ptr, 
                const bn::span<const bn::regular_bg_map_cell>& cells,
                const bn::regular_bg_item&                     bg_item,
				const bn::camera_ptr&                          camera) override;
	
};

#endif