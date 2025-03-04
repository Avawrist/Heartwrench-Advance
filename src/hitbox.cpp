#include "hitbox.h"

Hitbox::Hitbox(bn::point pos,
       int32      _hitstun_frames,
       int32      _lifespan_frames,
       int32      _x_knockback,
       int32      _y_knockback,
       int32      _knockback_decay,
       int32      _width,
       int32      _height,
       Direction  _dir,
       ObjectType _type)
{

    // Reset Variables //
    sprite_ptr.reset();
    animate_action_ptr.reset();

    // Init Variables //
	object_type        = _type;
	dir                = _dir;
    sprite_ptr         = bn::sprite_items::hitbox.create_sprite(pos.x(), pos.y());
    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  2,
								  								  bn::sprite_items::hitbox.tiles_item(),
								  								  0,
								  								  0);

	collider = Collider(pos.x(), pos.y(), _width, _height);
    collider_offset_x = 0;
	collider_offset_y = 0;

    hitstun_frames         = _hitstun_frames;
    lifespan_frames        = _lifespan_frames;
    current_lifespan_frame = lifespan_frames;
    x_knockback            = _x_knockback;
    y_knockback            = _y_knockback;
    knockback_decay        = _knockback_decay;
    width                  = _width;
    height                 = _height;

    dir                    = _dir;

}

Hitbox::~Hitbox()
{

}

void Hitbox::update(RoomBounds                                 room_bounds,
                    Collider**                                 tile_colliders,
                    bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects,
                    bn::regular_bg_ptr                         bg_ptr, 
                    bn::span<const bn::regular_bg_map_cell>    cells,
                    bn::regular_bg_item                        bg_item,
                    bn::camera_ptr                             camera)
{

    //////////////////////////
    // Udpate Frame Counter //
    //////////////////////////

    current_lifespan_frame--;
    current_lifespan_frame = clamp(0, lifespan_frames, current_lifespan_frame);
    
    if(current_lifespan_frame <= 0) {inactive = true;}

}