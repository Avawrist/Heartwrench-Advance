#ifndef COLLIDER_H
#define COLLIDER_H

// Butano
#include "bn_optional.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"
#include "bn_camera_ptr.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"

// My Libs
#include "utility.h"

#define MAX_ANIM_FRAMES 16

struct Collider {
	
	uint32 width;
	uint32 height;
	bn::fixed_point position;
	bn::fixed_point p1;
	bn::fixed_point p4;

	Collider();
	Collider(bn::fixed origin_x,
		 	 bn::fixed origin_y,
		 	 uint32 _width,
			 uint32 _height);
	Collider(const Collider& other);
	
	void update();
    void draw();
	void setCamera(const bn::camera_ptr& camera);
	
	bn::fixed x() const;
	bn::fixed y() const;
	bn::fixed_point pos() const;
	void setX(bn::fixed new_x);
	void setY(bn::fixed new_y);
	void setPos(bn::fixed new_x, bn::fixed new_y);
	void setPos(bn::fixed_point new_pos);
	
	bool isCollision(bn::fixed_point target_point);
	bool isCollision(Collider& target);
};

#endif
