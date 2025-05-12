#include "game_object.h"

///////////////////////
// Struct GameObject //
///////////////////////

GameObject::GameObject()
{
    hit_effect_sprite_ptr = bn::sprite_items::hit_effect.create_sprite(0, 0);
    hit_effect_sprite_ptr->set_z_order(HIT_EFFECT_Z_ORDER);
    hit_effect_sprite_ptr->set_visible(false);

    splat_effect_sprite_ptr = bn::sprite_items::wall_splat_effect.create_sprite(0, 0);
    splat_effect_sprite_ptr->set_z_order(SPLAT_EFFECT_Z_ORDER);
    splat_effect_sprite_ptr->set_visible(false);

    collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        GAME_OBJECT_COLLIDER_WIDTH, 
                        GAME_OBJECT_COLLIDER_HEIGHT);

    col_x_offset = 0;
	col_y_offset = 0;

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

    hit_effect_sprite_ptr         = other.hit_effect_sprite_ptr;
    hit_effect_animate_action_ptr = other.hit_effect_animate_action_ptr;

    splat_effect_sprite_ptr         = other.splat_effect_sprite_ptr;
    splat_effect_animate_action_ptr = other.splat_effect_animate_action_ptr;

    rigidbody = other.rigidbody;

    collider          = other.collider;
    collider_x_axis   = other.collider_x_axis;
    collider_y_axis   = other.collider_y_axis;
    collider_offset_x = other.collider_offset_x;
	collider_offset_y = other.collider_offset_y;

    col_x_offset = other.col_x_offset;
    col_y_offset = other.col_y_offset;

	x_dir = other.x_dir;
    y_dir = other.y_dir;

	is_inactive   = other.is_inactive;
    is_dead       = other.is_dead;
    is_persistent = other.is_persistent;

	received_platform_force = other.received_platform_force;

    hitstun_frames         = other.hitstun_frames;
    hit_flash_frames       = other.hit_flash_frames;
    invulnerability_frames = other.invulnerability_frames;
    hitpoints              = other.hitpoints;

}

GameObject::~GameObject()
{
    sprite_ptr.reset();
    default_palette_ptr.reset();
    animate_action_ptr.reset();

    hit_effect_sprite_ptr.reset();
    hit_effect_animate_action_ptr.reset();

    splat_effect_sprite_ptr.reset();
    splat_effect_animate_action_ptr.reset();
    
    if(object_type != PLAYER)
    {rigidbody.removeForces();}
}

GameObject& GameObject::operator =(const GameObject& other)
{
    state       = other.state;
    object_type = other.object_type;
    object_id   = other.object_id;

    sprite_ptr          = other.sprite_ptr;
    default_palette_ptr = other.default_palette_ptr;
    animate_action_ptr  = other.animate_action_ptr;

    hit_effect_sprite_ptr         = other.hit_effect_sprite_ptr;
    hit_effect_animate_action_ptr = other.hit_effect_animate_action_ptr;

    splat_effect_sprite_ptr         = other.splat_effect_sprite_ptr;
    splat_effect_animate_action_ptr = other.splat_effect_animate_action_ptr;

    rigidbody = other.rigidbody;

    collider          = other.collider;
    collider_x_axis   = other.collider_x_axis;
    collider_y_axis   = other.collider_y_axis;
    collider_offset_x = other.collider_offset_x;
	collider_offset_y = other.collider_offset_y;

    col_x_offset = other.col_x_offset;
    col_y_offset = other.col_y_offset;

	x_dir = other.x_dir;
    y_dir = other.y_dir;

	is_inactive   = other.is_inactive;
    is_dead       = other.is_dead;
    is_persistent = other.is_persistent;

	received_platform_force = other.received_platform_force;

    hitstun_frames         = other.hitstun_frames;
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

    ///////////////////
    // Check if dead //
    ///////////////////

    if(hitpoints <= 0 && state != OBJECT_DEATH)
    {setState(OBJECT_DEATH);}

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
    if(animate_action_ptr.has_value())
    {
        if(!animate_action_ptr->done())
        {
            animate_action_ptr->update();
        }
    }

    if(hit_effect_animate_action_ptr.has_value())
    {
        if(!hit_effect_animate_action_ptr->done())
        {
            hit_effect_animate_action_ptr->update();
            hit_effect_sprite_ptr->set_visible(true);
        }
        else
        {
            hit_effect_sprite_ptr->set_visible(false);
        }
    }

    if(splat_effect_animate_action_ptr.has_value())
    {
        if(!splat_effect_animate_action_ptr->done())
        {
            splat_effect_animate_action_ptr->update();
            splat_effect_sprite_ptr->set_visible(true);
        }
        else
        {
            splat_effect_sprite_ptr->set_visible(false);
        }
    }
}

