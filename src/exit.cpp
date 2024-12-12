#include "exit.h"

Exit::Exit(int32 _go_to_room_enum, bn::point origin)
{
    object_type = EXIT;
    
    sprite_ptr         = bn::sprite_items::exit.create_sprite(0, 0);
    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                  2,
								                                  bn::sprite_items::exit.tiles_item(),
								                                  0,
								                                  0);

    go_to_room_enum = _go_to_room_enum; 
    is_triggered = false;

    collider_offset_x = 0;
	collider_offset_y = 0;

	collider_ptr  = new Collider(x() + collider_offset_x, 
                                 y() + collider_offset_y, 
                                 EXIT_COLLIDER_WIDTH, 
                                 EXIT_COLLIDER_HEIGHT);

    setPos(origin);
}

Exit::~Exit()
{

}

void Exit::update(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects,
				  bn::regular_bg_ptr                         bg_ptr, 
                  bn::span<const bn::regular_bg_map_cell>    cells,
                  bn::regular_bg_item                        bg_item,
                  bn::camera_ptr                             camera)
{
    is_triggered = false;

    ////////////////////////////////
    // Check for Player Collision //
    ////////////////////////////////

    for(int32 i = 0; i < game_objects.size(); i++)
	{
		Collider* other_collider_ptr = game_objects.at(i)->collider_ptr;

		switch(game_objects.at(i)->object_type)
        {
            case PLAYER:

                if(collider_ptr->isCollision(*other_collider_ptr))
                {is_triggered = true;}

            break;

            default:
            break;
        }

        other_collider_ptr = NULL;
    }

}