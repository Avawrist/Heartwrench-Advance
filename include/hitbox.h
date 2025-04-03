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

#define HITBOX_KNOCKBACK_FORCE Force(bn::fixed_point_t<12>(x_knockback * dir, y_knockback), knockback_decay)

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

    Hitbox(bn::point  pos,
           int32      _hitstun_frames,
           int32      _lifespan_frames,
           int32      _x_knockback,
           int32      _y_knockback,
           int32      _knockback_decay,
           int32      _width,
           int32      _height,
           Direction  _dir,
           ObjectType _type);
    Hitbox(const Hitbox& other);
    ~Hitbox();

    Hitbox& operator =(const Hitbox& other);

    void update(const RoomBounds&                              room_bounds,
                bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                const bn::regular_bg_ptr&                      bg_ptr,
                const bn::span<const bn::regular_bg_map_cell>& cells,
                const bn::regular_bg_item&                     bg_item,
                const bn::camera_ptr&                          camera) override;

};

#endif