void GameObject::setCamera(const bn::camera_ptr& camera)
{
    if(sprite_ptr.has_value())
    {sprite_ptr->set_camera(camera);}

    if(hit_effect_sprite_ptr.has_value())
    {hit_effect_sprite_ptr->set_camera(camera);}

    if(splat_effect_sprite_ptr.has_value())
    {splat_effect_sprite_ptr->set_camera(camera);}
    
    collider.setCamera(camera);
}

void GameObject::setState(ObjectState new_state)
{
    state = new_state;
}

bn::fixed GameObject::x() const
{
    if(!sprite_ptr.has_value()) {return 0;}

    return sprite_ptr->x().integer();
}

bn::fixed GameObject::y() const
{
    if(!sprite_ptr.has_value()) {return 0;}

    return sprite_ptr->y().integer();
}

bn::fixed_point GameObject::pos() const
{
    if(!sprite_ptr.has_value()) {return bn::point(0, 0);}

    bn::fixed_point point(sprite_ptr->position().x().integer() + collider_offset_x,
			              sprite_ptr->position().y().integer() + collider_offset_y);

    return point; 
}

void GameObject::setX(bn::fixed new_x)
{
    if(!sprite_ptr.has_value()) {return;}

    sprite_ptr->set_x(new_x.integer());
    collider.setX(new_x.integer() + collider_offset_x);
}

void GameObject::setY(bn::fixed new_y)
{
    if(!sprite_ptr.has_value()) {return;}

    sprite_ptr->set_y(new_y.integer());
    collider.setY(new_y.integer() + collider_offset_y);
}

void GameObject::setPos(bn::fixed new_x, bn::fixed new_y)
{
    if(sprite_ptr.has_value())
    {
        sprite_ptr->set_x(new_x.integer());
        sprite_ptr->set_y(new_y.integer());
        collider.setX(new_x.integer() + collider_offset_x);
        collider.setY(new_y.integer() + collider_offset_y);
    }
}

void GameObject::setPos(bn::fixed_point new_pos)
{
    if(sprite_ptr.has_value())
    {
        sprite_ptr->set_x(new_pos.x().integer());
        sprite_ptr->set_y(new_pos.y().integer());
        collider.setX(new_pos.x().integer() + collider_offset_x);
        collider.setY(new_pos.y().integer() + collider_offset_y);
    }
}

void GameObject::setHitStretch()
{
    if(sprite_ptr.has_value())
    {
        sprite_ptr->set_horizontal_scale(GAME_OBJECT_MAX_STRETCH_H);				
        sprite_ptr->set_vertical_scale(GAME_OBJECT_MIN_STRETCH_V);
    }
}

void GameObject::setVerticalStretch()
{
    if(sprite_ptr.has_value())
    {
        sprite_ptr->set_horizontal_scale(GAME_OBJECT_MIN_STRETCH_H);
        sprite_ptr->set_vertical_scale(GAME_OBJECT_MAX_STRETCH_V);
    }
}

