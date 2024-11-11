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

enum ObjectType 
{
	PLAYER = 0,
	BLOCK,
	ONE_WAY_BLOCK,
};

struct GameObject {

	virtual void update(GameObject** game_objects_p, uint32 game_objects_size) = 0;
	virtual void draw() = 0;
	virtual void setCamera(const bn::camera_ptr& camera) = 0;
	virtual bn::fixed x() const = 0;
	virtual bn::fixed y() const = 0;
	virtual bn::fixed_point pos() const = 0;
	virtual void setX(bn::fixed new_x) = 0;
	virtual void setY(bn::fixed new_y) = 0;
	virtual void setPos(bn::fixed new_x, bn::fixed new_y) = 0;
	virtual void setPos(bn::fixed_point new_pos) = 0;
	virtual ~GameObject() = default;

	ObjectType object_type;
	Collider*  collider_ptr = NULL;
	int32 	   collider_offset_x;
	int32      collider_offset_y;
	
};

# endif
