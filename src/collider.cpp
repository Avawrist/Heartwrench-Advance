#include "collider.h"

Collider::Collider()
{
    position  = bn::fixed_point(0, 0);
    width     = 8;
    height    = 8;

    update();
}

Collider::Collider(bn::fixed origin_x,
		           bn::fixed origin_y,
		           uint32 _width,
                   uint32 _height)
{
    position  = bn::fixed_point(origin_x, origin_y);
    width  = _width;
    height = _height;
    
    update();
}

Collider::Collider(const Collider& other)
{

    width    = other.width;
	height   = other.height;
	position = other.position;
	p1       = other.p1;
	p4       = other.p4;

}

Collider& Collider::operator =(const Collider& other)
{

    width    = other.width;
	height   = other.height;
	position = other.position;
	p1       = other.p1;
	p4       = other.p4;

    return *this;

}

bn::fixed Collider::x() const
{
    return position.x().integer();
}

bn::fixed Collider::y() const
{
    return position.y().integer();
}

bn::fixed_point Collider::pos() const
{
    return position;
}

void Collider::setX(bn::fixed new_x)
{
    position.set_x(new_x.integer());
    update();
}

void Collider::setY(bn::fixed new_y)
{
    position.set_y(new_y.integer());
    update();
}

void Collider::setPos(bn::fixed new_x, bn::fixed new_y)
{
    position = bn::fixed_point(new_x.integer(), new_y.integer());
    update();
}

void Collider::setPos(bn::fixed_point new_pos)
{
    position = new_pos;
    update();
}

void Collider::update()
{
    //bn::fixed half_size = (size / 2) - 0.5;
    bn::fixed half_w = (width / 2) - 0.5;
    bn::fixed half_h = (height / 2) - 0.5;
    p1.set_x((x() - (half_w)));
    p1.set_y((y() - (half_h)));
    
    p4.set_x((x() + (half_w)));
    p4.set_y((y() + (half_h)));
}

void Collider::draw()
{

}

void Collider::setCamera(const bn::camera_ptr& camera)
{

}

bool Collider::isCollision(bn::fixed_point target)
{
    if(target.x() >= p1.x() && target.x() <= p4.x() &&
       target.y() >= p1.y() && target.y() <= p4.y())
    {
	    return true;
    }
    return false;
}

bool Collider::isCollision(Collider& target)
{
    if(p1.x() <= target.p4.x() &&
       p4.x() >= target.p1.x() &&
       p1.y() <= target.p4.y() &&
       p4.y() >= target.p1.y())
    {
        return true;
    }
    return false;
}
