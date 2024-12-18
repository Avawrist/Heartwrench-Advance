#include "scythe_platform.h"

ScythePlatform::ScythePlatform(Direction _dir, bn::fixed_point _p)
{
    // Reset Variables //
    sprite_ptr.reset();
    animate_action_ptr.reset();
    delete rigidbody_ptr;
    delete collider_ptr;

    // Init Variables //
    object_type = SCYTHE_PLATFORM;
    sprite_ptr  = bn::sprite_items::scythe_platform.create_sprite(0, 0);
    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                  2,
								                                  bn::sprite_items::scythe_platform.tiles_item(),
								                                  0,
								                                  0);
	
	// Make invisible for the first frame 
	// so we don't see ugly collisions on creation
	sprite_ptr->set_visible(false);

    collider_offset_x = 0;
    collider_offset_y = 0;

    rigidbody_ptr = new RigidBody();
	collider_ptr  = new Collider(x() + collider_offset_x, 
                                 y() + collider_offset_y, 
                                 SCYTHE_PLATFORM_COLLIDER_WIDTH, 
                                 SCYTHE_PLATFORM_COLLIDER_HEIGHT);

    state             = STATE_THROWN;
	player_was_riding = false;
    dir               = _dir;

	// Apply throw force
	rigidbody_ptr->addForce(SCYTHE_PLATFORM_THROW_FORCE);

	// Set initial stretch
	sprite_ptr->set_vertical_scale(SCYTHE_MIN_STRETCH_V);
	sprite_ptr->set_horizontal_scale(SCYTHE_MAX_STRETCH_H);

    setPos(_p);
}

ScythePlatform::~ScythePlatform()
{

}

void ScythePlatform::update(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects,
							bn::regular_bg_ptr                         bg_ptr, 
                        	bn::span<const bn::regular_bg_map_cell>    cells,
                        	bn::regular_bg_item                        bg_item,
							bn::camera_ptr                             camera)
{

    ///////////////////
    // State Machine //
    ///////////////////

    switch(state)
    {
        case STATE_THROWN:

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
    rigidbody_ptr->applyDecay();

    // Apply forces to scythe platform
    bn::fixed_point final_dir = applyForces();

	///////////////////////////////////
	// Resolve Game Object Collision //
	///////////////////////////////////

	Collider* other_collider_ptr = NULL;

	// Create test collider for roof collision checks
	#define SCYTHE_PLATFORM_ROOF_OFFSET           -3
	#define SCYTHE_PLATFORM_ROOF_COLLIDER_HEIGHT   1

	Collider* test_collider_roof_ptr = new Collider(collider_ptr->x(),
													collider_ptr->y() + SCYTHE_PLATFORM_ROOF_OFFSET,
													collider_ptr->width,
													SCYTHE_PLATFORM_ROOF_COLLIDER_HEIGHT);

	for(int32 i = 0; i < game_objects.size(); i++)
	{

		GameObject* object_ptr = game_objects.at(i);
		other_collider_ptr     = object_ptr->collider_ptr;

		switch(object_ptr->object_type)
		{
			case PLAYER:
				
				// If player is riding the platform:
				if(!object_ptr->received_platform_force && 
				test_collider_roof_ptr->isCollision(*other_collider_ptr) &&
				other_collider_ptr->p4.y() < collider_ptr->p1.y() - final_dir.y())
				{
					
					if(final_dir.y() <= 0)
					{
						// If descending, applying force to the x axis is all that's needed.
						// The player gravity will take care of the rest. 
						object_ptr->rigidbody_ptr->addForce(new Force(bn::fixed_point_t<12>(final_dir.x(), 0),
																	  SCYTHE_PLATFORM_DECAY));
					}
					else
					{
						// If ascending, apply force to BOTH axes and offset y by 1 
						// so the player hugs the platform tight.
						object_ptr->rigidbody_ptr->addForce(new Force(bn::fixed_point_t<12>(final_dir.x(), final_dir.y() + 1),
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

    //////////////////////
	// Update Direction //
	//////////////////////

    if(dir == LEFT)        {sprite_ptr->set_horizontal_flip(true);}
	else if (dir == RIGHT) {sprite_ptr->set_horizontal_flip(false);}

	////////////////////////////
    // Correct Sprite Offsets //
    ////////////////////////////
	
    bn::fixed h_scale   = sprite_ptr->horizontal_scale();
    bn::fixed v_scale   = sprite_ptr->vertical_scale();
    bn::fixed increment = 0.1;

    // Correct H Scale
    if(h_scale > 1) {sprite_ptr->set_horizontal_scale(h_scale - increment);}
    else if (h_scale < 1) {sprite_ptr->set_horizontal_scale(h_scale + increment);}
    if(abs(1 - sprite_ptr->horizontal_scale()) < increment) {sprite_ptr->set_horizontal_scale(1);}
    
    // Correct V Scale
    if(v_scale > 1) {sprite_ptr->set_vertical_scale(v_scale - increment);}
    else if (v_scale < 1) {sprite_ptr->set_vertical_scale(v_scale + increment);}
    if(abs(1 - sprite_ptr->vertical_scale()) < increment) {sprite_ptr->set_vertical_scale(1);}
}