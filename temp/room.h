#ifndef ROOM_H
#define ROOM_H

// Butano
#include "bn_regular_bg_ptr.h"
#include "bn_camera_ptr.h"

// Game Files
#include "collider.h"

// Assets
#include "bn_regular_bg_items_default_room_1_bg.h"
#include "bn_regular_bg_items_default_room_2_bg.h"

enum RoomType {
	DEFAULT_ROOM_1 = 0,
	DEFAULT_ROOM_2 = 1,
};

#define MAX_ROOM_COLLIDERS 20

struct Room {
	bn::optional<bn::regular_bg_ptr> bg_ptr;
	Collider colliders[MAX_ROOM_COLLIDERS];
	RoomType current_type;
	// Exits
	// Room bounds (for camera)

	Room(RoomType type);
	RoomType getType();
	void setBG(RoomType type);
	void setCamera(const bn::camera_ptr& camera);
};

#endif
