#include "smash_block_mini.h"

////////////////////////////
// Struct SmashBlockMini //
////////////////////////////

SmashBlockMini::SmashBlockMini()
{
    // Init Assets //
    object_type = SMASH_BLOCK_MINI;
    sprite_ptr  = bn::sprite_items::smash_block_mini.create_sprite(0, 0);
    sprite_ptr->set_z_order(PROP_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                   0,
								                                   bn::sprite_items::smash_block_mini.tiles_item(),
								                                   0, 0);

    // Init Variables //
    collider_offset_x = SMASH_BLOCK_MINI_COLLIDER_OFFSET_X;
    collider_offset_y = SMASH_BLOCK_MINI_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        SMASH_BLOCK_MINI_COLLIDER_WIDTH, 
                        SMASH_BLOCK_MINI_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;

    hitpoints = SMASH_BLOCK_MINI_HITPOINTS;
}

SmashBlockMini::SmashBlockMini(const SmashBlockMini& other) : GameObject(other)
{

}

SmashBlockMini::~SmashBlockMini()
{

}

SmashBlockMini& SmashBlockMini::operator =(const SmashBlockMini& other)
{
    return *this;
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

void SmashBlockMini::update(const RoomBounds& 							   room_bounds,
                            bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                            const bn::regular_bg_ptr&                      bg_ptr, 
                            const bn::span<const bn::regular_bg_map_cell>& cells,
                            const bn::regular_bg_item&                     bg_item,
                            const bn::camera_ptr&                          camera)
{
    //////////////////
    // Update State //
    //////////////////

    updateStateMachine(game_objects, bg_ptr, cells, bg_item, camera);

    ///////////////////
    // Check if dead //
    ///////////////////

    checkIfDead();

	//////////////////////
	// Update Hit Flash //
	//////////////////////

	updateHitFlash();

    ////////////////////
    // Clamp Position //
    ////////////////////

    clampPosition(bg_ptr);

	//////////////////////////////
    // Monitor unloading bounds //
    //////////////////////////////
    
	updateInactiveState(camera);
}

void SmashBlockMini::updateDeathState()
{
    if(animate_action_ptr->current_index() == SMASH_BLOCK_MINI_HIT_STOP_TRIGGER_FRAME)
    {global_hitstop_frames = SMASH_BLOCK_MINI_HIT_STOP_FRAMES;}

    GameObject::updateDeathState();
}

//////////////////////////////
// State Function Overrides //
//////////////////////////////

void SmashBlockMini::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                                         const bn::regular_bg_ptr&                      bg_ptr, 
                                         const bn::span<const bn::regular_bg_map_cell>& cells,
                                         const bn::regular_bg_item&                     bg_item,
                                         const bn::camera_ptr&                          camera)
{
    switch(state)
    {
		case OBJECT_DEATH:

			updateDeathState();

		break;
		
		default:
		break;
    }
}

void SmashBlockMini::setState(ObjectState new_state)
{
	// Set State
	state = new_state;

	// Set animations & other state specific variables
	switch(new_state)
	{
		case OBJECT_DEATH:

            sprite_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
			animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
																	   1,
																	   bn::sprite_items::smash_block_mini.tiles_item(),
																	   1, 1, 2, 2, 3, 3, 4, 4, 4, 5, 5, 5);

		break;

		default:
		break;
	}
}

/////////////////////////
// Collision Overrides //
/////////////////////////

// None..