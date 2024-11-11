#ifndef ONE_WAY_BLOCK_H
#define ONE_WAY_BLOCK_H

// Butano
#include "bn_optional.h"
#include "bn_camera_ptr.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"

// My Libs
#include "utility.h"
#include "game_object.h"

//////////////////
// Struct Block //
//////////////////

#define ONEWAYBLOCK_COLLIDER_WIDTH  8
#define ONEWAYBLOCK_COLLIDER_HEIGHT 1

struct OneWayBlock : GameObject {

    OneWayBlock(uint32 width);
	~OneWayBlock() override;

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