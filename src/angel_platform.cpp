#include "angel_platform.h"

AngelPlatform::AngelPlatform(bn::point _p1, bn::point _p2)
{
    // Reset Variables //
    sprite_ptr.reset();
    animate_action_ptr.reset();

    // Init Variables //
    object_type = ANGEL_PLATFORM;
    sprite_ptr  = bn::sprite_items::angel_platform.create_sprite(0, 0);
    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                  2,
								                                  bn::sprite_items::angel_platform.tiles_item(),
								                                  0,
								                                  0);

	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        ANGEL_PLATFORM_COLLIDER_WIDTH, 
                        ANGEL_PLATFORM_COLLIDER_HEIGHT);
    collider_offset_x = 0;
    collider_offset_y = -16;

    speed = ANGEL_PLATFORM_SPEED;

    target      = _p2;
    next_target = _p1;

    setPos(_p1);

    update_counter = 0;
}

AngelPlatform::~AngelPlatform()
{
    
}

void AngelPlatform::update(const RoomBounds&                              room_bounds,
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
                                                           ANGEL_PLATFORM_DECAY));
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

        // Apply forces to angel platform
        applyForces();

        ///////////////////////
        // Resolve Collision //
        ///////////////////////

        // Create test collider for roof collision checks
        #define ANGEL_PLATFORM_ROOF_OFFSET           -6
        #define ANGEL_PLATFORM_ROOF_COLLIDER_HEIGHT   1

        Collider* test_collider_roof_ptr = new Collider(collider.x(),
                                                        collider.y() + ANGEL_PLATFORM_ROOF_OFFSET,
                                                        collider.width,
                                                        ANGEL_PLATFORM_ROOF_COLLIDER_HEIGHT);

        for(int32 i = 0; i < game_objects.size(); i++)
        {
            if(game_objects.at(i)->object_type == PLAYER)
            {
                GameObject* player_ptr          = game_objects.at(i);
                Collider*   player_collider_ptr = &(player_ptr->collider);

                // If player is riding the platform:
                if(test_collider_roof_ptr->isCollision(*player_collider_ptr) && 
                   player_collider_ptr->p4.y() < collider.p1.y() + ANGEL_PLATFORM_SPEED)
                {
                    player_ptr->received_platform_force = true;

                    if(rigidbody.final_dir.y() <= 0)
                    {
                        // If descending, applying force to the x axis is all that's needed.
                        // The player gravity will take care of the rest. 
                        player_ptr->rigidbody.addForce(Force(bn::fixed_point_t<12>(rigidbody.final_dir.x(), 0),
                                                             ANGEL_PLATFORM_DECAY));
                    }
                    else
                    {
                        // If ascending, apply force to BOTH axes and offset y by 1 
                        // so the player hugs the platform tight.
                        player_ptr->rigidbody.addForce(Force(bn::fixed_point_t<12>(rigidbody.final_dir.x(), 
                                                                                   rigidbody.final_dir.y() + 1),
                                                             ANGEL_PLATFORM_DECAY));
                    }
                }
            }					
        }

        delete test_collider_roof_ptr;
    }

    update_counter++;
    if(update_counter >= 60) {update_counter = 0;}

    //////////////////////////////
    // Monitor unloading bounds //
    //////////////////////////////
    
    if(pos().x() < camera.position().x() - LOAD_RANGE_HALF_W || 
       pos().x() > camera.position().x() + LOAD_RANGE_HALF_W)
    {is_inactive = true;}
}