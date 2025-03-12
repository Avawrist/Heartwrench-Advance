#include "devil_platform.h"

DevilPlatform::DevilPlatform(bn::point _p1, bn::point _p2)
{
    // Reset Variables //
    sprite_ptr.reset();
    animate_action_ptr.reset();

    // Init Variables //
    object_type = DEVIL_PLATFORM;
    sprite_ptr  = bn::sprite_items::devil_platform.create_sprite(0, 0);
    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                  2,
								                                  bn::sprite_items::devil_platform.tiles_item(),
								                                  0,
								                                  0);

	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        DEVIL_PLATFORM_COLLIDER_WIDTH, 
                        DEVIL_PLATFORM_COLLIDER_HEIGHT);
    collider_offset_x = 0;
    collider_offset_y = -12;

    speed = DEVIL_PLATFORM_SPEED;

    target      = _p2;
    next_target = _p1;

    setPos(_p1);

    update_counter = 0;
}

DevilPlatform::~DevilPlatform()
{
    
}

void DevilPlatform::update(const RoomBounds&                              room_bounds,
                           bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
				           const bn::regular_bg_ptr&                      bg_ptr, 
                           const bn::span<const bn::regular_bg_map_cell>& cells,
                           const bn::regular_bg_item&                     bg_item,
                           const bn::camera_ptr&                          camera)
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
            rigidbody.addForce(Force(bn::fixed_point_t<12>(speed * normalized_dir.x(), 
                                                           speed * normalized_dir.y()),
                                                           DEVIL_PLATFORM_DECAY));
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
        rigidbody.applyDecay();

        // Apply forces to devil platform
        applyForces();

        ///////////////////////
        // Resolve Collision //
        ///////////////////////

        // Create test collider for roof collision checks
        #define DEVIL_PLATFORM_ROOF_OFFSET           -6
        #define DEVIL_PLATFORM_ROOF_COLLIDER_HEIGHT   1
        #define DEVIL_PLATFORM_SLIDE_COLLIDER_PADDING 2

        Collider* test_collider_roof_ptr = new Collider(collider.x(),
                                                        collider.y() + DEVIL_PLATFORM_ROOF_OFFSET,
                                                        collider.width,
                                                        DEVIL_PLATFORM_ROOF_COLLIDER_HEIGHT);

        Collider* test_collider_slide_ptr = new Collider(collider.x(),
                                                         collider.y(),
                                                         collider.width + DEVIL_PLATFORM_SLIDE_COLLIDER_PADDING,
                                                         collider.height);
        Collider* test_collider_x_ptr = NULL;
        Collider* test_collider_y_ptr = NULL;

        for(int32 i = 0; i < game_objects.size(); i++)
        {

            GameObject* object_ptr          = game_objects.at(i);
            Collider*   object_collider_ptr = &(object_ptr->collider);

            switch(game_objects.at(i)->object_type)
            {
                case PLAYER:

                    // If the player has any collision with the platform,
                    // resolve it immediately by shifting the player until
                    // there is no collision.
                    if(collider.isCollision(*object_collider_ptr))
                    {
                        test_collider_x_ptr = new Collider(collider.x(),
                                                           collider.y() - rigidbody.final_dir.y(),
                                                           collider.width,
                                                           collider.height);
                        test_collider_y_ptr = new Collider(collider.x() - rigidbody.final_dir.x(),
                                                           collider.y(),
                                                           collider.width,
                                                           collider.height);
                        
                        while(test_collider_x_ptr->isCollision(*object_collider_ptr))
                        {
                            object_ptr->setX(object_ptr->x() + normalized_dir.x());
                        }
                        
                        while(test_collider_y_ptr->isCollision(*object_collider_ptr))
                        {
                            object_ptr->setY(object_ptr->y() + normalized_dir.y());
                        }

                        // If there is still a collision, it must be a corner case:
                        while(collider.isCollision(*object_collider_ptr))
                        {
                            // We resolve a diagonal corner collision with a horizontal shift. 
                            object_ptr->setX(object_ptr->x() + normalized_dir.x());
                        } 
                    }

                    // Check for a wall slide to improve feel of wall sliding physics.
                    if(test_collider_slide_ptr->isCollision(*object_collider_ptr) && 
                    ((Player*)object_ptr)->state != STATE_GROUNDED_NEUTRAL)
                    {
                        object_ptr->rigidbody.addForce(Force(bn::fixed_point_t<12>(0, rigidbody.final_dir.y()),
                                                             DEVIL_PLATFORM_DECAY));
                    }

                    // Finally, if player is riding the platform:
                    if(test_collider_roof_ptr->isCollision(*object_collider_ptr))
                    {
                        object_ptr->received_platform_force = true;

                        if(rigidbody.final_dir.y() <= 0)
                        {
                            // If descending, applying force to the x axis is all that's needed.
                            // The player gravity will take care of the rest. 
                            object_ptr->rigidbody.addForce(Force(bn::fixed_point_t<12>(rigidbody.final_dir.x(), 0),
                                                                 DEVIL_PLATFORM_DECAY));
                        }
                        else
                        {
                            // If ascending, apply force to BOTH axes and offset y by 1 
                            // so the player hugs the platform tight.
                            object_ptr->rigidbody.addForce(Force(bn::fixed_point_t<12>(rigidbody.final_dir.x(), 
                                                                                       rigidbody.final_dir.y() + 1),
                                                                                       DEVIL_PLATFORM_DECAY));
                        }
                    }

                break;

                default:
                break;

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