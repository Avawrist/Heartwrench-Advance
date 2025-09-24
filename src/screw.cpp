#include "screw.h"

//////////////////
// Struct Screw //
//////////////////

Screw::Screw()
{
    // Init Assets //
    object_type = SCREW;
    sprite_ptr  = bn::sprite_items::screw.create_sprite(0, 0);
    sprite_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                   0,
								                                   bn::sprite_items::screw.tiles_item(),
								                                   0, 0);

    // Init Variables //
    collider_offset_x = SCREW_COLLIDER_OFFSET_X;
    collider_offset_y = SCREW_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        SCREW_COLLIDER_WIDTH, 
                        SCREW_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;

    hitpoints = SCREW_HITPOINTS;

    thirty_fps = SCREW_30_FPS;

    state = IDLE;
}

Screw::Screw(const Screw& other) : GameObject(other)
{

}

Screw::~Screw()
{

}

Screw& Screw::operator =(const Screw& other)
{
    return *this;
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

void Screw::update(const RoomBounds& 							  room_bounds,
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

    //updateTimers();

    ///////////////////
    // Check if dead //
    ///////////////////

    checkIfDead();

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

    clampPosition(room_bounds);

	//////////////////////////////
    // Monitor unloading bounds //
    //////////////////////////////
    
	updateInactiveState(camera);

}

//////////////////////////////
// State Function Overrides //
//////////////////////////////

void Screw::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                               const bn::regular_bg_ptr&                      bg_ptr, 
                               const bn::span<const bn::regular_bg_map_cell>& cells,
                               const bn::regular_bg_item&                     bg_item,
                               const bn::camera_ptr&                          camera)
{
    switch(state)
    {
        case IDLE:

            // Death condition
            if(collider_offset_y == SCREW_MAX_Y_OFFSET) {setState(OBJECT_DEATH);}

        break;

        case OBJECT_DEATH:

			updateDeathState();

		break;

        default:
        break;
    }
}

void Screw::setState(ObjectState new_state)
{
    int32 anim_frame;

    // Set State
	state = new_state;

    switch(new_state)
    {
        case SCREW_UP:

            // Update offset
            collider_offset_y -= SCREW_STEP;

            // Clamp Y offset
            collider_offset_y = clamp(SCREW_MIN_Y_OFFSET, 
                                      SCREW_MAX_Y_OFFSET, 
                                      collider_offset_y);

            // Get anim frame
            anim_frame = (collider_offset_y + 1) * 0.5;

            // Update sprite
            animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
                                                                       0,
                                                                       bn::sprite_items::screw.tiles_item(),
                                                                       anim_frame, anim_frame);

            setState(IDLE);

        break;

        case SCREW_DOWN:

            // Update offset
            collider_offset_y += SCREW_STEP;

            // Clamp Y offset
            collider_offset_y = clamp(SCREW_MIN_Y_OFFSET, 
                                      SCREW_MAX_Y_OFFSET, 
                                      collider_offset_y);

            // Get anim frame
            anim_frame = collider_offset_y * 0.5;

            // Update sprite
            animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
                                                                       0,
                                                                       bn::sprite_items::screw.tiles_item(),
                                                                       anim_frame, anim_frame);

            if(anim_frame == SCREW_STOP_FRAME) {global_hitstop_frames = SCREW_HITSTOP_FRAMES;}

            setState(IDLE);

        break;

        case OBJECT_DEATH:

            global_hitstop_frames = SCREW_HITSTOP_FRAMES;
            object_request        = ObjectRequest(GEAR_DROP, bn::fixed_point(x(), y()));

            bn::sound_items::screw_click.play();

        break;

        default:
        break;
    }
}

/////////////////////////
// Collision Overrides //
/////////////////////////

// None..
