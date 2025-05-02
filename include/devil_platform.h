#ifndef DEVIL_PLATFORM_H
#define DEVIL_PLATFORM_H

// Butano
#include "bn_math.h"

// Base Class
#include "game_object.h"
#include "player.h"

//////////////////////////
// Struct DevilPlatform //
//////////////////////////

// The devil platform can push, raise, lower and carry the player. 

// -The moving platform must never push, raise, lower or otherwise "pinch" the player 
//  into another collidable surface, including another platform. 
//  This will always result in a player death.

// -Carrying the player into a collidable surface is OK :) 

// -Platform movement is 8 directional.

// -This object is updated every OTHER frame (30 fps)

#define DEVIL_PLATFORM_COLLIDER_WIDTH  32
#define DEVIL_PLATFORM_COLLIDER_HEIGHT 8

#define DEVIL_PLATFORM_SPEED 1
#define DEVIL_PLATFORM_DECAY 1

struct DevilPlatform : GameObject {

    bn::fixed  speed;
    bn::point  target;
    bn::point  next_target;
	int32 	   update_counter;

    DevilPlatform(bn::point p1, bn::point p2);
    ~DevilPlatform();

    void update(const RoomBounds&                              room_bounds,
                bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
				const bn::regular_bg_ptr&                      bg_ptr, 
                const bn::span<const bn::regular_bg_map_cell>& cells,
                const bn::regular_bg_item&                     bg_item,
                const bn::camera_ptr&                          camera) override;
	
};

#endif