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
    return sprite_ptr->x().integer();
}

bn::fixed GameObject::y() const
{
    return sprite_ptr->y().integer();
}

bn::fixed_point GameObject::pos() const
{
    bn::fixed_point point(sprite_ptr->position().x().integer(),
			  sprite_ptr->position().y().integer());
    return point; 
}

void GameObject::setX(bn::fixed new_x)
{
    sprite_ptr->set_x(new_x.integer());
    if(collider_p) {collider_p->setX(new_x.integer());}
}

void GameObject::setY(bn::fixed new_y)
{
    sprite_ptr->set_y(new_y.integer());
    if(collider_p) {collider_p->setY(new_y.integer());}
}

void GameObject::setPos(bn::fixed new_x, bn::fixed new_y)
{
    sprite_ptr->set_x(new_x.integer());
    sprite_ptr->set_y(new_y.integer());
    if(collider_p)
    {
        collider_p->setX(new_x.integer());
        collider_p->setY(new_y.integer());
    }
}

void GameObject::setPos(bn::fixed_point new_pos)
{
    sprite_ptr->set_x(new_pos.x().integer());
    sprite_ptr->set_y(new_pos.y().integer());
    if(collider_p)
    {
        collider_p->setX(new_pos.x().integer());
        collider_p->setY(new_pos.y().integer());
    }
}
