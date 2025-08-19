#include "finish_seal.h"

//////////////////////
// Struct FinishSeal //
//////////////////////

FinishSeal::FinishSeal()
{
    hitpoints = FINISH_SEAL_HITPOINTS;
    thirty_fps = FINISH_SEAL_30_FPS;

    // Init Assets //
    state       = IDLE;
    object_type = FINISH_SEAL;
    sprite_ptr  = bn::sprite_items::finish_seal.create_sprite(0, 0);
    sprite_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                   0,
								                                   bn::sprite_items::finish_seal.tiles_item(),
								                                   hitpoints - 1, 
                                                                   hitpoints - 1);

    // Init Variables //
    collider_offset_x = FINISH_SEAL_COLLIDER_OFFSET_X;
    collider_offset_y = FINISH_SEAL_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        FINISH_SEAL_COLLIDER_WIDTH, 
                        FINISH_SEAL_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;

    level_complete = false;
}

FinishSeal::FinishSeal(const FinishSeal& other) : GameObject(other)
{
    level_complete = other.level_complete;
}

FinishSeal::~FinishSeal()
{

}

FinishSeal& FinishSeal::operator =(const FinishSeal& other)
{
    level_complete = other.level_complete;

    return *this;
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

void FinishSeal::update(const RoomBounds& 							   room_bounds,
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

//////////////////////////////
// State Function Overrides //
//////////////////////////////

void FinishSeal::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                                    const bn::regular_bg_ptr&                      bg_ptr, 
                                    const bn::span<const bn::regular_bg_map_cell>& cells,
                                    const bn::regular_bg_item&                     bg_item,
                                    const bn::camera_ptr&                          camera)
{
    switch(state)
    {
        case IDLE:

            animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                          0,
                                                                          bn::sprite_items::finish_seal.tiles_item(),
                                                                          hitpoints - 1, 
                                                                          hitpoints - 1);

        break;

        case OBJECT_HITSTUN:

            updateHitstunState();

            animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                0,
                                                                bn::sprite_items::finish_seal.tiles_item(),
                                                                hitpoints - 1, 
                                                                hitpoints - 1);

        break;

		case OBJECT_DEATH:

			updateDeathState();

            if(animate_action_ptr->done()) {level_complete = true;}

		break;
		
		default:
		break;
    }
}

void FinishSeal::setState(ObjectState new_state)
{
	// Set State
	state = new_state;

	// Set animations & other state specific variables
	switch(new_state)
	{
		case OBJECT_DEATH:

            sprite_ptr->set_z_order(ENEMY_Z_ORDER);
			animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
																		1,
																		bn::sprite_items::finish_seal.tiles_item(),
																		1, 1, 2, 2, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5);

		break;

		default:
		break;
	}
}

/////////////////////////
// Collision Overrides //
/////////////////////////

// None..
