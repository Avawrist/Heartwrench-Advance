#include "block.h"

//////////////////
// Struct Block //
//////////////////

Block::Block(uint32 width, uint32 height)
{
    collider_ptr = new Collider(x(), y(), width, height);
}

Block::~Block()
{
    delete collider_ptr;
}

void Block::update(GameObject** game_objects_p, uint32 game_objects_size)
{

}

void Block::draw()
{

}

void Block::setCamera(const bn::camera_ptr& camera)
{
    collider_ptr->setCamera(camera);
}

bn::fixed Block::x() const
{
    return collider_ptr->x().integer();
}

bn::fixed Block::y() const
{
    return collider_ptr->y().integer();
}

bn::fixed_point Block::pos() const
{
    bn::fixed_point point(collider_ptr->x().integer(),
			              collider_ptr->y().integer());
    return point;
}

void Block::setX(bn::fixed new_x)
{
    collider_ptr->setX(new_x.integer());
}

void Block::setY(bn::fixed new_y)
{
    collider_ptr->setY(new_y.integer());
}

void Block::setPos(bn::fixed new_x, bn::fixed new_y) 
{
    collider_ptr->setX(new_x.integer());
    collider_ptr->setY(new_y.integer());
}

void Block::setPos(bn::fixed_point new_pos)
{
    collider_ptr->setX(new_pos.x().integer());
    collider_ptr->setY(new_pos.y().integer());
}

