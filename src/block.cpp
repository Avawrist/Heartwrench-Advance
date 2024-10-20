#include "block.h"

//////////////////
// Struct Block //
//////////////////

Block::Block()
{
    sprite_ptr = bn::sprite_items::block.create_sprite(0, 0);
    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  2,
								  bn::sprite_items::block.tiles_item(),
								  0,
								  0);

    collider_p  = new Collider(x(), y(), COLLIDER_32);
}

Block::~Block()
{
    delete collider_p;
}

void Block::update(GameObject** game_objects_p, uint8 game_objects_size)
{

}

