#include "hp_totem.h"

////////////////////
// Struct HPTotem //
////////////////////

HPTotem::HPTotem()
{
    // Init Assets //
    object_type = HP_TOTEM;
    sprite_ptr  = bn::sprite_items::hp_totem.create_sprite(0, 0);
    sprite_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                   0,
								                                   bn::sprite_items::hp_totem.tiles_item(),
								                                   0, 0);

    // Init Variables //
    collider_offset_x = HP_TOTEM_COLLIDER_OFFSET_X;
    collider_offset_y = HP_TOTEM_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        HP_TOTEM_COLLIDER_WIDTH, 
                        HP_TOTEM_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;

    hitpoints = HP_TOTEM_HITPOINTS;
    max_hp    = HP_TOTEM_HITPOINTS;

    thirty_fps = HP_TOTEM_30_FPS;
}

HPTotem::HPTotem(const HPTotem& other) : GameObject(other)
{}

HPTotem::~HPTotem()
{

}

HPTotem& HPTotem::operator =(const HPTotem& other)
{
    return *this;
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

void HPTotem::update(const RoomBounds& 							    room_bounds,
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

void HPTotem::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
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

void HPTotem::setState(ObjectState new_state)
{
	// Set State
	state = new_state;

	// Set animations & other state specific variables
	switch(new_state)
	{
		case OBJECT_DEATH:

            object_request = ObjectRequest(HP_DROP, bn::fixed_point(x(), y()));

            sprite_ptr->set_z_order(ENEMY_Z_ORDER);

            playBlockDeathAnim();

		break;

        case OBJECT_HITSTUN:

            // SFX
            bn::sound_items::generic_hit.play();
            
        break;

		default:
		break;
	}
}

/////////////////////////
// Collision Overrides //
/////////////////////////

// None..