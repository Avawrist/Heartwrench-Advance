#ifndef THORN_COLUMN_H
#define THORN_COLUMN_H

// Butano
#include "bn_optional.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_math.h"

// My Libs
#include "utility.h"
#include "collider.h"

// Base Class
#include "enemy.h"

////////////////////////
// Struct ThornColumn //
////////////////////////

#define THORN_COLUMN_COLLIDER_WIDTH    18
#define THORN_COLUMN_COLLIDER_HEIGHT   64
#define THORN_COLUMN_COLLIDER_OFFSET_X 0
#define THORN_COLUMN_COLLIDER_OFFSET_Y 0

#define THORN_COLUMN_HITPOINTS 0

struct ThornColumn : Enemy {

    ThornColumn();
    ThornColumn(const ThornColumn& other);
    ~ThornColumn();

    ThornColumn& operator =(const ThornColumn& other);

    void update(const RoomBounds&                                 room_bounds,
                bn::vector<GameObject*, MAX_GAME_OBJECTS>&        game_objects,
				const bn::regular_bg_ptr&                         bg_ptr, 
                const bn::span<const bn::regular_bg_map_cell>&    cells,
                const bn::regular_bg_item&                        bg_item,
                const bn::camera_ptr&                             camera) override;

};

#endif