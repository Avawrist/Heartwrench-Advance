#include "collider.h"

Collider::Collider()
{
    size = COLLIDER_8x;

    p1 = bn::fixed_point(0 - (size/2), 0 - (size/2));
    p2 = bn::fixed_point(0 + (size/2), 0 + (size/2));
    p3 = bn::fixed_point(0 - (size/2), 0 + (size/2));
    p4 = bn::fixed_point(0 + (size/2), 0 + (size/2));

    switch(size)
    {
    case COLLIDER_8x:
	sprite_ptr = bn::sprite_items::debug_rect_8_sprite.create_sprite(0, 0);
	break;
    case COLLIDER_16x:
	sprite_ptr = bn::sprite_items::debug_rect_16_sprite.create_sprite(0, 0);
	break;
    case COLLIDER_32x:
	sprite_ptr = bn::sprite_items::debug_rect_32_sprite.create_sprite(0, 0);
	break;
    default:
	break;
    }
    sprite_ptr->set_z_order(COLLIDER_Z_ORDER);
}

Collider::Collider(bn::fixed _origin_x,
		   bn::fixed _origin_y,
		   ColliderSize _size)
{
    size = _size;

    p1 = bn::fixed_point(_origin_x - (size/2), _origin_y - (size/2));
    p2 = bn::fixed_point(_origin_x + (size/2), _origin_y - (size/2));
    p3 = bn::fixed_point(_origin_x - (size/2), _origin_y + (size/2));
    p4 = bn::fixed_point(_origin_x + (size/2), _origin_y + (size/2));
    
    switch(size)
    {
    case COLLIDER_8x:
	sprite_ptr = bn::sprite_items::debug_rect_8_sprite.create_sprite(_origin_x, _origin_y);
	break;
    case COLLIDER_16x:
	sprite_ptr = bn::sprite_items::debug_rect_16_sprite.create_sprite(_origin_x, _origin_y);
	break;
    case COLLIDER_32x:
	sprite_ptr = bn::sprite_items::debug_rect_32_sprite.create_sprite(_origin_x, _origin_y);
	break;
    default:
	break;
    }
    sprite_ptr->set_z_order(COLLIDER_Z_ORDER);
}

bn::fixed Collider::x() const
{
    return sprite_ptr->x();
}

bn::fixed Collider::y() const
{
    return sprite_ptr->y();
}

bn::fixed_point Collider::position() const
{
    return sprite_ptr->position();
}

void Collider::setX(bn::fixed new_x)
{
    sprite_ptr->set_x(new_x);
    updatePoints();
}

void Collider::setY(bn::fixed new_y)
{
    sprite_ptr->set_y(new_y);
    updatePoints();
}

void Collider::setPos(bn::fixed new_x, bn::fixed new_y)
{
    sprite_ptr->set_x(new_x);
    sprite_ptr->set_y(new_y);
    updatePoints();
}

void Collider::setPos(bn::fixed_point new_pos)
{
    sprite_ptr->set_position(new_pos);
    updatePoints();
}

void Collider::updatePoints()
{
    p1.set_x(x() - (size/2));
    p1.set_y(y() - (size/2));
    p2.set_x(x() + (size/2));
    p2.set_y(y() - (size/2));    
    p3.set_x(x() - (size/2));
    p3.set_y(y() + (size/2));
    p4.set_x(x() + (size/2));
    p4.set_y(y() + (size/2));
}

void Collider::setCamera(const bn::camera_ptr& camera)
{
    sprite_ptr->set_camera(camera);
}

bool Collider::pointIsColliding(bn::fixed_point test_p) const
{
    if(test_p.x() > p1.x() && test_p.x() < p4.x() &&
       test_p.y() > p1.y() && test_p.y() < p4.y())
    {
	return true;
    }
    return false;
}

bool Collider::willCollide(const Collider& target, bn::fixed_point dir) const
{
    bn::fixed_point test_p1 = p1 + dir;
    bn::fixed_point test_p2 = p2 + dir;
    bn::fixed_point test_p3 = p3 + dir;
    bn::fixed_point test_p4 = p4 + dir;
    
    if(target.pointIsColliding(test_p1) || target.pointIsColliding(test_p2) ||
       target.pointIsColliding(test_p3) || target.pointIsColliding(test_p4))
    {
	return true;
    }
    return false;
}
