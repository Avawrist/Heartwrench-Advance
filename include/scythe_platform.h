#ifndef SCYTHE_PLATFORM_H
#define SCYTHE_PLATFORM_H

// Butano
#include "bn_keypad.h"
#include "bn_math.h"

// Assets
#include "bn_sprite_items_scythe_platform.h"

// Base Class
#include "game_object.h"

///////////////////////////
// Struct ScythePlatform //
///////////////////////////

// The scythe platform can lodge in blocks and carry the player. 

// -The angel platform must never push, raise, lower or otherwise "pinch" the player 
//  into another collidable surface, including another platform. 
//  This will always result in a player death.

// -Carrying the player into a collidable surface is OK :) 

// -Scythe movement is 2 directional.

enum ScytheState {
	STATE_IDLE = 0,
	STATE_THROWN,
	STATE_STUCK_IN_OBJECT,
	STATE_STUCK_IN_MAP,
	STATE_RETURNING,
};

#define SCYTHE_PLATFORM_COLLIDER_WIDTH  24
#define SCYTHE_PLATFORM_COLLIDER_HEIGHT 2

#define SCYTHE_PLATFORM_SPEED        4
#define SCYTHE_PLATFORM_SLOWED_SPEED 1
#define SCYTHE_PLATFORM_DECAY        1

#define SCYTHE_PLAYER_X_OFFSET 20 
#define SCYTHE_PLAYER_Y_OFFSET 0

#define SCYTHE_RETURN_COOLDOWN_FRAMES 3

#define SCYTHE_PLATFORM_X_FORCE new Force(bn::fixed_point_t<12>(speed * dir, 0), SCYTHE_PLATFORM_DECAY)

struct ScythePlatform : GameObject {

	ScytheState state;
    bn::fixed speed;

	int32 return_cooldown;

    ScythePlatform(Direction _dir, bn::fixed_point _p);
    ~ScythePlatform();

    void update(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects,
				bn::regular_bg_ptr                        bg_ptr, 
                bn::span<const bn::regular_bg_map_cell>   cells,
                bn::regular_bg_item                       bg_item) override;
	
};

#endif