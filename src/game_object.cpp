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

    rigidbody_ptr = new RigidBody();
	collider_ptr  = new Collider(x() + collider_offset_x, 
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

    collider_ptr  = new Collider(*(other.collider_ptr));
    rigidbody_ptr = new RigidBody(*(other.rigidbody_ptr));

    collider_offset_x = other.collider_offset_x;
	collider_offset_y = other.collider_offset_y;

	dir = other.dir;

	inactive = other.inactive;

	received_platform_force = other.received_platform_force;

}

GameObject::~GameObject()
{
    sprite_ptr.reset();
    animate_action_ptr.reset();
    sprite_palette_ptr.reset();
    
    delete collider_ptr;
    delete rigidbody_ptr;
}

void GameObject::applyForces()
{
    // Apply all of the forces in the RigidBody to the object
    bn::fixed_point final_dir(0, 0);
    
    bn::ivector<Force*>::iterator current = rigidbody_ptr->forces.begin();
    bn::ivector<Force*>::iterator last    = rigidbody_ptr->forces.end();
    while(current != last)
    {
        // Update object position with new force
        setX(x() + (*current)->x());
        setY(y() + (*current)->y());

        // Update final dir vector with new force
        final_dir.set_x(final_dir.x() + (*current)->x());
        final_dir.set_y(final_dir.y() + (*current)->y());

        // Update iterator
        current++;
    }

    bn::fixed normalized_dir_x = 0;
	bn::fixed normalized_dir_y = 0;
	if(final_dir.x() != 0) {normalized_dir_x = final_dir.x() / abs(final_dir.x().integer());}
	if(final_dir.y() != 0) {normalized_dir_y = final_dir.y() / abs(final_dir.y().integer());}

    rigidbody_ptr->normalized_dir = bn::fixed_point(normalized_dir_x, normalized_dir_y);
    rigidbody_ptr->final_dir      = final_dir;
}

void GameObject::draw()
{
    animate_action_ptr->update();
}

void GameObject::setCamera(const bn::camera_ptr& camera)
{
    sprite_ptr->set_camera(camera);
    collider_ptr->setCamera(camera);
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
    collider_ptr->setX(new_x.integer() + collider_offset_x);
}

void GameObject::setY(bn::fixed new_y)
{
    sprite_ptr->set_y(new_y.integer());
    collider_ptr->setY(new_y.integer() + collider_offset_y);
}

void GameObject::setPos(bn::fixed new_x, bn::fixed new_y)
{
    sprite_ptr->set_x(new_x.integer());
    sprite_ptr->set_y(new_y.integer());
	collider_ptr->setX(new_x.integer() + collider_offset_x);
	collider_ptr->setY(new_y.integer() + collider_offset_y);
}

void GameObject::setPos(bn::fixed_point new_pos)
{
    sprite_ptr->set_x(new_pos.x().integer());
    sprite_ptr->set_y(new_pos.y().integer());
	collider_ptr->setX(new_pos.x().integer() + collider_offset_x);
	collider_ptr->setY(new_pos.y().integer() + collider_offset_y);
}

int32 GameObject::getTileAtBGIndex(uint32 x, uint32 y, 
                                   bn::regular_bg_ptr                      bg_ptr, 
                                   bn::span<const bn::regular_bg_map_cell> cells,
                                   bn::regular_bg_item                     bg_item) const
{

    // Returns -1 if input range is invalid. 
    if(x > (uint32)((bg_ptr.dimensions().width() / 8) - 1) ||
       y > (uint32)((bg_ptr.dimensions().height() / 8) - 1))
    {return -1;}

    bn::regular_bg_map_cell       cell_index = cells[bg_item.map_item().cell_index(x, y)];
    bn::regular_bg_map_cell_info  cell_info(cell_index);    

    return cell_info.tile_index();

}