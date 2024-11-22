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
#include "physics.h"
#include "collider.h"
#include "room.h"

// Assets
#include "bn_sprite_items_game_object.h"

#define MAX_ANIM_FRAMES 16

///////////////////////
// Struct GameObject //
///////////////////////

#define GAME_OBJECT_COLLIDER_WIDTH  8
#define GAME_OBJECT_COLLIDER_HEIGHT 8

enum ObjectType 
{
	PLAYER = 0,
	DEVIL_PLATFORM,
	ANGEL_PLATFORM,
	SCYTHE_PLATFORM,
};

struct GameObject {

	ObjectType object_type;

	bn::optional<bn::sprite_ptr> sprite_ptr;
	bn::optional<bn::sprite_animate_action<MAX_ANIM_FRAMES>> animate_action_ptr;

	Collider*  collider_ptr  = NULL;
	RigidBody* rigidbody_ptr = NULL;

	int32 collider_offset_x = 0;
	int32 collider_offset_y = 0;

	bn::fixed_point applyForces();

	GameObject();
	virtual ~GameObject();

	virtual void update(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects,
						const Room& room, 
						const bn::camera_ptr& camera) = 0;
	
	void draw();
	void setCamera(const bn::camera_ptr& camera);
	bn::fixed x() const;
	bn::fixed y() const;
	bn::fixed_point pos() const;
	void setX(bn::fixed new_x);
	void setY(bn::fixed new_y);
	void setPos(bn::fixed new_x, bn::fixed new_y);
	void setPos(bn::fixed_point new_pos);

};

# endif
