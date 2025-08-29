#include "smash_block_ziggurat_c.h"

////////////////////////////
// Struct SmashBlockZigguratC //
////////////////////////////

SmashBlockZigguratC::SmashBlockZigguratC()
{
    // Init Assets //
    object_type = SMASH_BLOCK_ZIGGURAT_C;
    sprite_ptr  = bn::sprite_items::smash_block_ziggurat_c.create_sprite(0, 0);
    sprite_ptr->set_z_order(PROP_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                   0,
								                                   bn::sprite_items::smash_block_ziggurat_c.tiles_item(),
								                                   0, 0);

    // Init Variables //
    collider_offset_x = SMASH_BLOCK_ZIGGURAT_C_COLLIDER_OFFSET_X;
    collider_offset_y = SMASH_BLOCK_ZIGGURAT_C_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        SMASH_BLOCK_ZIGGURAT_C_COLLIDER_WIDTH, 
                        SMASH_BLOCK_ZIGGURAT_C_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;

    thirty_fps = SMASH_BLOCK_ZIGGURAT_C_30_FPS;

    hitpoints = SMASH_BLOCK_ZIGGURAT_C_HITPOINTS;
}

SmashBlockZigguratC::SmashBlockZigguratC(const SmashBlockZigguratC& other) : GameObject(other)
{

}

SmashBlockZigguratC::~SmashBlockZigguratC()
{

}

SmashBlockZigguratC& SmashBlockZigguratC::operator =(const SmashBlockZigguratC& other)
{
    return *this;
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

void SmashBlockZigguratC::update(const RoomBounds& 							   room_bounds,
                             bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                             const bn::regular_bg_ptr&                      bg_ptr, 
                             const bn::span<const bn::regular_bg_map_cell>& cells,
                             const bn::regular_bg_item&                     bg_item,
                             const bn::camera_ptr&                          camera)
{
    /////////////////////////////////
    // Early out for 30FPS Objects //
    /////////////////////////////////

    if(thirty_fps && (global_timer % 2 == 0)) {return;}

    //////////////////
    // Update State //
    //////////////////

    updateStateMachine(game_objects, bg_ptr, cells, bg_item, camera);

    ///////////////////
    // Update Timers //
    ///////////////////

    updateTimers();

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

    clampPosition(room_bounds);

	//////////////////////////////
    // Monitor unloading bounds //
    //////////////////////////////
    
	updateInactiveState(camera);
}

void SmashBlockZigguratC::updateDeathState()
{
    if(animate_action_ptr->current_index() == SMASH_BLOCK_ZIGGURAT_C_HIT_STOP_TRIGGER_FRAME)
    {global_hitstop_frames = SMASH_BLOCK_ZIGGURAT_C_HIT_STOP_FRAMES;}

    GameObject::updateDeathState();
}

//////////////////////////////
// State Function Overrides //
//////////////////////////////

void SmashBlockZigguratC::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
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

void SmashBlockZigguratC::setState(ObjectState new_state)
{
	// Set State
	state = new_state;

	// Set animations & other state specific variables
	switch(new_state)
	{
		case OBJECT_DEATH:

            //object_request = ObjectRequest(STAR_DROP, bn::fixed_point(x(), y()));

            sprite_ptr->set_z_order(ENEMY_Z_ORDER);
            playBlockDeathAnim();

		break;

		default:
		break;
	}
}

/////////////////////////
// Collision Overrides //
/////////////////////////

// None..