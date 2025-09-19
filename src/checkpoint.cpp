#include "checkpoint.h"

///////////////////////
// Struct Checkpoint //
///////////////////////

Checkpoint::Checkpoint()
{
    // Init Assets //
    state       = CHECKPOINT_IDLE_OFF;
    object_type = CHECKPOINT;
    sprite_ptr  = bn::sprite_items::checkpoint.create_sprite(0, 0);
    sprite_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                   2,
								                                   bn::sprite_items::checkpoint.tiles_item(),
								                                   0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1);

    // Init Variables //
    collider_offset_x = CHECKPOINT_COLLIDER_OFFSET_X;
    collider_offset_y = CHECKPOINT_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        CHECKPOINT_COLLIDER_WIDTH, 
                        CHECKPOINT_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;

    hitpoints = CHECKPOINT_HITPOINTS;

    thirty_fps = CHECKPOINT_30_FPS;

    cost = CHECKPOINT_COST;
}

Checkpoint::Checkpoint(const Checkpoint& other) : GameObject(other)
{
    cost = other.cost;
}

Checkpoint::~Checkpoint()
{

}

Checkpoint& Checkpoint::operator =(const Checkpoint& other)
{
    cost = other.cost;

    return *this;
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

void Checkpoint::update(const RoomBounds& 							   room_bounds,
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

    //////////////////////////
    // Update State Machine //
    //////////////////////////

    updateStateMachine(game_objects, bg_ptr, cells, bg_item, camera);

    ///////////////////
    // Update Timers //
    ///////////////////

    updateTimers();
    
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

void Checkpoint::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                                    const bn::regular_bg_ptr&                      bg_ptr, 
                                    const bn::span<const bn::regular_bg_map_cell>& cells,
                                    const bn::regular_bg_item&                     bg_item,
                                    const bn::camera_ptr&                          camera) 
{
    switch(state)
    {
        case CHECKPOINT_ACTIVE:

            setState(CHECKPOINT_IDLE_OFF);

        break;

        case CHECKPOINT_OVERWRITE:

            if(animate_action_ptr->done()) {setState(CHECKPOINT_IDLE_ON);}

        break;

        default:
        break;
    }
}

void Checkpoint::setState(ObjectState new_state)
{
    state = new_state;

    switch(new_state)
    {
        case CHECKPOINT_IDLE_OFF:

            animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                           2,
                                                                           bn::sprite_items::checkpoint.tiles_item(),
                                                                           0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1);       

        break;

        case CHECKPOINT_IDLE_ON:

            animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                           2,
                                                                           bn::sprite_items::checkpoint.tiles_item(),
                                                                           3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 4, 4, 4, 4);

        break;

        case CHECKPOINT_ACTIVE:
        break;

        case CHECKPOINT_OVERWRITE:

            bn::sound_items::checkpoint.play();
            animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
                                                                       2,
                                                                       bn::sprite_items::checkpoint.tiles_item(),
                                                                       6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9,
                                                                       10, 10, 10, 11, 11, 11, 12, 12, 12);

        break;

        default:
        break;
    }
}

/////////////////////////
// Collision Overrides //
/////////////////////////

// None..
