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

#include "bn_keypad.h"
#include "bn_profiler.h"

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

#define GAME_OBJECT_Z_ORDER 0

#define GAME_OBJECT_COLLIDER_WIDTH  8
#define GAME_OBJECT_COLLIDER_HEIGHT 8

#define GAME_OBJECT_IGNORE_OWP_FRAMES 3

// The index order here needs to align with the object tile index order in 
// Aseprite.

enum ObjectType 
{
	NO_TYPE = 0,
	// Level Objects
	TILE_PASSAGE,
	PHASE_ORB_UP,
	PHASE_ORB_DOWN,
	PHASE_ORB_LEFT,
	PHASE_ORB_RIGHT,
	// Level Enemies
	GROUND_GHOUL,
	CEILING_GHOUL,
	WALL_LEFT_GHOUL,
	WALL_RIGHT_GHOUL,
	TEST_ENEMY,
	// Special Objects
	DEVIL_PLATFORM,
	ANGEL_PLATFORM,
	SCYTHE_PLATFORM,
	HITBOX_SCYTHE_GROUND_1,
	HITBOX_SCYTHE_GROUND_2,
	HITBOX_SCYTHE_GROUND_3,
	HITBOX_SCYTHE_AIR_1,
	PLAYER,
};

struct GameObject {

	ObjectType object_type;
	int32      object_id;

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

	bool is_inactive   = false;
	bool is_dead       = false;
	bool is_persistent = false;

	bool received_platform_force = false;

	void applyForces();

	GameObject();
	GameObject(const GameObject& other);
	virtual ~GameObject();

	GameObject& operator =(const GameObject& other);

	virtual void update(const RoomBounds& 							   room_bounds,
	                    bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
						const bn::regular_bg_ptr&                      bg_ptr, 
                        const bn::span<const bn::regular_bg_map_cell>& cells,
                        const bn::regular_bg_item&                     bg_item,
						const bn::camera_ptr&                          camera) = 0;
	
	virtual void draw();
	virtual void setCamera(const bn::camera_ptr& camera);
	bn::fixed x() const;
	bn::fixed y() const;
	bn::fixed_point pos() const;
	void setX(bn::fixed new_x);
	void setY(bn::fixed new_y);
	void setPos(bn::fixed new_x, bn::fixed new_y);
	void setPos(bn::fixed_point new_pos);
	void updateInactiveState(const bn::camera_ptr& camera);

};

///////////////////////////
// Struct UnloadedObject //
///////////////////////////

#define UNLOADED_OBJECT_STATE_UNLOADED -1
#define UNLOADED_OBJECT_STATE_DEAD     -2

struct UnloadedObject
{
    bn::point  room_pos;
    ObjectType object_type;
	int32      loaded_instance_id;
	bool       is_persistent = false;

    UnloadedObject();
    UnloadedObject(bn::point _room_pos, ObjectType _object_type);
    UnloadedObject(const UnloadedObject& other);
    ~UnloadedObject();

    void operator =(const UnloadedObject& other);
};

# endif
