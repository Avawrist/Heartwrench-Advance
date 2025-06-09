#include "ground_ghoul.h"

GroundGhoul::GroundGhoul()
{    
        // Init Variables //
        object_type = GROUND_GHOUL;
        sprite_ptr  = bn::sprite_items::ground_ghoul.create_sprite(0, 0);
        sprite_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
        default_palette_ptr = sprite_ptr->palette();
        animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                      0,
                                                                      bn::sprite_items::ground_ghoul.tiles_item(),
                                                                      0,
                                                                      0);
    
        collider          = Collider(x(), y(), GROUND_GHOUL_COLLIDER_WIDTH, GROUND_GHOUL_COLLIDER_HEIGHT);
        collider_x_axis   = collider;
        collider_y_axis   = collider;
        collider_offset_x = GROUND_GHOUL_COLLIDER_OFFSET_X;
        collider_offset_y = GROUND_GHOUL_COLLIDER_OFFSET_Y;

        state = GROUND_GHOUL_CRAWL;
        x_dir = RIGHT;
        y_dir = UP;

        grounded_detected = false;

        test_collider       = collider;
        test_collider_right = collider;
        test_collider_left  = collider;

        hitpoints = GROUND_GHOUL_HITPOINTS;
}

GroundGhoul::GroundGhoul(const GroundGhoul& other) : Enemy(other)
{
    grounded_detected = other.grounded_detected;

    test_collider       = other.test_collider;
    test_collider_right = other.test_collider_right;
    test_collider_left  = other.test_collider_left;
}

GroundGhoul::~GroundGhoul()
{

}

GroundGhoul& GroundGhoul::operator =(const GroundGhoul& other)
{
    grounded_detected = other.grounded_detected;

    test_collider       = other.test_collider;
    test_collider_right = other.test_collider_right;
    test_collider_left  = other.test_collider_left;

    return *this;
}

void GroundGhoul::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&        game_objects,
                                        const bn::regular_bg_ptr&                         bg_ptr, 
                                        const bn::span<const bn::regular_bg_map_cell>&    cells,
                                        const bn::regular_bg_item&                        bg_item,
                                        const bn::camera_ptr&                             camera)
{
    switch(state)
    {
        case GROUND_GHOUL_IDLE:
        break;

        case GROUND_GHOUL_CRAWL:

            rigidbody.addForce(GROUND_GHOUL_CRAWL_FORCE);

        break;

        case GROUND_GHOUL_AIR:

            rigidbody.addForce(GROUND_GHOUL_CRAWL_FORCE);
            rigidbody.addForce(GROUND_GHOUL_GRAVITY_FORCE);

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

void GroundGhoul::updateState()
{
    if(state != OBJECT_DEATH &&
       state != OBJECT_HITSTUN) {setState(GROUND_GHOUL_CRAWL);}
}

void GroundGhoul::setState(ObjectState new_state)
{
    state = new_state;

    switch(new_state)
    {
        case GROUND_GHOUL_IDLE:
        break;

        case GROUND_GHOUL_CRAWL:

            animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                 0,
                                 bn::sprite_items::ground_ghoul.tiles_item(),
                                 0, 0);

        break;

        case GROUND_GHOUL_AIR:
        break;

        case OBJECT_HITSTUN:

            animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                 0,
                                 bn::sprite_items::ground_ghoul.tiles_item(),
                                 18, 18);

        break;

        case OBJECT_DEATH:

            animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
                                 0,
                                 bn::sprite_items::ground_ghoul.tiles_item(),
                                 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17);

        break;

        default:
        break;
    }
}

void GroundGhoul::resolveXAxisCollision(const Collider& other_collider)
{
    Enemy::resolveXAxisCollision(other_collider);

    // Update direction
    if(col_x_offset < 0)      {x_dir = LEFT;}
	else if(col_x_offset > 0) {x_dir = RIGHT;}
}