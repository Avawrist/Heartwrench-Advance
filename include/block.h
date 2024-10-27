#ifndef BLOCK_H
#define BLOCK_H

// Butano
#include "bn_optional.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"
#include "bn_camera_ptr.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"

// My Libs
#include "utility.h"
#include "game_object.h"

// Assets
#include "bn_sprite_items_block.h"

//////////////////
// Struct Block //
//////////////////

struct Block : GameObject {
    Block();
	~Block() override;
	void update(GameObject** game_objects_p,
		    	uint32 game_objects_size) override;
};

#endif
