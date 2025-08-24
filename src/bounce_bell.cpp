#include "bounce_bell.h"

///////////////////////
// Struct BounceBell //
///////////////////////

BounceBell::BounceBell()
{
    // Init Assets //
    object_type = BOUNCE_BELL;
    sprite_ptr  = bn::sprite_items::bounce_bell.create_sprite(0, 0);
    sprite_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                    0,
                                                                    bn::sprite_items::bounce_bell.tiles_item(),
                                                                    3, 3);

    // Init Variables //
    collider_offset_x = BOUNCE_BELL_COLLIDER_OFFSET_X;
    collider_offset_y = BOUNCE_BELL_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        BOUNCE_BELL_COLLIDER_WIDTH, 
                        BOUNCE_BELL_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;

    hitpoints = BOUNCE_BELL_HITPOINTS;

    thirty_fps = BOUNCE_BELL_30_FPS;
}

BounceBell::BounceBell(const BounceBell& other) : GameObject(other)
{

}

BounceBell::~BounceBell()
{

}

BounceBell& BounceBell::operator =(const BounceBell& other)
{
    return *this;
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

void BounceBell::update(const RoomBounds& 							   room_bounds,
                        bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                        const bn::regular_bg_ptr&                      bg_ptr, 
                        const bn::span<const bn::regular_bg_map_cell>& cells,
                        const bn::regular_bg_item&                     bg_item,
                        const bn::camera_ptr&                          camera)
{
    
    /////////////////////////////////
    // Early out for 30FPS Objects //
    /////////////////////////////////

    if((thirty_fps || is_frozen) && (global_timer % 2 == 0)) {return;}

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

    //resolveCollision(game_objects, bg_ptr, cells, bg_item);

    //////////////////
    // Update State //
    //////////////////

    updateState(game_objects, bg_ptr, cells, bg_item);

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

	updateHitFlash();

    ////////////////////
    // Clamp Position //
    ////////////////////

    clampPosition(room_bounds);

	//////////////////////////////
    // Monitor unloading bounds //
    //////////////////////////////
    
	updateInactiveState(camera);

}

void BounceBell::applyHit(int32 _damage, int32 knockback_x_dir, int32 knockback_y_dir)
{
    if(invulnerability_frames) {return;}
    
    global_bell_struck = true;

    // Object invuln:
    invulnerability_frames = GAME_OBJECT_HIT_INVULNERABILITY_FRAMES;

    // Global juice
    global_bg_hitflash_frames   = GENERIC_HIT_HITSTOP_FRAMES;
    global_hitstop_frames       = GENERIC_HIT_HITSTOP_FRAMES;
    global_screenshake_frames   = GENERIC_HIT_SCREENSHAKE_FRAMES;
    global_screenshake_severity = GENERIC_HIT_SCREENSHAKE_SEVERITY;

    // Object physics:
    rigidbody.removeForces();
    rigidbody.addForce(Force(bn::fixed_point_t<12>(GENERIC_HIT_X_KNOCKBACK * knockback_x_dir, 
                                                   GENERIC_HIT_Y_KNOCKBACK * knockback_y_dir), 
                                                   GENERIC_HIT_KNOCKBACK_DECAY));

    // Object damage:
    applyDamage(_damage);

    // Object hitstun state:
    //hitstun_frames = GENERIC_HIT_HITSTUN_FRAMES;
    setState(OBJECT_HITSTUN);

    // Object juice:
    setHitFlash();
    applyHitEffect(x().integer(),
                   y().integer());
}

void BounceBell::updateTimers()
{
    GameObject::updateTimers();

    invulnerability_frames = 0;
}

//////////////////////////////
// State Function Overrides //
//////////////////////////////

void BounceBell::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                                    const bn::regular_bg_ptr&                      bg_ptr, 
                                    const bn::span<const bn::regular_bg_map_cell>& cells,
                                    const bn::regular_bg_item&                     bg_item,
                                    const bn::camera_ptr&                          camera)
{
    switch(state)
    {
        case IDLE:

            if(animate_action_ptr->done())
            {
                animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                                0,
                                                                                bn::sprite_items::bounce_bell.tiles_item(),
                                                                                3, 3);
            }

        break;

        case OBJECT_HITSTUN:

            updateHitstunState();

        break;

		case OBJECT_DEATH:

			updateDeathState();

		break;
		
		default:
		break;
    }
}

void BounceBell::setState(ObjectState new_state)
{
	// Set State
	state = new_state;

	// Set animations & other state specific variables
	switch(new_state)
	{
        case OBJECT_HITSTUN:

            animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
                                                                        4,
                                                                        bn::sprite_items::bounce_bell.tiles_item(),
                                                                        7, 8, 9, 10, 11, 6,
                                                                        5, 4, 3, 2, 1, 
                                                                        2, 2, 3, 3, 4, 4, 5, 5,
                                                                        4, 4, 4, 4, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2);


        break;

		case OBJECT_DEATH:
		break;

		default:
		break;
	}
}

/////////////////////////
// Collision Overrides //
/////////////////////////

// None..
