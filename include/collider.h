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

// Assets
#include "bn_sprite_items_collider_8.h"
#include "bn_sprite_items_collider_16.h"
#include "bn_sprite_items_collider_32.h"

#define MAX_ANIM_FRAMES 16

enum ColliderSize{
	COLLIDER_8  = 8,
	COLLIDER_16 = 16,
	COLLIDER_32 = 32
};

struct Collider {
	
	ColliderSize size;
	bn::fixed_point position;
	bn::fixed_point p1;
	bn::fixed_point p4;

	Collider();
	Collider(bn::fixed origin_x,
		 	 bn::fixed origin_y,
		 	 ColliderSize _size);
	
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
