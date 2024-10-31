#include "collider.h"

Collider::Collider()
{
    size = COLLIDER_8;

    sprite_ptr = bn::sprite_items::collider_8.create_sprite(0, 0);
    if(!DEBUG_BUILD) {sprite_ptr->set_visible(false);}

    update();
}

Collider::Collider(bn::fixed origin_x,
		   bn::fixed origin_y,
		   ColliderSize _size)
{
    size = _size;
    
    switch(size)
    {
    case COLLIDER_8:
        sprite_ptr = bn::sprite_items::collider_8.create_sprite(origin_x, origin_y);
        break;
    case COLLIDER_16:
        sprite_ptr = bn::sprite_items::collider_16.create_sprite(origin_x, origin_y);
        break;
    case COLLIDER_32:
        sprite_ptr = bn::sprite_items::collider_32.create_sprite(origin_x, origin_y);
        break;
    default:
	    BN_LOG("ERROR: Collider sprite not found - Invalid collider type.");
	    return;
	    break;
    }
    if(!DEBUG_BUILD) {sprite_ptr->set_visible(false);}

    update();
}

bn::fixed Collider::x() const
{
    return sprite_ptr->x().integer();
}

bn::fixed Collider::y() const
{
    return sprite_ptr->y().integer();
}

bn::fixed_point Collider::pos() const
{
    bn::fixed_point point(sprite_ptr->position().x().integer(),
			              sprite_ptr->position().y().integer());
    return point;
}

void Collider::setX(bn::fixed new_x)
{
    sprite_ptr->set_x(new_x.integer());
    update();
}

void Collider::setY(bn::fixed new_y)
{
    sprite_ptr->set_y(new_y.integer());
    update();
}

void Collider::setPos(bn::fixed new_x, bn::fixed new_y)
{
    sprite_ptr->set_x(new_x.integer());
    sprite_ptr->set_y(new_y.integer());
    update();
}

void Collider::setPos(bn::fixed_point new_pos)
{
    sprite_ptr->set_x(new_pos.x().integer());
    sprite_ptr->set_y(new_pos.y().integer());
    update();
}

void Collider::update()
{
    bn::fixed half_size = (size / 2) - 0.5;
    p1.set_x((x() - (half_size)));
    p1.set_y((y() - (half_size)));
    
    p4.set_x((x() + (half_size)));
    p4.set_y((y() + (half_size)));
}

void Collider::draw()
{
    animate_action_ptr->update();
}

void Collider::setCamera(const bn::camera_ptr& camera)
{
    sprite_ptr->set_camera(camera);
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
