#include "jumppoint.h"

///////////////////////
// Struct Jumppoint //
///////////////////////

Jumppoint::Jumppoint()
{
    // Init Assets //
    state       = JUMPPOINT_UNCOLLECTED;
    object_type = JUMPPOINT;
    sprite_ptr  = bn::sprite_items::jumppoint.create_sprite(0, 0);
    sprite_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                   2,
								                                   bn::sprite_items::jumppoint.tiles_item(),
								                                   0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1);

    // Init Variables //
    collider_offset_x = JUMPPOINT_COLLIDER_OFFSET_X;
    collider_offset_y = JUMPPOINT_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        JUMPPOINT_COLLIDER_WIDTH, 
                        JUMPPOINT_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;

    hitpoints = JUMPPOINT_HITPOINTS;

    thirty_fps = JUMPPOINT_30_FPS;

    cost = JUMPPOINT_COST;
}

Jumppoint::Jumppoint(const Jumppoint& other) : GameObject(other)
{
    cost = other.cost;
}

Jumppoint::~Jumppoint()
{

}

Jumppoint& Jumppoint::operator =(const Jumppoint& other)
{
    cost = other.cost;

    return *this;
}

void Jumppoint::setCollectingAnimation()
{
	bn::optional<bn::sprite_ptr> temp_sprite_ptr = bn::sprite_items::jumppoint.create_sprite(sprite_ptr->x().integer(), 
																				  			 sprite_ptr->y().integer());
	temp_sprite_ptr->set_camera(sprite_ptr->camera());
	temp_sprite_ptr->set_z_order(sprite_ptr->z_order());
	temp_sprite_ptr->set_visible(sprite_ptr->visible());

	sprite_ptr->swap(temp_sprite_ptr.value());

	temp_sprite_ptr.reset();

    animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
                                                               1,
                                                               bn::sprite_items::jumppoint.tiles_item(),
                                                               3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7,
                                                               8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 11);
}

void Jumppoint::setCollectedAnimation()
{
	bn::optional<bn::sprite_ptr> temp_sprite_ptr = bn::sprite_items::jumppoint.create_sprite(sprite_ptr->x().integer(), 
																				  			 sprite_ptr->y().integer());
	temp_sprite_ptr->set_camera(sprite_ptr->camera());
	temp_sprite_ptr->set_z_order(sprite_ptr->z_order());
	temp_sprite_ptr->set_visible(sprite_ptr->visible());

	sprite_ptr->swap(temp_sprite_ptr.value());

	temp_sprite_ptr.reset();

    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                  0,
                                                                  bn::sprite_items::jumppoint.tiles_item(),
                                                                  11, 11);
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

void Jumppoint::update(const RoomBounds& 							   room_bounds,
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

void Jumppoint::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&    game_objects,
                                    const bn::regular_bg_ptr&                      bg_ptr, 
                                    const bn::span<const bn::regular_bg_map_cell>& cells,
                                    const bn::regular_bg_item&                     bg_item,
                                    const bn::camera_ptr&                          camera) 
{
    switch(state)
    {
        case JUMPPOINT_COLLECTED:

            // Play collected animation post collecting animation
            if(animate_action_ptr->done())
            {setCollectedAnimation();}

        break;

        case JUMPPOINT_UNCOLLECTED:

            if(global_overjump)
            {
                state = JUMPPOINT_COLLECTED;
                setCollectedAnimation();
            }

        break;

        case OBJECT_HITSTUN:

            updateHitstunState();

        break;

        default:
        break;
    }
}

void Jumppoint::setState(ObjectState new_state)
{
    state = new_state;

    switch(new_state)
    {
        case JUMPPOINT_UNCOLLECTED:

            animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                           2,
                                                                           bn::sprite_items::jumppoint.tiles_item(),
                                                                           0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1);       

        break;

        case JUMPPOINT_COLLECTED:

            global_overjump = true;
            setCollectingAnimation();

        break;

        case OBJECT_HITSTUN:

            // SFX
            bn::sound_items::generic_hit.play();

        break;

        default:
        break;
    }
}

void Jumppoint::updateHitstunState()
{
    hitstun_frames--;
    hitstun_frames = max(0, hitstun_frames);

    if(!hitstun_frames)
    {setState(JUMPPOINT_UNCOLLECTED);}
}

/////////////////////////
// Collision Overrides //
/////////////////////////

// None..
