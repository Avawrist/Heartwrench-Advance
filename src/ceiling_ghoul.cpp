#include "ceiling_ghoul.h"

CeilingGhoul::CeilingGhoul()
{   
        // Init Variables //
        object_type = CEILING_GHOUL;
        sprite_ptr  = bn::sprite_items::ceiling_ghoul.create_sprite(0, 0);
        sprite_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
        default_palette_ptr = sprite_ptr->palette();
        animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                      2,
                                                                      bn::sprite_items::ceiling_ghoul.tiles_item(),
                                                                      0,
                                                                      0);
    
        collider          = Collider(x(), y(), CEILING_GHOUL_COLLIDER_WIDTH, CEILING_GHOUL_COLLIDER_HEIGHT);
        collider_x_axis   = collider;
        collider_y_axis   = collider;
        collider_offset_x = CEILING_GHOUL_COLLIDER_OFFSET_X;
        collider_offset_y = CEILING_GHOUL_COLLIDER_OFFSET_Y;

        state = CEILING_GHOUL_CRAWL;
        x_dir = RIGHT;
        y_dir = UP;

        hitpoints = CEILING_GHOUL_HITPOINTS;
}

CeilingGhoul::CeilingGhoul(const CeilingGhoul& other) : Enemy(other)
{

}

CeilingGhoul::~CeilingGhoul()
{

}

CeilingGhoul& CeilingGhoul::operator =(const CeilingGhoul& other)
{
    return *this;
}

void CeilingGhoul::update(const RoomBounds&                              room_bounds,
                         bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                         const bn::regular_bg_ptr&                      bg_ptr, 
                         const bn::span<const bn::regular_bg_map_cell>& cells,
                         const bn::regular_bg_item&                     bg_item,
                         const bn::camera_ptr&                          camera)
{

    ///////////////////
    // State Machine //
    ///////////////////

    switch(state)
    {
        case CEILING_GHOUL_IDLE:
        break;

        case CEILING_GHOUL_CRAWL:

            rigidbody.addForce(CEILING_GHOUL_CRAWL_FORCE);

        break;

        case OBJECT_HITSTUN:

            updateHitstunState();

        break;

        case OBJECT_DEATH:

            udpateDeathState();

        break;

        default:
        break;
    }

    ////////////////////
    // Update Physics //
    ////////////////////

    // Apply Decay to Forces
	rigidbody.applyDecay();

	// Apply forces to enemy
	applyForces();

    ///////////////////////
    // Resolve Collision //
    ///////////////////////

    resolveCollision(game_objects, bg_ptr, cells, bg_item);

    ////////////////////////////////
    // Get State from GameObjects //
    ////////////////////////////////

    Collider other_collider;

    for(int32 i = 0; i < game_objects.size(); i++)
    {
        if(i != object_id)
        {
            other_collider = game_objects.at(i)->collider;

            switch(game_objects.at(i)->object_type)
            {
                case TILE_PASSAGE:
                case PHASE_ORB_UP:
                case PHASE_ORB_DOWN:
                case PHASE_ORB_LEFT:
                case PHASE_ORB_RIGHT:
                case DEVIL_PLATFORM:
                case ANGEL_PLATFORM:
                case SCYTHE_PLATFORM:
                break;
    
                case GROUND_GHOUL:
                case CEILING_GHOUL:
                case WALL_LEFT_GHOUL:
                case WALL_RIGHT_GHOUL:    
                break;
                
                default:
                break;
            }
        }
	}

    //////////////////
    // Update State //
    //////////////////

    if(state != OBJECT_DEATH &&
       state != OBJECT_HITSTUN)
    {
        setState(CEILING_GHOUL_CRAWL);
    }

    /////////////////////////////////
	// Generic Object Update stuff //
	/////////////////////////////////
	
	GameObject::update(room_bounds,
                       game_objects,
                       bg_ptr,
                       cells,
                       bg_item,
                       camera);

}

void CeilingGhoul::setState(ObjectState new_state)
{
    state = new_state;

    switch(new_state)
    {
        case CEILING_GHOUL_IDLE:
        break;

        case CEILING_GHOUL_CRAWL:

            animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                 0,
                                 bn::sprite_items::ceiling_ghoul.tiles_item(),
                                 0, 0);

        break;

        case OBJECT_HITSTUN:

            animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                 0,
                                 bn::sprite_items::ceiling_ghoul.tiles_item(),
                                 18, 18);

        break;

        case OBJECT_DEATH:

            animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
                                 0,
                                 bn::sprite_items::ceiling_ghoul.tiles_item(),
                                 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17);

        break;

        default:
        break;
    }
}

void CeilingGhoul::resolveXAxisCollision(const Collider& other_collider)
{
    Enemy::resolveXAxisCollision(other_collider);

    // Update direction
	if(col_x_offset < 0)      {x_dir = LEFT;}
	else if(col_x_offset > 0) {x_dir = RIGHT;}
}

