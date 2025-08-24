#include "hitbox.h"

///////////////////
// Struct Hitbox //
///////////////////

Hitbox::Hitbox(bn::point pos,
       int32      _hitstop_frames,
       int32      _hitstun_frames,
       int32      _screenshake_frames,
       int32      _lifespan_frames,
       int32      _x_knockback,
       int32      _y_knockback,
       bn::fixed  _knockback_decay,
       int32      _width,
       int32      _height,
       int32      _damage,
       XDirection _x_dir,
       YDirection _y_dir,
       ObjectType _type,
       ScreenShakeSeverity _screenshake_severity)
{
    // Init Variables //
	object_type = _type;
    
    sprite_ptr  = bn::sprite_items::hitbox.create_sprite(pos.x(), pos.y());
    sprite_ptr->set_visible(false);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  0,
								  								  bn::sprite_items::hitbox.tiles_item(),
								  								  0,
								  								  0);

	collider = Collider(pos.x(), pos.y(), _width, _height);
    collider_offset_x = 0;
	collider_offset_y = 0;

    thirty_fps = HITBOX_30_FPS;

    hitstop_frames         = _hitstop_frames;
    hitstun_frames         = _hitstun_frames;
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
    hitstun_frames         = other.hitstun_frames;
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
    hitstun_frames         = other.hitstun_frames;
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

void Hitbox::applyHBHit(GameObject& object)
{
    if(object.invulnerability_frames) {return;}
    
    // Object invuln:
    object.invulnerability_frames = GAME_OBJECT_HIT_INVULNERABILITY_FRAMES;

    // Determine hit effect location
    int32 x_offset_multiplier = object.x().integer() - x().integer(); 
    if(x_offset_multiplier != 0) {x_offset_multiplier /= abs(x_offset_multiplier);}

    int32 y_offset_multiplier = object.y().integer() - y().integer(); 
    if(y_offset_multiplier != 0) {y_offset_multiplier /= abs(y_offset_multiplier);}

    int32 x_offset = (width / 2)  * x_offset_multiplier;
    int32 y_offset = (height / 2) * y_offset_multiplier;

    // Global juice
    //if(object.hitpoints - damage <= 0)
    //{global_bg_hitflash_frames  = hitstop_frames;}
    global_bg_hitflash_frames   = hitstop_frames;
    global_hitstop_frames       = hitstop_frames;
    global_screenshake_frames   = screenshake_frames;
    global_screenshake_severity = screenshake_severity;

    // Object physics:
    object.rigidbody.removeForces();
    object.rigidbody.addForce(HITBOX_KNOCKBACK_FORCE);

    // Object Dir:
    object.x_dir = (XDirection)(x_dir * -1);

    // Object damage:
    object.applyDamage(damage);

    // Object hitstun state:
    object.hitstun_frames = hitstun_frames;
    object.setState(OBJECT_HITSTUN);

    // Object juice:
    object.setHitFlash();
    object.applyHitEffect(x().integer() + x_offset,
                          y().integer() + y_offset);
    
}

void Hitbox::applyWallHit(GameObject& object)
{
    #define SPLAT_SPRITE_X_OFFSET 16

    // Determine splat effect offset
    int32 splat_x_offset = 0;
    if     (x_dir == LEFT)  {splat_x_offset = (object.collider.width / 2) - SPLAT_SPRITE_X_OFFSET;}
    else if(x_dir == RIGHT) {splat_x_offset = (-object.collider.width / 2) + SPLAT_SPRITE_X_OFFSET;}

    // Global juice
    if(object.hitpoints - damage <= 0)
    {global_bg_hitflash_frames  = hitstop_frames;}
    global_hitstop_frames       = hitstop_frames;
    global_screenshake_frames   = screenshake_frames;
    global_screenshake_severity = screenshake_severity;

    // Object invuln:
    object.invulnerability_frames = GAME_OBJECT_HIT_INVULNERABILITY_FRAMES;

    // Object physics:
    object.rigidbody.removeForces();
    object.rigidbody.addForce(HITBOX_KNOCKBACK_FORCE);

    // Object damage:
    object.applyDamage(damage);

    // Object hitstun state:
    object.hitstun_frames = hitstun_frames;
    object.setState(OBJECT_HITSTUN);

    // Object juice:
    object.setHitFlash();
    //object.setVerticalStretch();
    object.applySplatEffect(object.x().integer() + splat_x_offset, 
                            object.y().integer());
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

void Hitbox::update(const RoomBounds& 							   room_bounds,
                    bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                    const bn::regular_bg_ptr&                      bg_ptr, 
                    const bn::span<const bn::regular_bg_map_cell>& cells,
                    const bn::regular_bg_item&                     bg_item,
                    const bn::camera_ptr&                          camera)
{
    
    /////////////////////////////////
    // Early out for 30FPS Objects //
    /////////////////////////////////

    //if((thirty_fps || is_frozen) && (global_timer % 2 == 0)) {return;}

    //////////////////
    // Update State //
    //////////////////

    updateStateMachine(game_objects, bg_ptr, cells, bg_item, camera);

    ////////////////////
    // Update Physics //
    ////////////////////

    //updatePhysics();

    ///////////////////////
    // Resolve Collision //
    ///////////////////////

    resolveCollision(game_objects, bg_ptr, cells, bg_item);

    //////////////////
    // Update State //
    //////////////////

    //updateState(game_objects, bg_ptr, cells, bg_item);

    /////////////////////
    // Update Hitboxes //
    /////////////////////

    //updateHitboxes(room_bounds, game_objects, bg_ptr, cells, bg_item, camera);

    ///////////////////
    // Update Timers //
    ///////////////////

    updateTimers();

    ///////////////////
    // Check if dead //
    ///////////////////

    //checkIfDead();

    /////////////////////////////
	// Update Sprite Direction //
	/////////////////////////////
	
    //updateSpriteDirection();
    
    ////////////////////////////
    // Correct Sprite Offsets //
    ////////////////////////////
	
    //updateSpriteOffsets();

    //////////////////////////
    // Update HP Bar Visual //
    //////////////////////////

    //updateHPBar();

	//////////////////////
	// Update Hit Flash //
	//////////////////////

	//updateHitFlash();

    ////////////////////
    // Clamp Position //
    ////////////////////

    //clampPosition(room_bounds);

	//////////////////////////////
    // Monitor unloading bounds //
    //////////////////////////////
    
	//updateInactiveState(camera);

}

//////////////////////////////
// State Function Overrides //
//////////////////////////////

void Hitbox::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
		                        const bn::regular_bg_ptr&                      bg_ptr, 
                                const bn::span<const bn::regular_bg_map_cell>& cells,
                                const bn::regular_bg_item&                     bg_item,
		                        const bn::camera_ptr&                          camera)
{
    // Udpate Frame Counter //
    current_lifespan_frame--;
    current_lifespan_frame = clamp(0, lifespan_frames, current_lifespan_frame);
    
    if(current_lifespan_frame <= 0) {is_inactive = true;}
}

