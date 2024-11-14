#include "one_way_block.h"

//////////////////
// Struct Block //
//////////////////

OneWayBlock::OneWayBlock(uint32 width)
{
    /*
    object_type       = ONE_WAY_BLOCK; 
    collider_ptr      = new Collider(x(), 
                                     y(), 
                                     width, 
                                     ONEWAYBLOCK_COLLIDER_HEIGHT);
    collider_offset_x =  0;
    collider_offset_y = -4;
    */
}

OneWayBlock::~OneWayBlock()
{
    delete collider_ptr;
}

void OneWayBlock::update(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects, 
                         const Room& room)
{

}

void OneWayBlock::draw()
{

}

void OneWayBlock::setCamera(const bn::camera_ptr& camera)
{
    collider_ptr->setCamera(camera);
}

bn::fixed OneWayBlock::x() const
{
    return collider_ptr->x().integer();
}

bn::fixed OneWayBlock::y() const
{
    return collider_ptr->y().integer();
}

bn::fixed_point OneWayBlock::pos() const
{
    bn::fixed_point point(collider_ptr->x().integer(),
			              collider_ptr->y().integer());
    return point;
}

void OneWayBlock::setX(bn::fixed new_x)
{
    collider_ptr->setX(new_x.integer() + collider_offset_x);
}

void OneWayBlock::setY(bn::fixed new_y)
{
    collider_ptr->setY(new_y.integer() + collider_offset_y);
}

void OneWayBlock::setPos(bn::fixed new_x, bn::fixed new_y)
{
    collider_ptr->setX(new_x.integer() + collider_offset_x);
    collider_ptr->setY(new_y.integer() + collider_offset_y);
}

void OneWayBlock::setPos(bn::fixed_point new_pos)
{
    collider_ptr->setX(new_pos.x().integer() + collider_offset_x);
    collider_ptr->setY(new_pos.y().integer() + collider_offset_y);
}