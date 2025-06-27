#include "ground_ghoul.h"

////////////////////////
// Struct GroundGhoul //
////////////////////////

GroundGhoul::GroundGhoul()
{    
        // Init Variables //
        object_type = GROUND_GHOUL;
        sprite_ptr  = bn::sprite_items::ground_ghoul.create_sprite(0, 0);
        sprite_ptr->set_z_order(ENEMY_Z_ORDER);
        default_palette_ptr = sprite_ptr->palette();
        animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                2,
                                                                bn::sprite_items::ground_ghoul.tiles_item(),
                                                                0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3);
    
        collider          = Collider(x(), y(), GROUND_GHOUL_COLLIDER_WIDTH, GROUND_GHOUL_COLLIDER_HEIGHT);
        collider_x_axis   = collider;
        collider_y_axis   = collider;
        collider_offset_x = GROUND_GHOUL_COLLIDER_OFFSET_X;
        collider_offset_y = GROUND_GHOUL_COLLIDER_OFFSET_Y;

        state = IDLE;
        x_dir = LEFT;
        y_dir = UP;

        test_collider       = collider;
        test_collider_right = collider;
        test_collider_left  = collider;

        hitpoints = GROUND_GHOUL_HITPOINTS;

        action_timer   = GROUND_GHOUL_ACTION_TIMER;
        next_crawl_dir = LEFT;
}

GroundGhoul::GroundGhoul(const GroundGhoul& other) : Enemy(other)
{
    test_collider       = other.test_collider;
    test_collider_right = other.test_collider_right;
    test_collider_left  = other.test_collider_left;

    action_timer   = other.action_timer;
    next_crawl_dir = other.next_crawl_dir;
}

GroundGhoul::~GroundGhoul()
{

}

GroundGhoul& GroundGhoul::operator =(const GroundGhoul& other)
{
    test_collider       = other.test_collider;
    test_collider_right = other.test_collider_right;
    test_collider_left  = other.test_collider_left;

    action_timer   = other.action_timer;
    next_crawl_dir = other.next_crawl_dir;

    return *this;
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

void GroundGhoul::updateTimers()
{
    GameObject::updateTimers();

    action_timer--;
    action_timer = clamp(0, GROUND_GHOUL_ACTION_TIMER, action_timer);
}

//////////////////////////////
// State Function Overrides //
//////////////////////////////

void GroundGhoul::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&        game_objects,
                                        const bn::regular_bg_ptr&                      bg_ptr, 
                                        const bn::span<const bn::regular_bg_map_cell>& cells,
                                        const bn::regular_bg_item&                     bg_item,
                                        const bn::camera_ptr&                          camera)
{

    switch(state)
    {
        case IDLE:

            // Crawl timer
            if(action_timer <= 0) 
            {setState(GROUND_GHOUL_CRAWL);}

            // Update direction
            if(game_objects.at(PLAYER_OBJECT_LIST_INDEX)->x() > x())
            {next_crawl_dir = RIGHT;}
            else
            {next_crawl_dir = LEFT;}

            // Gravity
            if(!grounded_detected)
            {rigidbody.addForce(GAME_OBJECT_GRAVITY_FORCE);}

        break;

        case GROUND_GHOUL_CRAWL:

            // Exit state condition
            if(animate_action_ptr->done())
            {setState(IDLE);}

            //if(animate_action_ptr->current_index() >= GROUND_GHOUL_CRAWL_FRAME)
            //{rigidbody.addForce(GROUND_GHOUL_CRAWL_FORCE);}

            // Gravity
            if(!grounded_detected)
            {rigidbody.addForce(GAME_OBJECT_GRAVITY_FORCE);}

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
}

void GroundGhoul::setState(ObjectState new_state)
{
    state = new_state;

    switch(new_state)
    {
        case IDLE:

            action_timer       = GROUND_GHOUL_ACTION_TIMER;
            animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                            2,
                                                                            bn::sprite_items::ground_ghoul.tiles_item(),
                                                                            0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3);

        break;

        case GROUND_GHOUL_CRAWL:

            x_dir = next_crawl_dir;
            animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
                                                                          2,
                                                                          bn::sprite_items::ground_ghoul.tiles_item(),
                                                                          4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 
                                                                          8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 11);

        break;

        case OBJECT_HITSTUN:

            animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                 0,
                                 bn::sprite_items::ground_ghoul.tiles_item(),
                                 31, 31);

        break;

        case OBJECT_DEATH:

            animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
                                 0,
                                 bn::sprite_items::ground_ghoul.tiles_item(),
                                 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30);

        break;

        default:
        break;
    }
}

/////////////////////////
// Collision Overrides //
/////////////////////////

// None..