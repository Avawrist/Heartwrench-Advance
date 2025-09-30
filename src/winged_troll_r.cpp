#include "winged_troll_r.h"

////////////////////////
// Struct WingedTrollR //
////////////////////////

WingedTrollR::WingedTrollR()
{
    // Init Assets //
    object_type = WINGED_TROLL_R;
    sprite_ptr  = bn::sprite_items::winged_troll.create_sprite(0, 0);
    sprite_ptr->set_z_order(ENEMY_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                   2,
								                                   bn::sprite_items::winged_troll.tiles_item(),
								                                   0, 0, 0, 1, 1, 1);

    // Init Variables //
    collider_offset_x = WINGED_TROLL_R_COLLIDER_OFFSET_X;
    collider_offset_y = WINGED_TROLL_R_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        WINGED_TROLL_R_COLLIDER_WIDTH, 
                        WINGED_TROLL_R_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;

    state = IDLE;
    x_dir = RIGHT;

    hitpoints = WINGED_TROLL_R_HITPOINTS;

    thirty_fps = WINGED_TROLL_R_30_FPS;

    first = true;
}

WingedTrollR::WingedTrollR(const WingedTrollR& other) : Enemy(other)
{
    first = other.first;

    left_bound  = other.left_bound;
    right_bound = other.right_bound;
}

WingedTrollR::~WingedTrollR()
{

}

WingedTrollR& WingedTrollR::operator =(const WingedTrollR& other)
{
    first = other.first;

    left_bound  = other.left_bound;
    right_bound = other.right_bound;

    return *this;
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

// None..

//////////////////////////////
// State Function Overrides //
//////////////////////////////

void WingedTrollR::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                                     const bn::regular_bg_ptr&                      bg_ptr, 
                                     const bn::span<const bn::regular_bg_map_cell>& cells,
                                     const bn::regular_bg_item&                     bg_item,
                                     const bn::camera_ptr&                          camera)
{
    switch(state)
    {
        case IDLE:

            // Set boundaries if first update
            if(first)
            {
                first = false;
                left_bound =  x().integer() - (WINGED_TROLL_R_FLIGHT_RANGE / 2);
                right_bound = x().integer() + (WINGED_TROLL_R_FLIGHT_RANGE / 2);
            }

            // Update direction
            if(x() <= left_bound)       {x_dir = RIGHT;}
            else if(x() >= right_bound) {x_dir = LEFT;}

            // Add Flight Force
            rigidbody.addForce(WINGED_TROLL_R_FLIGHT_FORCE);

        break;

        case OBJECT_HITSTUN:

            updateHitstunState();

        break;

        case OBJECT_DEATH:

            updateDeathState();

            if(animate_action_ptr->done())
            {object_request = ObjectRequest(SKULL_DROP, bn::fixed_point(x(), y()));}

        break;

        default:
        break;
    }
}

void WingedTrollR::setState(ObjectState new_state)
{
    state = new_state;

    switch(new_state)
    {
        case IDLE:

            animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                          2,
                                                                          bn::sprite_items::winged_troll.tiles_item(),
                                                                          0, 0, 0, 1, 1, 1);

        break;

        case OBJECT_HITSTUN:

            setHitStretch();

            // SFX
            bn::sound_items::generic_hit.play();

        break;

        case OBJECT_DEATH:

            playEnemyDeathAnim();

        break;

        default:
        break;
    }
}

/////////////////////////
// Collision Overrides //
/////////////////////////

void WingedTrollR::resolveCollision(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                                   const bn::regular_bg_ptr&                      bg_ptr, 
                                   const bn::span<const bn::regular_bg_map_cell>& cells,
                                   const bn::regular_bg_item&                     bg_item)
{
    if(is_frozen) {return;}

    ////////////////////////////
    // Resolve Tile Collision //
    ////////////////////////////

	//resolveTileCollision(bg_ptr, cells, bg_item);

    //////////////////////////////////
    // Resolve GameObject Collision //
    //////////////////////////////////

    resolveObjectCollision(game_objects);

}
