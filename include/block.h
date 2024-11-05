#ifndef BLOCK_H
#define BLOCK_H

// Butano
#include "bn_optional.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"
#include "bn_camera_ptr.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_regular_bg_tiles_ptr.h"

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
	void draw() override;
	void setCamera(const bn::camera_ptr& camera) override;
	bn::fixed x() const override;
	bn::fixed y() const override;
	bn::fixed_point pos() const override;
	void setX(bn::fixed new_x) override;
	void setY(bn::fixed new_y) override;
	void setPos(bn::fixed new_x, bn::fixed new_y) override;
	void setPos(bn::fixed_point new_pos) override;
};

#endif
