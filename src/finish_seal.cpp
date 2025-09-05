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
                                                                    4,
                                                                    bn::sprite_items::finish_seal.tiles_item(),
                                                                    6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 7, 7, 7, 7);

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

void FinishSeal::updateTimers()
{
    // Invuln frames
    invulnerability_frames -= 2;
    if(invulnerability_frames < 0) {invulnerability_frames = 0;}
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
        break;

        case OBJECT_HITSTUN:

            updateHitstunState();

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
        case OBJECT_HITSTUN:

            switch(hitpoints)
            {
                case 1:

                    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                                    1,
                                                                                    bn::sprite_items::finish_seal.tiles_item(),
                                                                                    0, 0, 1, 1, 2, 2, 1, 1);

                break;

                case 2:

                    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                                    3,
                                                                                    bn::sprite_items::finish_seal.tiles_item(),
                                                                                    3, 3, 3, 4, 4, 4, 5, 5, 5, 4, 4, 4);

                break;

                case 3:
                
                    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                                    4,
                                                                                    bn::sprite_items::finish_seal.tiles_item(),
                                                                                    6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 7, 7, 7, 7);

                break;

                default:
                break;
            }

        break;

		case OBJECT_DEATH:

            sprite_ptr->set_z_order(ENEMY_Z_ORDER);
			animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
																		1,
																		bn::sprite_items::finish_seal.tiles_item(),
																		9, 9, 10, 10, 11, 11, 12, 12, 12, 12, 13, 13, 13, 13);

		break;

		default:
		break;
	}
}

/////////////////////////
// Collision Overrides //
/////////////////////////

// None..
