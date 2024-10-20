#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

// Butano
#include "bn_optional.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"
#include "bn_camera_ptr.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"

// My Libs
#include "utility.h"
#include "collider.h"

#define MAX_ANIM_FRAMES 16

///////////////////////
// Struct GameObject //
///////////////////////

struct GameObject {
	bn::optional<bn::sprite_ptr> sprite_ptr;
	bn::optional<bn::sprite_animate_action<MAX_ANIM_FRAMES>> animate_action_ptr;
	Collider* collider_p = NULL;
	
	virtual void update(GameObject** game_objects_p, uint8 game_objects_size) = 0;
	virtual void draw();
	virtual void setCamera(const bn::camera_ptr& camera);
	bn::fixed x() const;
	bn::fixed y() const;
	bn::fixed_point pos() const;
	virtual void setX(bn::fixed new_x);
	virtual void setY(bn::fixed new_y);
	virtual void setPos(bn::fixed new_x, bn::fixed new_y);
	virtual void setPos(bn::fixed_point new_pos);
	virtual ~GameObject() = default;
};

# endif
