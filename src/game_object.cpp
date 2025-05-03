#include "game_object.h"

///////////////////////
// Struct GameObject //
///////////////////////

GameObject::GameObject()
{
    sprite_ptr          = bn::sprite_items::game_object.create_sprite(0, 0);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
						 		  								   2,
							  								       bn::sprite_items::game_object.tiles_item(),
								  								   0,
								  								   0);
	
    collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        GAME_OBJECT_COLLIDER_WIDTH, 
                        GAME_OBJECT_COLLIDER_HEIGHT);

    state       = NONE;
    object_type = NO_TYPE;
    x_dir       = RIGHT;
    y_dir       = UP;

    object_id              = 0;
    hit_flash_frames       = 0;
    invulnerability_frames = 0;
}

GameObject::GameObject(const GameObject& other)
{

    state       = other.state;
    object_type = other.object_type;
    object_id   = other.object_id;

    sprite_ptr          = other.sprite_ptr;
    default_palette_ptr = other.default_palette_ptr;
    animate_action_ptr  = other.animate_action_ptr;

    rigidbody = other.rigidbody;

    collider          = other.collider;
    collider_x_axis   = other.collider_x_axis;
    collider_y_axis   = other.collider_y_axis;
    collider_offset_x = other.collider_offset_x;
	collider_offset_y = other.collider_offset_y;

	x_dir = other.x_dir;
    y_dir = other.y_dir;

	is_inactive   = other.is_inactive;
    is_dead       = other.is_dead;
    is_persistent = other.is_persistent;

	received_platform_force = other.received_platform_force;

    hit_flash_frames       = other.hit_flash_frames;
    invulnerability_frames = other.invulnerability_frames;
    hitpoints              = other.hitpoints;

}

GameObject::~GameObject()
{
    sprite_ptr.reset();
    default_palette_ptr.reset();
    animate_action_ptr.reset();
    
    if(object_type != PLAYER)
    {
        rigidbody.removeForces();
    }
}

GameObject& GameObject::operator =(const GameObject& other)
{
    state       = other.state;
    object_type = other.object_type;
    object_id   = other.object_id;

    sprite_ptr          = other.sprite_ptr;
    default_palette_ptr = other.default_palette_ptr;
    animate_action_ptr  = other.animate_action_ptr;

    rigidbody = other.rigidbody;

    collider          = other.collider;
    collider_x_axis   = other.collider_x_axis;
    collider_y_axis   = other.collider_y_axis;
    collider_offset_x = other.collider_offset_x;
	collider_offset_y = other.collider_offset_y;

	x_dir = other.x_dir;
    y_dir = other.y_dir;

	is_inactive   = other.is_inactive;
    is_dead       = other.is_dead;
    is_persistent = other.is_persistent;

	received_platform_force = other.received_platform_force;

    hit_flash_frames       = other.hit_flash_frames;
    invulnerability_frames = other.invulnerability_frames;
    hitpoints              = other.hitpoints;

    return *this;
}

