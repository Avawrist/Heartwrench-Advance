#include "sealed_gate.h"

///////////////////////
// Struct SealedGate //
///////////////////////

SealedGate::SealedGate()
{
    hitpoints  = SEALED_GATE_HITPOINTS;
    thirty_fps = SEALED_GATE_30_FPS;

    // Init Assets //
    state       = IDLE;
    object_type = SEALED_GATE;
    sprite_ptr  = bn::sprite_items::sealed_gate.create_sprite(0, 0);
    sprite_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                    0,
                                                                    bn::sprite_items::sealed_gate.tiles_item(),
                                                                    0, 0);

    // Init Variables //
    collider_offset_x = SEALED_GATE_COLLIDER_OFFSET_X;
    collider_offset_y = SEALED_GATE_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        SEALED_GATE_COLLIDER_WIDTH, 
                        SEALED_GATE_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;
}

SealedGate::SealedGate(const SealedGate& other) : GameObject(other)
{}

SealedGate::~SealedGate()
{

}

SealedGate& SealedGate::operator =(const SealedGate& other)
{
    return *this;
}

void SealedGate::setOpen1Anim()
{
    bn::optional<bn::sprite_ptr> temp_sprite_ptr = bn::sprite_items::sealed_gate.create_sprite(sprite_ptr->x().integer(), 
																				  			   sprite_ptr->y().integer());
	temp_sprite_ptr->set_camera(sprite_ptr->camera());
	temp_sprite_ptr->set_z_order(sprite_ptr->z_order());
	temp_sprite_ptr->set_visible(sprite_ptr->visible());

	sprite_ptr->swap(temp_sprite_ptr.value());

	temp_sprite_ptr.reset();

	animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
															   0,
															   bn::sprite_items::sealed_gate.tiles_item(),
														   	   1, 1);
}

void SealedGate::setOpen2Anim()
{
    bn::optional<bn::sprite_ptr> temp_sprite_ptr = bn::sprite_items::sealed_gate.create_sprite(sprite_ptr->x().integer(), 
																				  			   sprite_ptr->y().integer());
	temp_sprite_ptr->set_camera(sprite_ptr->camera());
	temp_sprite_ptr->set_z_order(sprite_ptr->z_order());
	temp_sprite_ptr->set_visible(sprite_ptr->visible());

	sprite_ptr->swap(temp_sprite_ptr.value());

	temp_sprite_ptr.reset();

	animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
															   0,
															   bn::sprite_items::sealed_gate.tiles_item(),
														   	   2, 2);
}

void SealedGate::setOpen3Anim()
{
    bn::optional<bn::sprite_ptr> temp_sprite_ptr = bn::sprite_items::sealed_gate.create_sprite(sprite_ptr->x().integer(), 
																				  			   sprite_ptr->y().integer());
	temp_sprite_ptr->set_camera(sprite_ptr->camera());
	temp_sprite_ptr->set_z_order(sprite_ptr->z_order());
	temp_sprite_ptr->set_visible(sprite_ptr->visible());

	sprite_ptr->swap(temp_sprite_ptr.value());

	temp_sprite_ptr.reset();

	animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
															   0,
															   bn::sprite_items::sealed_gate.tiles_item(),
														   	   3, 3);
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

void SealedGate::update(const RoomBounds& 							   room_bounds,
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

    clampPosition(room_bounds);

	//////////////////////////////
    // Monitor unloading bounds //
    //////////////////////////////
    
	//updateInactiveState(camera);

}

//////////////////////////////
// State Function Overrides //
//////////////////////////////

void SealedGate::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                                    const bn::regular_bg_ptr&                      bg_ptr, 
                                    const bn::span<const bn::regular_bg_map_cell>& cells,
                                    const bn::regular_bg_item&                     bg_item,
                                    const bn::camera_ptr&                          camera)
{
    switch(state)
    {	
		default:
		break;
    }
}

void SealedGate::setState(ObjectState new_state)
{
	// Set State
	state = new_state;

	// Set animations & other state specific variables
	switch(new_state)
	{
        case IDLE:
        break;

        case SEALED_GATE_OPEN_1:

            setOpen1Anim();

        break;

        case SEALED_GATE_OPEN_2:

            setOpen2Anim();

        break;

        case SEALED_GATE_OPEN_3:

            setOpen3Anim();

            // Resize & reposition collider
            collider_offset_x = SEALED_GATE_COLLIDER_OPEN_OFFSET_X;
            collider_offset_y = SEALED_GATE_COLLIDER_OPEN_OFFSET_Y;

            collider = Collider(x() + collider_offset_x, 
                                y() + collider_offset_y, 
                                SEALED_GATE_COLLIDER_WIDTH, 
                                SEALED_GATE_COLLIDER_HEIGHT);

        break;

		default:
		break;
	}
}

/////////////////////////
// Collision Overrides //
/////////////////////////

// None..
