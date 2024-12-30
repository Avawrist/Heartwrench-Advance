#ifndef HITBOX_H
#define HITBOX_H

// Butano
#include "bn_math.h"

// Assets
#include "bn_sprite_items_hitbox.h"

// Base Class
#include "game_object.h"

///////////////////
// Struct Hitbox //
///////////////////

#define HITBOX_KNOCKBACK_FORCE new Force(bn::fixed_point_t<12>(x_knockback * dir, y_knockback), knockback_decay)

struct Hitbox : GameObject
{

    int32 hitstun_frames;
    int32 lifespan_frames;
    int32 current_lifespan_frame;
    int32 x_knockback;
    int32 y_knockback;
    int32 knockback_decay;
    int32 width;
    int32 height;

    Hitbox(bn::point pos,
           int32     _hitstun_frames,
           int32     _lifespan_frames,
           int32     _x_knockback,
           int32     _y_knockback,
           int32     _knockback_decay,
           int32     _width,
           int32     _height,
           Direction _dir);
    ~Hitbox();

    void update(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects,
                bn::regular_bg_ptr                         bg_ptr,
                bn::span<const bn::regular_bg_map_cell>    cells,
                bn::regular_bg_item                        bg_item,
                bn::camera_ptr                             camera) override;

};

#endif