#include "hitbox.h"

Hitbox::Hitbox(bn::point pos,
       int32      _hitstop_frames,
       int32      _screenshake_frames,
       int32      _lifespan_frames,
       int32      _x_knockback,
       int32      _y_knockback,
       int32      _knockback_decay,
       int32      _width,
       int32      _height,
       int32      _damage,
       XDirection _x_dir,
       YDirection _y_dir,
       ObjectType _type,
       ScreenShakeSeverity _screenshake_severity)
{

    // Reset Variables //
    sprite_ptr.reset();
    default_palette_ptr.reset();
    animate_action_ptr.reset();

    // Init Variables //
	object_type        = _type;
    
    sprite_ptr         = bn::sprite_items::hitbox.create_sprite(pos.x(), pos.y());
    sprite_ptr->set_visible(false);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  2,
								  								  bn::sprite_items::hitbox.tiles_item(),
								  								  0,
								  								  0);

	collider = Collider(pos.x(), pos.y(), _width, _height);
    collider_offset_x = 0;
	collider_offset_y = 0;

    hitstop_frames         = _hitstop_frames;
    screenshake_frames     = _screenshake_frames;
    lifespan_frames        = _lifespan_frames;
    current_lifespan_frame = lifespan_frames;
    x_knockback            = _x_knockback;
    y_knockback            = _y_knockback;
    knockback_decay        = _knockback_decay;
    width                  = _width;
    height                 = _height;
    damage                 = _damage;

    x_dir                  = _x_dir;
    y_dir                  = _y_dir;

    screenshake_severity = _screenshake_severity;

}

Hitbox::Hitbox(const Hitbox& other) : GameObject(other)
{
    hitstop_frames         = other.hitstop_frames;
    screenshake_frames     = other.screenshake_frames;
    lifespan_frames        = other.lifespan_frames;
    current_lifespan_frame = other.current_lifespan_frame;
    x_knockback            = other.x_knockback;
    y_knockback            = other.y_knockback;
    knockback_decay        = other.knockback_decay;
    width                  = other.width;
    height                 = other.height;
    damage                 = other.damage;
    screenshake_severity   = other.screenshake_severity;
}

Hitbox::~Hitbox()
{

}

Hitbox& Hitbox::operator =(const Hitbox& other)
{
    hitstop_frames         = other.hitstop_frames;
    screenshake_frames     = other.screenshake_frames;
    lifespan_frames        = other.lifespan_frames;
    current_lifespan_frame = other.current_lifespan_frame;
    x_knockback            = other.x_knockback;
    y_knockback            = other.y_knockback;
    knockback_decay        = other.knockback_decay;
    width                  = other.width;
    height                 = other.height;
    damage                 = other.damage;
    screenshake_severity   = screenshake_severity;

    return *this;
}

void Hitbox::update(const RoomBounds&                              room_bounds,
                    bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                    const bn::regular_bg_ptr&                      bg_ptr, 
                    const bn::span<const bn::regular_bg_map_cell>& cells,
                    const bn::regular_bg_item&                     bg_item,
                    const bn::camera_ptr&                          camera)
{

    //////////////////////
    // Collision Events //
    //////////////////////

    Collider other_collider;

    for(int32 i = 0; i < game_objects.size(); i++)
    {
        other_collider = game_objects.at(i)->collider;

        switch(game_objects.at(i)->object_type)
        {
                
            case CEILING_GHOUL:
            case GROUND_GHOUL:
            case WALL_LEFT_GHOUL:
            case WALL_RIGHT_GHOUL:
                    
                if(collider.isCollision(other_collider))
                {applyHit(*(game_objects.at(i)));}

            break;

            case PHASE_ORB_UP:
            case PHASE_ORB_DOWN:
            case PHASE_ORB_LEFT:
            case PHASE_ORB_RIGHT:
            case TILE_PASSAGE:
            case DEVIL_PLATFORM:
            case ANGEL_PLATFORM:
            case SCYTHE_PLATFORM:	
            default:
            break;
        }
    }

    //////////////////////////
    // Udpate Frame Counter //
    //////////////////////////

    current_lifespan_frame--;
    current_lifespan_frame = clamp(0, lifespan_frames, current_lifespan_frame);
    
    if(current_lifespan_frame <= 0) {is_inactive = true;}

    /////////////////////////////////
	// Generic Object Update stuff //
	/////////////////////////////////
	
	GameObject::update(room_bounds,
                       game_objects,
                       bg_ptr,
                       cells,
                       bg_item,
                       camera);

}

void Hitbox::applyHit(GameObject& object)
{
    if(object.invulnerability_frames <= 0)
    {
        object.setHitFlash();
        object.setHitStretch();
        object.applyDamage(damage);
        object.rigidbody.addForce(HITBOX_KNOCKBACK_FORCE);
        object.invulnerability_frames = GAME_OBJECT_HIT_INVULNERABILITY_FRAMES;
        
        bg_hitflash_frames     = hitstop_frames;
        hitstop_frames         = hitstop_frames;
        screenshake_frames     = screenshake_frames;
        screenshake_severity   = screenshake_severity;
    }
}