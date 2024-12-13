#ifndef ANGEL_PLATFORM_H
#define ANGEL_PLATFORM_H

// Butano
#include "bn_optional.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_math.h"

// My Libs
#include "utility.h"
#include "physics.h"
#include "collider.h"
#include "room.h"

// Assets
#include "bn_sprite_items_angel_platform.h"

// Base Class
#include "game_object.h"

//////////////////////////
// Struct AngelPlatform //
//////////////////////////

// The angel platform can carry the player. 

// -The angel platform must never push, raise, lower or otherwise "pinch" the player 
//  into another collidable surface, including another platform. 
//  This will always result in a player death.

// -Carrying the player into a collidable surface is OK :) 

// -Platform movement is 8 directional.

// -This object is updated every OTHER frame (30 fps)

#define ANGEL_PLATFORM_COLLIDER_WIDTH  32
#define ANGEL_PLATFORM_COLLIDER_HEIGHT 1

#define ANGEL_PLATFORM_SPEED 1
#define ANGEL_PLATFORM_DECAY 1

struct AngelPlatform : GameObject {

    bn::fixed  speed;
    bn::point  target;
    bn::point  next_target;
	int32 	   update_counter;

    AngelPlatform(bn::point p1, bn::point p2);
    ~AngelPlatform();

    void update(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects,
				bn::regular_bg_ptr                         bg_ptr, 
                bn::span<const bn::regular_bg_map_cell>    cells,
                bn::regular_bg_item                        bg_item,
                bn::camera_ptr                             camera) override;

};

#endif