void GameObject::setHorizontalStretch()
{
    if(sprite_ptr.has_value())
    {
        sprite_ptr->set_horizontal_scale(GAME_OBJECT_MAX_STRETCH_H);				
        sprite_ptr->set_vertical_scale(GAME_OBJECT_MIN_STRETCH_V);
    }
}

void GameObject::updateSpriteDirection()
{
    if(sprite_ptr.has_value())
    {
        if      (x_dir == LEFT)  {sprite_ptr->set_horizontal_flip(true);}
        else if (x_dir == RIGHT) {sprite_ptr->set_horizontal_flip(false);}
        
        if      (y_dir == DOWN)  {sprite_ptr->set_vertical_flip(true);}
        else if (y_dir == UP)    {sprite_ptr->set_vertical_flip(false);}
    }
}

void GameObject::updateSpriteOffsets()
{
    if(sprite_ptr.has_value())
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

void GameObject::updateHitstunState()
{
    hitstun_frames--;
    hitstun_frames = max(0, hitstun_frames);

    if(!hitstun_frames)
    {setState(NONE);}
}

void GameObject::udpateDeathState()
{
    if(animate_action_ptr->done())
    {is_dead = true;}
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

    if(sprite_ptr.has_value())
    {sprite_ptr->set_palette(sprite_palette);}
}

void GameObject::setHitFlash(int32 frames)
{
    if(frames > GAME_OBJECT_MAX_HIT_FLASH_FRAMES) 
    {frames = GAME_OBJECT_MAX_HIT_FLASH_FRAMES;}

    hit_flash_frames = frames;

    bn::sprite_palette_ptr sprite_palette = bn::sprite_palette_items::sprite_flash_palette.create_palette();
    
    if(sprite_ptr.has_value())
    {sprite_ptr->set_palette(sprite_palette);}
}

void GameObject::updateHitFlash()
{
    
    if(hit_flash_frames)
    {
        bn::sprite_palette_ptr sprite_palette = bn::sprite_palette_items::sprite_flash_palette.create_palette();
        if(sprite_ptr.has_value())
        {sprite_ptr->set_palette(sprite_palette);}
    }
    else
    {
        if(sprite_ptr.has_value())
        {sprite_ptr->set_palette(default_palette_ptr.value());}
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

void GameObject::applyHitEffect(int32 x, int32 y)
{   
    hit_effect_sprite_ptr->set_position(x, y);
    hit_effect_animate_action_ptr = bn::create_sprite_animate_action_once(hit_effect_sprite_ptr.value(), 
                                0, 
                                bn::sprite_items::hit_effect.tiles_item(),
                                0, 1, 2, 3, 4, 5, 6, 7, 8);
}

void GameObject::applySplatEffect(int32 x, int32 y)
{
    splat_effect_sprite_ptr->set_position(x, y);
    splat_effect_animate_action_ptr = bn::create_sprite_animate_action_once(splat_effect_sprite_ptr.value(), 
                                0, 
                                bn::sprite_items::wall_splat_effect.tiles_item(),
                                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14);

    if(col_x_offset < 0) {splat_effect_sprite_ptr->set_horizontal_flip(true);}
    else                 {splat_effect_sprite_ptr->set_horizontal_flip(false);}
}

void GameObject::resolveCollision(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                                const bn::regular_bg_ptr&                      bg_ptr, 
                                const bn::span<const bn::regular_bg_map_cell>& cells,
                                const bn::regular_bg_item&                     bg_item)
{
    //////////////////////////////
	// Init Collision Variables //
	//////////////////////////////

	// Get current cell index that enemy resides in:
	int32 half_level_width_pixels  = (bg_ptr.dimensions().width() / 2);
	int32 half_level_height_pixels = (bg_ptr.dimensions().height() / 2);
	bn::fixed index_x = (x() + half_level_width_pixels  + collider_offset_x)  / TILE_WIDTH;
	bn::fixed index_y = (y() + half_level_height_pixels + collider_offset_y)  / TILE_HEIGHT;
	bn::point cell_index = bn::point(index_x.integer(), index_y.integer());

	// Update colliders for each axis. 
	collider_x_axis.setPos(collider.x(), collider.y() - rigidbody.final_dir.y());
	collider_y_axis.setPos(collider.x() - rigidbody.final_dir.x(), collider.y());

	// Placeholder for other objects
	Collider other_collider;

    //////////////////////////////////
    // Resolve GameObject Collision //
    //////////////////////////////////

    for(int32 i = 0; i < game_objects.size(); i++)
    {
        other_collider = game_objects.at(i)->collider;
        
        switch(game_objects.at(i)->object_type)
        {
            default:
            break;
        }
    }

	////////////////////////////
    // Resolve Tile Collision //
    ////////////////////////////
	
	for(int32 y = -1; y < 2; y++)
	{
		for(int32 x = -1; x < 2; x++)
		{
			
			// 1. Get tile type at index //
			int32 check_index_x = cell_index.x() + x;
			int32 check_index_y = cell_index.y() + y;

			// Determine world coords in case we need to make a collider.
			int32 world_x = ((check_index_x * TILE_WIDTH)  - half_level_width_pixels)  + (TILE_WIDTH / 2);
			int32 world_y = ((check_index_y * TILE_HEIGHT) - half_level_height_pixels) + (TILE_HEIGHT / 2);

			uint32 tile_index = getTileAtBGIndex(check_index_x, check_index_y, 
			                                     bg_ptr, cells, bg_item);

			// 2. If the tile is collidable make a temporary collider based on type //

			if(tile_index >= HARD_BLOCK_MIN_INDEX && 
			   tile_index <= HARD_BLOCK_MAX_INDEX)
			{
				// Prepare offsets in case they are needed for Block collision.
				int32 block_w_offset = 0;
				int32 block_x_offset = 0;

				// If the neighbor to the right is also a BLOCK, smooth over the corner.
				// This is a hack to resolve collision since checks are always made from
				// left to right. 
				if(getTileAtBGIndex(check_index_x + 1, check_index_y, 
									 bg_ptr, cells, bg_item) >= HARD_BLOCK_MIN_INDEX && 
					getTileAtBGIndex(check_index_x + 1, check_index_y, 
									 bg_ptr, cells, bg_item) <= HARD_BLOCK_MAX_INDEX)
				{
					block_w_offset = TILE_WIDTH;
					block_x_offset = TILE_WIDTH / 2;
					x++;
				}

				other_collider = Collider(world_x + block_x_offset, 
										  world_y, 
										  TILE_WIDTH + block_w_offset,
										  TILE_HEIGHT);

				if(collider.isCollision(other_collider))
				{
					// Resolve Axis Collision
					resolveXAxisCollision(other_collider);
					resolveYAxisCollision(other_collider);

					// If there is still collision somehow, must be corner case
					resolveCornerCollision(other_collider);
				}
			}	

		}
	}
}

void GameObject::resolveXAxisCollision(const Collider& other_collider)
{
	col_x_offset = collider_x_axis.getCollisionXOffset(other_collider, rigidbody.normalized_dir.x());
	collider_x_axis.setX(collider_x_axis.x() + col_x_offset);
	setX(this->x() + col_x_offset);
}

void GameObject::resolveYAxisCollision(const Collider& other_collider)
{
    col_y_offset = collider_y_axis.getCollisionYOffset(other_collider, rigidbody.normalized_dir.y());
	collider_y_axis.setY(collider_y_axis.y() + col_y_offset);
	setY(this->y() + col_y_offset);
}

void GameObject::resolveCornerCollision(const Collider& other_collider)
{
    while(collider.isCollision(other_collider))
	{
		// We always resolve diagonal corner collisions with a horizontal shift. 
		setX(this->x() - rigidbody.normalized_dir.x());
	}
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