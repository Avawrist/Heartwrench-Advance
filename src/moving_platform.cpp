#include "moving_platform.h"

MovingPlatform::MovingPlatform(bn::point _p1, bn::point _p2)
{
    object_type = MOVING_PLATFORM;
    sprite_ptr  = bn::sprite_items::moving_platform.create_sprite(0, 0);
    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                  2,
								                                  bn::sprite_items::moving_platform.tiles_item(),
								                                  0,
								                                  0);

    rigidbody_ptr = new RigidBody();
	collider_ptr  = new Collider(x(), y(), MOVING_PLATFORM_COLLIDER_WIDTH, MOVING_PLATFORM_COLLIDER_HEIGHT);

    collider_offset_x = 0;
	collider_offset_y = -12;

    speed = MOVING_PLATFORM_SPEED;

    target      = _p2;
    next_target = _p1;

    setPos(_p1);

    update_counter = 0;
}

MovingPlatform::~MovingPlatform()
{
    delete rigidbody_ptr;
    delete collider_ptr;
}

void MovingPlatform::update(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects,
                            const Room& room)
{
    if(update_counter % 2 == 0) // Perform update on even frames.
    {
        //////////////////////////////
        // Update Position & Target //
        //////////////////////////////

        int32 x_diff = target.x() - x().integer();
        int32 y_diff = target.y() - y().integer();
        int32 normalized_dir_x = 0;
        int32 normalized_dir_y = 0;

        if(x_diff != 0) {normalized_dir_x = x_diff / abs(x_diff);}
        if(y_diff != 0) {normalized_dir_y = y_diff / abs(y_diff);}
        bn::fixed_point normalized_dir = bn::fixed_point(normalized_dir_x, normalized_dir_y);

        // If we have not reached the target:
        if(normalized_dir_x || normalized_dir_y)
        {
            // Move toward target
            rigidbody_ptr->addForce(new Force(bn::fixed_point_t<12>(speed * normalized_dir.x(), 
                                                                    speed * normalized_dir.y()),
                                                                    MOVING_PLATFORM_DECAY));
        }
        else
        {
            // Swap targets
            bn::point temp = target;
            target         = next_target;
            next_target    = temp;
        }

        ///////////////////
        // Apply Physics //
        ///////////////////
        
        // Apply Decay to Forces
        rigidbody_ptr->applyDecay();

        // Apply forces to moving platform
        bn::fixed_point final_dir = rigidbody_ptr->applyForces(*this);

        ///////////////////////
        // Resolve Collision //
        ///////////////////////

        // Create test collider for roof collision checks
        #define MOVING_PLATFORM_ROOF_OFFSET            6
        #define MOVING_PLATFORM_ROOF_COLLIDER_HEIGHT   1
        #define MOVING_PLATFORM_SLIDE_COLLIDER_PADDING 2

        Collider* test_collider_roof_ptr = new Collider(collider_ptr->x(),
                                                        collider_ptr->y() - MOVING_PLATFORM_ROOF_OFFSET,
                                                        collider_ptr->width,
                                                        MOVING_PLATFORM_ROOF_COLLIDER_HEIGHT);

        Collider* test_collider_slide_ptr = new Collider(collider_ptr->x(),
                                                         collider_ptr->y(),
                                                         collider_ptr->width + MOVING_PLATFORM_SLIDE_COLLIDER_PADDING,
                                                         collider_ptr->height);
        Collider* test_collider_x_ptr = NULL;
        Collider* test_collider_y_ptr = NULL;

        for(int32 i = 0; i < game_objects.size(); i++)
        {
            if(game_objects.at(i)->object_type == PLAYER)
            {
                GameObject* player_ptr          = game_objects.at(i);
                Collider*   player_collider_ptr = player_ptr->collider_ptr;

                // If the player has any collision with the platform,
                // resolve it immediately by shifting the player until
                // there is no collision.
                if(collider_ptr->isCollision(*player_collider_ptr))
                {
                    test_collider_x_ptr = new Collider(collider_ptr->x(),
                                                    collider_ptr->y() - final_dir.y(),
                                                    collider_ptr->width,
                                                    collider_ptr->height);
                    test_collider_y_ptr = new Collider(collider_ptr->x() - final_dir.x(),
                                                    collider_ptr->y(),
                                                    collider_ptr->width,
                                                    collider_ptr->height);

                    while(test_collider_x_ptr->isCollision(*player_collider_ptr))
                    {
                        player_ptr->setX(player_ptr->x() + normalized_dir.x());
                    }
                    
                    while(test_collider_y_ptr->isCollision(*player_collider_ptr))
                    {
                        player_ptr->setY(player_ptr->y() + normalized_dir.y());
                    }

                    // If there is still a collision, it must be a corner case:
                    while(collider_ptr->isCollision(*(player_collider_ptr)))
                    {
                        // We resolve a diagonal corner collision with a horizontal shift. 
                        player_ptr->setX(player_ptr->x() + normalized_dir.x());
                    }
                }

                // Check for a wall slide to improve feel of wall sliding physics.
                if(test_collider_slide_ptr->isCollision(*player_collider_ptr) && 
                ((Player*)player_ptr)->state != STATE_GROUNDED_NEUTRAL)
                {
                    ((Player*)player_ptr)->rigidbody_ptr->addForce(new Force(bn::fixed_point_t<12>(0, 
                                                                                                final_dir.y()),
                                                                                                MOVING_PLATFORM_DECAY));
                }

                // Finally, if player is riding the platform:
                if(test_collider_roof_ptr->isCollision(*player_collider_ptr))
                {
                    if(final_dir.y() <= 0)
                    {
                        // If descending, applying force to the x axis is all that's needed.
                        // The player gravity will take care of the rest. 
                        ((Player*)player_ptr)->rigidbody_ptr->addForce(new Force(bn::fixed_point_t<12>(final_dir.x(), 0),
                                                                                MOVING_PLATFORM_DECAY));
                    }
                    else
                    {
                        // If ascending, apply force to BOTH axes and offset y by 1 
                        // so the player hugs the platform tight.
                        ((Player*)player_ptr)->rigidbody_ptr->addForce(new Force(bn::fixed_point_t<12>(final_dir.x(), final_dir.y() + 1),
                                                                                MOVING_PLATFORM_DECAY));
                    }
                }
            }					
        }

        delete test_collider_roof_ptr;
        delete test_collider_slide_ptr;
        delete test_collider_x_ptr;
        delete test_collider_y_ptr;
    }

    update_counter++;
    if(update_counter >= 60) {update_counter = 0;}
}

