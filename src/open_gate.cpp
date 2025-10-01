#include "open_gate.h"

///////////////////////
// Struct OpenGate //
///////////////////////

OpenGate::OpenGate()
{
    hitpoints  = OPEN_GATE_HITPOINTS;
    thirty_fps = OPEN_GATE_30_FPS;

    // Init Assets //
    state       = IDLE;
    object_type = OPEN_GATE;
    sprite_ptr  = bn::sprite_items::open_gate.create_sprite(0, 0);
    sprite_ptr->set_z_order(PROP_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                   0,
                                                                   bn::sprite_items::open_gate.tiles_item(),
                                                                   0, 0);

    // Init Variables //
    collider_offset_x = OPEN_GATE_COLLIDER_OFFSET_X;
    collider_offset_y = OPEN_GATE_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        OPEN_GATE_COLLIDER_WIDTH, 
                        OPEN_GATE_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;
}

OpenGate::OpenGate(const OpenGate& other) : GameObject(other)
{}

OpenGate::~OpenGate()
{

}

OpenGate& OpenGate::operator =(const OpenGate& other)
{
    return *this;
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

void OpenGate::update(const RoomBounds& 							   room_bounds,
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

void OpenGate::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&    game_objects,
                                    const bn::regular_bg_ptr&                      bg_ptr, 
                                    const bn::span<const bn::regular_bg_map_cell>& cells,
                                    const bn::regular_bg_item&                     bg_item,
                                    const bn::camera_ptr&                          camera)
{
    switch(state)
    {	
        case IDLE:
        break;

		default:
		break;
    }
}

void OpenGate::setState(ObjectState new_state)
{
	// Set State
	state = new_state;

	// Set animations & other state specific variables
	switch(new_state)
	{
        case IDLE:
        break;

		default:
		break;
	}
}

/////////////////////////
// Collision Overrides //
/////////////////////////

// None..