/////////////////////////
// Collision Overrides //
/////////////////////////

// Level Objects

void Hitbox::resolveSmashBlockLargeCollision(GameObject& object)
{
    if(object.state == OBJECT_DEATH) {return;}

    if(collider.isCollision(object.collider))
    {applyHBHit(object);}
}

void Hitbox::resolveSmashBlockMiniCollision(GameObject& object)
{
    if(object.state == OBJECT_DEATH) {return;}

    if(collider.isCollision(object.collider))
    {applyHBHit(object);}
}

void Hitbox::resolveSmashBlockZigguratLCollision(GameObject& object)
{
    if(object.state == OBJECT_DEATH) {return;}

    if(collider.isCollision(object.collider))
    {applyHBHit(object);}
}

void Hitbox::resolveSmashBlockZigguratCCollision(GameObject& object)
{
    if(object.state == OBJECT_DEATH) {return;}

    if(collider.isCollision(object.collider))
    {applyHBHit(object);}
}

void Hitbox::resolveSmashBlockZigguratRCollision(GameObject& object)
{
    if(object.state == OBJECT_DEATH) {return;}

    if(collider.isCollision(object.collider))
    {applyHBHit(object);}
}

void Hitbox::resolveLargeVaseCollision(GameObject& object)
{
    if(object.state == OBJECT_DEATH) {return;}

    if(collider.isCollision(object.collider))
    {applyHBHit(object);}
}

void Hitbox::resolveSmallVaseCollision(GameObject& object)
{
    if(object.state == OBJECT_DEATH) {return;}

    if(collider.isCollision(object.collider))
    {applyHBHit(object);}
}

void Hitbox::resolveHPTotemCollision(GameObject& object)
{
    if(object.state == OBJECT_DEATH) {return;}

    if(collider.isCollision(object.collider))
    {applyHBHit(object);}
}

void Hitbox::resolveHPDropCollision(GameObject& object)
{
    if(object.state == OBJECT_DEATH) {return;}

    if(collider.isCollision(object.collider))
    {applyHBHit(object);}
}

void Hitbox::resolveCheckpointCollision(GameObject& object)
{	
	if(collider.isCollision(object.collider)                         && 
      (object_type == HITBOX_SPIN_1 || object_type == HITBOX_SPIN_2) &&
	   object.state == CHECKPOINT_IDLE_OFF)
    {	
        applyHBHit(object);
		object.setState(CHECKPOINT_ACTIVE);
	}
}

void Hitbox::resolveFinishSealCollision(GameObject& object)
{
    if(object.state == OBJECT_DEATH) {return;}

    if(collider.isCollision(object.collider))
    {applyHBHit(object);}
}

void Hitbox::resolveBounceBellCollision(GameObject& object)
{
    if(object.state == OBJECT_HITSTUN) {return;}

    if(collider.isCollision(object.collider))
    {
        global_bell_struck = true;
        applyHBHit(object);
    }
}

void Hitbox::resolveAutoBounceBellCollision(GameObject& object)
{
    if(object.state == OBJECT_HITSTUN) {return;}

    if(collider.isCollision(object.collider))
    {
        global_bell_struck = true;
        applyHBHit(object);
    }
}

void Hitbox::resolvePushBlockCollision(GameObject& object)
{
    if(collider.isCollision(object.collider))
    {applyHBHit(object);}
}

void Hitbox::resolvePushBlockMiniCollision(GameObject& object)
{
    if(collider.isCollision(object.collider))
    {applyHBHit(object);}
}

// Level Enemies
void Hitbox::resolveThornColumnCollision(GameObject& object)
{
    if(collider.isCollision(object.collider))
    {applyHBHit(object);}
}

void Hitbox::resolveThornBarCollision(GameObject& object)
{
    if(collider.isCollision(object.collider))
    {applyHBHit(object);}
}

void Hitbox::resolveGroundGhoulCollision(GameObject& object)
{
    if(object.state == OBJECT_DEATH) {return;}

    if(collider.isCollision(object.collider))
    {applyHBHit(object);}
}

void Hitbox::resolveBellTrollCollision(GameObject& object)
{
    if(object.state == OBJECT_DEATH) {return;}

    if(collider.isCollision(object.collider))
    {applyHBHit(object);}
}


// Tiles
void Hitbox::resolveTileCollision(const bn::regular_bg_ptr&                      bg_ptr, 
                                  const bn::span<const bn::regular_bg_map_cell>& cells,
                                  const bn::regular_bg_item&                     bg_item)
{

}