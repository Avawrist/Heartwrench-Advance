#include "small_vase.h"

//////////////////////
// Struct SmallVase //
//////////////////////

SmallVase::SmallVase()
{
    // Init Assets //
    object_type = SMALL_VASE;
    sprite_ptr  = bn::sprite_items::small_vase.create_sprite(0, 0);
    sprite_ptr->set_z_order(PROP_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                   0,
								                                   bn::sprite_items::small_vase.tiles_item(),
								                                   1, 1);

    // Init Variables //
    collider_offset_x = SMALL_VASE_COLLIDER_OFFSET_X;
    collider_offset_y = SMALL_VASE_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        SMALL_VASE_COLLIDER_WIDTH, 
                        SMALL_VASE_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;

    hitpoints = SMALL_VASE_HITPOINTS;
    max_hp    = SMALL_VASE_HITPOINTS;

    thirty_fps = SMALL_VASE_30_FPS;
}

SmallVase::SmallVase(const SmallVase& other) : GameObject(other)
{}

SmallVase::~SmallVase()
{

}

SmallVase& SmallVase::operator =(const SmallVase& other)
{
    return *this;
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

void SmallVase::update(const RoomBounds& 							   room_bounds,
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

//////////////////////////////
// State Function Overrides //
//////////////////////////////

void SmallVase::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
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

void SmallVase::setState(ObjectState new_state)
{
	// Set State
	state = new_state;

	// Set animations & other state specific variables
	switch(new_state)
	{
		case OBJECT_DEATH:

            object_request = ObjectRequest(MOON_DROP, bn::fixed_point(x(), y()));

            sprite_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
			animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
																	   1,
																	   bn::sprite_items::small_vase.tiles_item(),
																	   3, 3, 4, 4, 5, 5, 5, 6, 6, 6);

		break;

		default:
		break;
	}
}

/////////////////////////
// Collision Overrides //
/////////////////////////

// None..
