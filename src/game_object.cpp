#include "game_object.h"

///////////////////////
// Struct GameObject //
///////////////////////

void GameObject::draw()
{
    animate_action_ptr->update();
}

void GameObject::setCamera(const bn::camera_ptr& camera)
{
    sprite_ptr->set_camera(camera);
    if(collider_p) {collider_p->setCamera(camera);}
}

bn::fixed GameObject::x() const
{
    return sprite_ptr->x().floor_integer();
}

bn::fixed GameObject::y() const
{
    return sprite_ptr->y().floor_integer();
}

bn::fixed_point GameObject::pos() const
{
    bn::fixed_point point(sprite_ptr->position().x().floor_integer(),
			  sprite_ptr->position().y().floor_integer());
    return point; 
}

void GameObject::setX(bn::fixed new_x)
{
    sprite_ptr->set_x(new_x.floor_integer());
    if(collider_p) {collider_p->setX(new_x.floor_integer());}
}

void GameObject::setY(bn::fixed new_y)
{
    sprite_ptr->set_y(new_y.floor_integer());
    if(collider_p) {collider_p->setY(new_y.floor_integer());}
}

void GameObject::setPos(bn::fixed new_x, bn::fixed new_y)
{
    sprite_ptr->set_x(new_x.floor_integer());
    sprite_ptr->set_y(new_y.floor_integer());
    if(collider_p)
    {
	collider_p->setX(new_x.floor_integer());
	collider_p->setY(new_y.floor_integer());
    }
}

void GameObject::setPos(bn::fixed_point new_pos)
{
    sprite_ptr->set_x(new_pos.x().floor_integer());
    sprite_ptr->set_y(new_pos.y().floor_integer());
    if(collider_p)
    {
	collider_p->setX(new_pos.x().floor_integer());
	collider_p->setY(new_pos.y().floor_integer());
    }
}