void GameObject::update(const RoomBounds& 							   room_bounds,
                        bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                        const bn::regular_bg_ptr&                      bg_ptr, 
                        const bn::span<const bn::regular_bg_map_cell>& cells,
                        const bn::regular_bg_item&                     bg_item,
                        const bn::camera_ptr&                          camera)
{

    ///////////////////////////////////
    // Update Invulnerability frames //
    ///////////////////////////////////

    invulnerability_frames -= 1;
    if(invulnerability_frames < 0) {invulnerability_frames = 0;}

    /////////////////////////////
	// Update Sprite Direction //
	/////////////////////////////
	
    updateSpriteDirection();
    
    ////////////////////////////
    // Correct Sprite Offsets //
    ////////////////////////////
	
    updateSpriteOffsets();

	//////////////////////
	// Update Hit Flash //
	//////////////////////

	updateHitFlash();

    ////////////////////
    // Clamp Position //
    ////////////////////

    clampPosition(bg_ptr);

	//////////////////////////////
    // Monitor unloading bounds //
    //////////////////////////////
    
	updateInactiveState(camera);

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

void GameObject::setHitStretch()
{
    sprite_ptr->set_horizontal_scale(GAME_OBJECT_MAX_STRETCH_H);				
    sprite_ptr->set_vertical_scale(GAME_OBJECT_MAX_STRETCH_V);
}

void GameObject::setVerticalStretch()
{
    sprite_ptr->set_horizontal_scale(GAME_OBJECT_MIN_STRETCH_H);
    sprite_ptr->set_vertical_scale(GAME_OBJECT_MAX_STRETCH_V);
}

void GameObject::setHorizontalStretch()
{
    sprite_ptr->set_horizontal_scale(GAME_OBJECT_MAX_STRETCH_H);				
    sprite_ptr->set_vertical_scale(GAME_OBJECT_MIN_STRETCH_V);
}

void GameObject::updateSpriteDirection()
{
    if      (x_dir == LEFT)  {sprite_ptr->set_horizontal_flip(true);}
	else if (x_dir == RIGHT) {sprite_ptr->set_horizontal_flip(false);}
    
    if      (y_dir == DOWN)  {sprite_ptr->set_vertical_flip(true);}
	else if (y_dir == UP)    {sprite_ptr->set_vertical_flip(false);}
}

void GameObject::updateSpriteOffsets()
{
    bn::fixed h_scale   = sprite_ptr->horizontal_scale();
    bn::fixed v_scale   = sprite_ptr->vertical_scale();
    bn::fixed increment = 0.1;

    // Correct H Scale
    if(h_scale > 1) {sprite_ptr->set_horizontal_scale(h_scale - increment);}
    else if (h_scale < 1) {sprite_ptr->set_horizontal_scale(h_scale + increment);}
    if(abs(1 - sprite_ptr->horizontal_scale()) < increment) {sprite_ptr->set_horizontal_scale(1);}
    
    // Correct V Scale
    if(v_scale > 1) {sprite_ptr->set_vertical_scale(v_scale - increment);}
    else if (v_scale < 1) {sprite_ptr->set_vertical_scale(v_scale + increment);}
    if(abs(1 - sprite_ptr->vertical_scale()) < increment) {sprite_ptr->set_vertical_scale(1);}
}

void GameObject::updateInactiveState(const bn::camera_ptr& camera)
{
    if(is_persistent) {return;}

    Collider load_range_collider(camera.position().x(), 
                                 camera.position().y(), 
                                 LOAD_RANGE_W, 
                                 LOAD_RANGE_H);

    if(!load_range_collider.isCollision(pos()))
    {is_inactive = true;}
}

void GameObject::clampPosition(const bn::regular_bg_ptr& bg_ptr)
{
    int32 half_level_width_pixels  = bg_ptr.dimensions().width() / 2;
	int32 half_level_height_pixels = bg_ptr.dimensions().height() / 2;
    bn::fixed new_x = x();
	bn::fixed new_y = y();
	new_x = clamp(-half_level_width_pixels,  half_level_width_pixels,  new_x);
	new_y = clamp(-half_level_height_pixels, half_level_height_pixels, new_y);

	setPos(new_x, new_y);
}

void GameObject::setHitFlash()
{
    hit_flash_frames = GAME_OBJECT_MAX_HIT_FLASH_FRAMES;

    bn::sprite_palette_ptr sprite_palette = bn::sprite_palette_items::sprite_flash_palette.create_palette();
    sprite_ptr->set_palette(sprite_palette);
}

void GameObject::setHitFlash(int32 frames)
{
    if(frames > GAME_OBJECT_MAX_HIT_FLASH_FRAMES) 
    {frames = GAME_OBJECT_MAX_HIT_FLASH_FRAMES;}

    hit_flash_frames = frames;

    bn::sprite_palette_ptr sprite_palette = bn::sprite_palette_items::sprite_flash_palette.create_palette();
    sprite_ptr->set_palette(sprite_palette);
}

void GameObject::updateHitFlash()
{
    
    if(hit_flash_frames)
    {
        bn::sprite_palette_ptr sprite_palette = bn::sprite_palette_items::sprite_flash_palette.create_palette();
        sprite_ptr->set_palette(sprite_palette);
    }
    else
    {
        sprite_ptr->set_palette(default_palette_ptr.value());
    }

    hit_flash_frames--;
    if(hit_flash_frames < 0)
    {hit_flash_frames = 0;}
}

void GameObject::applyDamage(int32 damage)
{
    hitpoints -= damage;
    if(hitpoints < 0) {hitpoints = 0;}
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