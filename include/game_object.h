#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

// Butano
#include "bn_optional.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_camera_ptr.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_map_cell.h"
#include "bn_regular_bg_map_cell_info.h"

// My Libs
#include "room_bounds.h"
#include "utility.h"
#include "physics.h"
#include "collider.h"

// Assets
#include "bn_sprite_items_game_object.h"
#include "bn_sprite_palette_items_default_sprite_palette.h"
#include "bn_sprite_palette_items_white_sprite_palette.h"

#define MAX_ANIM_FRAMES 16

///////////////////////
// Struct GameObject //
///////////////////////

#define GAME_OBJECT_COLLIDER_WIDTH  8
#define GAME_OBJECT_COLLIDER_HEIGHT 8

#define GAME_OBJECT_IGNORE_OWP_FRAMES 3

enum ObjectType 
{
	PLAYER = 0,
	TEST_ENEMY,
	DEVIL_PLATFORM,
	ANGEL_PLATFORM,
	SCYTHE_PLATFORM,
	EXIT,
	HITBOX_SCYTHE_1,
	HITBOX_SCYTHE_2,
	HITBOX_SCYTHE_3
};

struct GameObject {

	ObjectType object_type;
	uint32     object_id;

	bn::optional<bn::sprite_ptr>                             sprite_ptr;
	bn::optional<bn::sprite_animate_action<MAX_ANIM_FRAMES>> animate_action_ptr;
	bn::optional<bn::sprite_palette_ptr>                     sprite_palette_ptr;

	RigidBody rigidbody;

	Collider collider;
	Collider collider_x_axis;
	Collider collider_y_axis;
	int32 collider_offset_x = 0;
	int32 collider_offset_y = 0;

	Direction dir;

	bool inactive = false;

	bool received_platform_force = false;

	void applyForces();

	GameObject();
	GameObject(const GameObject& other);
	virtual ~GameObject();

	virtual void update(RoomBounds room_bounds,
	                    bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects,
						bn::regular_bg_ptr                         bg_ptr, 
                        bn::span<const bn::regular_bg_map_cell>    cells,
                        bn::regular_bg_item                        bg_item,
						bn::camera_ptr                             camera) = 0;
	
	void draw();
	void setCamera(const bn::camera_ptr& camera);
	bn::fixed x() const;
	bn::fixed y() const;
	bn::fixed_point pos() const;
	void setX(bn::fixed new_x);
	void setY(bn::fixed new_y);
	void setPos(bn::fixed new_x, bn::fixed new_y);
	void setPos(bn::fixed_point new_pos);

	int32 getTileAtBGIndex(uint32 x, uint32 y, 
						   bn::regular_bg_ptr                      bg_ptr, 
						   bn::span<const bn::regular_bg_map_cell> cells,
						   bn::regular_bg_item                     bg_item) const;


};

# endif
