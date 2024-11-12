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
}

MovingPlatform::~MovingPlatform()
{
    delete rigidbody_ptr;
    delete collider_ptr;
}

void MovingPlatform::update(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects)
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

    // If we have not reached the target:
    if(normalized_dir_x || normalized_dir_y)
    {
        // Move toward target
        rigidbody_ptr->addForce(new Force(bn::fixed_point_t<12>(speed * normalized_dir_x, 
                                                                speed * normalized_dir_y),
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
	const uint32 roof_ray_length = 1;
	Collider*    test_collider_roof_ptr = new Collider(collider_ptr->x(),
											           collider_ptr->y() - roof_ray_length,
												       collider_ptr->width,
												       collider_ptr->height);

	for(int32 i = 0; i < game_objects.size(); i++)
    {
        if(game_objects.at(i)->object_type == PLAYER)
        {
            GameObject* player_ptr = game_objects.at(i);
            Collider*   player_collider_ptr = player_ptr->collider_ptr;

            if(test_collider_roof_ptr->isCollision(*player_collider_ptr)  || 
               collider_ptr->isCollision(*player_collider_ptr))
            {
                player_ptr->setPos(player_ptr->x() + final_dir.x(),
                                   player_ptr->y() + final_dir.y());
            }
        }					
    }

    delete test_collider_roof_ptr;
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