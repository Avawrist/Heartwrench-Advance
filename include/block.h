#ifndef BLOCK_H
#define BLOCK_H

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

#define BLOCK_COLLIDER_HEIGHT 8

struct Block : GameObject {

    Block(uint32 width);
	~Block() override;

	void update(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects) override;
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
