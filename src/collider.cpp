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
    return sprite_ptr->x().floor_integer();
}

bn::fixed Collider::y() const
{
    return sprite_ptr->y().floor_integer();
}

bn::fixed_point Collider::pos() const
{
    bn::fixed_point point(sprite_ptr->position().x().floor_integer(),
			  sprite_ptr->position().y().floor_integer());
    return point;
}

void Collider::setX(bn::fixed new_x)
{
    sprite_ptr->set_x(new_x.floor_integer());
    update();
}

void Collider::setY(bn::fixed new_y)
{
    sprite_ptr->set_y(new_y.floor_integer());
    update();
}

void Collider::setPos(bn::fixed new_x, bn::fixed new_y)
{
    sprite_ptr->set_x(new_x.floor_integer());
    sprite_ptr->set_y(new_y.floor_integer());
    update();
}

void Collider::setPos(bn::fixed_point new_pos)
{
    sprite_ptr->set_x(new_pos.x().floor_integer());
    sprite_ptr->set_y(new_pos.y().floor_integer());
    update();
}

void Collider::update()
{
    bn::fixed half_size = (size / 2) - 0.5;
    p1.set_x((x() - (half_size)));
    p1.set_y((y() - (half_size)));
    
    p2.set_x((x() + (half_size)));
    p2.set_y((y() - (half_size)));
    
    p3.set_x((x() - (half_size)));
    p3.set_y((y() + (half_size)));
    
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
    if(isCollision(target.p1) || isCollision(target.p2) ||
       isCollision(target.p3) || isCollision(target.p4) ||
       target.isCollision(p1) || target.isCollision(p2) ||
       target.isCollision(p3) || target.isCollision(p4))
    {
	return true;
    }
    return false;
}
