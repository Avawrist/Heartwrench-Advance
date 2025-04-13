#include "game_object.h"

///////////////////////
// Struct GameObject //
///////////////////////

GameObject::GameObject()
{
    object_id = 0;

    sprite_ptr         = bn::sprite_items::game_object.create_sprite(0, 0);
    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  2,
								  								  bn::sprite_items::game_object.tiles_item(),
								  								  0,
								  								  0);
    sprite_palette_ptr = bn::sprite_palette_items::default_sprite_palette.create_palette();
	
    collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        GAME_OBJECT_COLLIDER_WIDTH, 
                        GAME_OBJECT_COLLIDER_HEIGHT);
}

GameObject::GameObject(const GameObject& other)
{

    object_type = other.object_type;
    object_id   = other.object_id;

    sprite_ptr         = other.sprite_ptr;
    animate_action_ptr = other.animate_action_ptr;
    sprite_palette_ptr = other.sprite_palette_ptr;

    rigidbody = other.rigidbody;

    collider          = other.collider;
    collider_x_axis   = other.collider_x_axis;
    collider_y_axis   = other.collider_y_axis;
    collider_offset_x = other.collider_offset_x;
	collider_offset_y = other.collider_offset_y;

	dir = other.dir;

	is_inactive   = other.is_inactive;
    is_dead       = other.is_dead;
    is_persistent = other.is_persistent;

	received_platform_force = other.received_platform_force;

}

GameObject::~GameObject()
{
    sprite_ptr.reset();
    animate_action_ptr.reset();
    sprite_palette_ptr.reset();
    
    if(object_type != PLAYER)
    {
        rigidbody.removeForces();
    }
}

GameObject& GameObject::operator =(const GameObject& other)
{
    object_type = other.object_type;
    object_id   = other.object_id;

    sprite_ptr         = other.sprite_ptr;
    animate_action_ptr = other.animate_action_ptr;
    sprite_palette_ptr = other.sprite_palette_ptr;

    rigidbody = other.rigidbody;

    collider          = other.collider;
    collider_x_axis   = other.collider_x_axis;
    collider_y_axis   = other.collider_y_axis;
    collider_offset_x = other.collider_offset_x;
	collider_offset_y = other.collider_offset_y;

	dir = other.dir;

	is_inactive   = other.is_inactive;
    is_dead       = other.is_dead;
    is_persistent = other.is_persistent;

	received_platform_force = other.received_platform_force;

    return *this;
}

void GameObject::applyForces()
{
    // Apply all of the forces in the RigidBody to the object
    bn::fixed_point final_dir(0, 0);
    
    bn::ivector<Force>::iterator current = rigidbody.forces.begin();
    bn::ivector<Force>::iterator last    = rigidbody.forces.end();
    while(current != last)
    {
        // Update object position with new force
        setX(x() + current->x());
        setY(y() + current->y());

        // Update final dir vector with new force
        final_dir.set_x(final_dir.x() + current->x());
        final_dir.set_y(final_dir.y() + current->y());

        // Update iterator
        current++;
    }

    bn::fixed normalized_dir_x = 0;
	bn::fixed normalized_dir_y = 0;
	if(final_dir.x() != 0) {normalized_dir_x = final_dir.x() / abs(final_dir.x().integer());}
	if(final_dir.y() != 0) {normalized_dir_y = final_dir.y() / abs(final_dir.y().integer());}

    rigidbody.normalized_dir = bn::fixed_point(normalized_dir_x, normalized_dir_y);
    rigidbody.final_dir      = final_dir;
}

void GameObject::draw()
{
    animate_action_ptr->update();
}

void GameObject::setCamera(const bn::camera_ptr& camera)
{
    sprite_ptr->set_camera(camera);
    collider.setCamera(camera);
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
    bn::fixed_point point(sprite_ptr->position().x().integer() + collider_offset_x,
			              sprite_ptr->position().y().integer() + collider_offset_y);
    return point; 
}

void GameObject::setX(bn::fixed new_x)
{
    sprite_ptr->set_x(new_x.integer());
    collider.setX(new_x.integer() + collider_offset_x);
}

void GameObject::setY(bn::fixed new_y)
{
    sprite_ptr->set_y(new_y.integer());
    collider.setY(new_y.integer() + collider_offset_y);
}

void GameObject::setPos(bn::fixed new_x, bn::fixed new_y)
{
    sprite_ptr->set_x(new_x.integer());
    sprite_ptr->set_y(new_y.integer());
	collider.setX(new_x.integer() + collider_offset_x);
	collider.setY(new_y.integer() + collider_offset_y);
}

void GameObject::setPos(bn::fixed_point new_pos)
{
    sprite_ptr->set_x(new_pos.x().integer());
    sprite_ptr->set_y(new_pos.y().integer());
	collider.setX(new_pos.x().integer() + collider_offset_x);
	collider.setY(new_pos.y().integer() + collider_offset_y);
}

void GameObject::updateInactiveState(const bn::camera_ptr& camera)
{
    if(is_persistent) {return;}

    if(pos().x() < camera.position().x() - LOAD_RANGE_HALF_W || 
       pos().x() > camera.position().x() + LOAD_RANGE_HALF_W ||
       pos().y() < camera.position().y() - LOAD_RANGE_HALF_H ||
       pos().y() > camera.position().y() + LOAD_RANGE_HALF_H)
    {is_inactive = true;}
}

///////////////////////////
// Struct UnloadedObject //
///////////////////////////

UnloadedObject::UnloadedObject()
{
    room_pos           = bn::point(0, 0);
    object_type        = NO_TYPE;
    loaded_instance_id = UNLOADED_OBJECT_STATE_UNLOADED;
}

UnloadedObject::UnloadedObject(bn::point _room_pos, ObjectType _object_type)
{
    room_pos           = _room_pos;
    object_type        = _object_type;
    loaded_instance_id = UNLOADED_OBJECT_STATE_UNLOADED;
}

UnloadedObject::UnloadedObject(const UnloadedObject& other)
{
    room_pos           = other.room_pos;
    object_type        = other.object_type;
    loaded_instance_id = other.loaded_instance_id;
    is_persistent      = other.is_persistent;
}

UnloadedObject::~UnloadedObject()
{

}

void UnloadedObject::operator =(const UnloadedObject& other)
{
    room_pos           = other.room_pos;
    object_type        = other.object_type;
    loaded_instance_id = other.loaded_instance_id;
    is_persistent      = other.is_persistent;
}