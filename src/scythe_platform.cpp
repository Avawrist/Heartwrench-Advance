#include "scythe_platform.h"

ScythePlatform::ScythePlatform(XDirection _x_dir, bn::fixed_point _p)
{
    // Reset Variables //
    sprite_ptr.reset();
	default_palette_ptr.reset();
    animate_action_ptr.reset();

    // Init Variables //
    object_type = SCYTHE_PLATFORM;
    sprite_ptr  = bn::sprite_items::scythe_platform.create_sprite(0, 0);
	default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                  2,
								                                  bn::sprite_items::scythe_platform.tiles_item(),
								                                  0,
								                                  0);
	
	// Make invisible for the first frame 
	// so we don't see ugly collisions on creation
	sprite_ptr->set_visible(false);

	collider = Collider(x() + collider_offset_x, 
						y() + collider_offset_y, 
						SCYTHE_PLATFORM_COLLIDER_WIDTH, 
						SCYTHE_PLATFORM_COLLIDER_HEIGHT);
	collider_offset_x = 0;
	collider_offset_y = 0;

    state             = SCYTHE_PLATFORM_THROWN;
	player_was_riding = false;
	update_counter    = 0;
    x_dir             = _x_dir;

	// Apply throw force
	rigidbody.addForce(SCYTHE_PLATFORM_THROW_FORCE);

	// Set initial stretch
	sprite_ptr->set_vertical_scale(SCYTHE_MIN_STRETCH_V);
	sprite_ptr->set_horizontal_scale(SCYTHE_MAX_STRETCH_H);

    setPos(_p);
}

ScythePlatform::~ScythePlatform()
{

}

void ScythePlatform::update(const RoomBounds& 								  room_bounds,
							bn::vector<GameObject*, MAX_GAME_OBJECTS>&        game_objects,
							const bn::regular_bg_ptr&                         bg_ptr, 
                        	const bn::span<const bn::regular_bg_map_cell>&    cells,
                        	const bn::regular_bg_item&                        bg_item,
							const bn::camera_ptr&                             camera)
{

    ///////////////////
    // State Machine //
    ///////////////////

    switch(state)
    {
        case SCYTHE_PLATFORM_THROWN:

			// Make visible
			sprite_ptr->set_visible(true);

        break;

        default:
        break;
    }

    ///////////////////
    // Apply Physics //
    ///////////////////
    
    // Apply Decay to Forces
    rigidbody.applyDecay();

    // Apply forces to scythe platform
    applyForces();

	///////////////////////////////////
	// Resolve Game Object Collision //
	///////////////////////////////////

	Collider* other_collider_ptr = NULL;

	// Create test collider for roof collision checks
	#define SCYTHE_PLATFORM_ROOF_OFFSET           -2
	#define SCYTHE_PLATFORM_ROOF_COLLIDER_HEIGHT   1

	Collider* test_collider_roof_ptr = new Collider(collider.x(),
													collider.y() + SCYTHE_PLATFORM_ROOF_OFFSET,
													collider.width,
													SCYTHE_PLATFORM_ROOF_COLLIDER_HEIGHT);

	for(int32 i = 0; i < game_objects.size(); i++)
	{

		GameObject* object_ptr = game_objects.at(i);
		other_collider_ptr     = &(object_ptr->collider);

		switch(object_ptr->object_type)
		{
			case PLAYER:
				
				// If player is riding the platform:
				if(!object_ptr->received_platform_force && 
					test_collider_roof_ptr->isCollision(*other_collider_ptr) &&
					other_collider_ptr->p4.y() < collider.p1.y() - rigidbody.final_dir.y())
				{
					if(update_counter % 2 == 0) // Lower every other frame.
					{setY(y() + 1);}
					
					if(rigidbody.final_dir.y() <= 0)
					{
						// If descending, applying force to the x axis is all that's needed.
						// The player gravity will take care of the rest. 
						object_ptr->rigidbody.addForce(Force(bn::fixed_point_t<12>(rigidbody.final_dir.x(), 0),
															 SCYTHE_PLATFORM_DECAY));
					}
					else
					{
						// If ascending, apply force to BOTH axes and offset y by 1 
						// so the player hugs the platform tight.
						object_ptr->rigidbody.addForce(Force(bn::fixed_point_t<12>(rigidbody.final_dir.x(), 
																				   rigidbody.final_dir.y() + 1),
															 SCYTHE_PLATFORM_DECAY));
					}
				}

			break;

			default:
			break;
		}

		object_ptr         = NULL;
		other_collider_ptr = NULL;

	}

	// Clean up temp colliders
	delete test_collider_roof_ptr;
    
    ///////////////////
    // Update States //
    ///////////////////

	///////////////////
	// Update Timers //
	///////////////////

	update_counter++;
    if(update_counter >= 60) {update_counter = 0;}

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