void MovingPlatform::draw()
{
    animate_action_ptr->update();
}

void MovingPlatform::setCamera(const bn::camera_ptr& camera)
{
    sprite_ptr->set_camera(camera);
    collider_ptr->setCamera(camera); 
}

bn::fixed MovingPlatform::x() const
{
    return sprite_ptr->x().integer();
}

bn::fixed MovingPlatform::y() const
{
    return sprite_ptr->y().integer();
} 

bn::fixed_point MovingPlatform::pos() const
{
    bn::fixed_point point(sprite_ptr->position().x().integer(),
			  			  sprite_ptr->position().y().integer());
    return point; 
}

void MovingPlatform::setX(bn::fixed new_x)
{
    sprite_ptr->set_x(new_x.integer());
    collider_ptr->setX(new_x.integer() + collider_offset_x);
}

void MovingPlatform::setY(bn::fixed new_y)
{
    sprite_ptr->set_y(new_y.integer());
    collider_ptr->setY(new_y.integer() + collider_offset_y);
}

void MovingPlatform::setPos(bn::fixed new_x, bn::fixed new_y) 
{
    sprite_ptr->set_x(new_x.integer());
    sprite_ptr->set_y(new_y.integer());
	collider_ptr->setX(new_x.integer() + collider_offset_x);
	collider_ptr->setY(new_y.integer() + collider_offset_y);
}

void MovingPlatform::setPos(bn::fixed_point new_pos)
{
    sprite_ptr->set_x(new_pos.x().integer());
    sprite_ptr->set_y(new_pos.y().integer());
	collider_ptr->setX(new_pos.x().integer() + collider_offset_x);
	collider_ptr->setY(new_pos.y().integer() + collider_offset_y);
}