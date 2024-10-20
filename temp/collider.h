#ifndef COLLIDER_H
#define COLLIDER_H

// Butano
#include "bn_point.h"
#include "bn_fixed_fwd.h"
#include "bn_fixed_point_fwd.h"
#include "bn_optional.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"

#include "bn_camera_ptr.h"

// Assets
#include "bn_sprite_items_debug_rect_8_sprite.h"
#include "bn_sprite_items_debug_rect_16_sprite.h"
#include "bn_sprite_items_debug_rect_32_sprite.h"

#define COLLIDER_Z_ORDER -999

enum ColliderSize{
	COLLIDER_8x = 8,
	COLLIDER_16x = 16,
	COLLIDER_32x = 32
};

struct Collider{
	ColliderSize size;
	bn::fixed_point p1;
	bn::fixed_point p2;
	bn::fixed_point p3;
	bn::fixed_point p4;
	bn::optional<bn::sprite_ptr> sprite_ptr;

	Collider();
	Collider(bn::fixed _origin_x,
		 bn::fixed _origin_y,
		 ColliderSize _size);
	bn::fixed x() const;
	bn::fixed y() const;
	bn::fixed_point position() const;
	void setX(bn::fixed new_x);
	void setY(bn::fixed new_y);
	void setPos(bn::fixed new_x, bn::fixed new_y);
	void setPos(bn::fixed_point new_pos);
	void updatePoints();
	void setCamera(const bn::camera_ptr& camera);
	bool pointIsColliding(bn::fixed_point test_p) const;
        bool willCollide(const Collider& target, bn::fixed_point dir) const;
};

#endif
