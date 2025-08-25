#include "bell_troll.h"

//////////////////////
// Struct BellTroll //
//////////////////////

BellTroll::BellTroll()
{
    // Init Assets //
    object_type = BELL_TROLL;
    sprite_ptr  = bn::sprite_items::bell_troll.create_sprite(0, 0);
    sprite_ptr->set_z_order(ENEMY_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                   2,
								                                   bn::sprite_items::bell_troll.tiles_item(),
								                                   0, 0, 1, 1);

    // Init Variables //
    collider_offset_x = BELL_TROLL_COLLIDER_OFFSET_X;
    collider_offset_y = BELL_TROLL_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y, 
                        BELL_TROLL_COLLIDER_WIDTH, 
                        BELL_TROLL_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;

    hitpoints = BELL_TROLL_HITPOINTS;

    thirty_fps = BELL_TROLL_30_FPS;

    state = IDLE;
}

BellTroll::BellTroll(const BellTroll& other) : Enemy(other)
{

}

BellTroll::~BellTroll()
{

}

BellTroll& BellTroll::operator =(const BellTroll& other)
{
    return *this;
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

void BellTroll::updateHPBar()
{
	if(hitpoints > ENEMY_MAX_HP) {return;}

	hp_sprite_ptr->set_position(x() + ENEMY_HP_BAR_X_OFFSET, y() + ENEMY_HP_BAR_Y_OFFSET);
	hp_animate_action_ptr = bn::create_sprite_animate_action_forever(hp_sprite_ptr.value(),
																	 0,
																	 bn::sprite_items::enemy_hp_bar.tiles_item(),
																	 hitpoints, hitpoints);

    // Hide or reveal if Bell Troll is frozen:
    if(state == BELL_TROLL_FROZEN) {hp_sprite_ptr->set_visible(false);}
    else                           {hp_sprite_ptr->set_visible(true);}
}

void BellTroll::updateTimers()
{
    GameObject::updateTimers();

    // Update frozen frames
    frozen_frames--;
    frozen_frames = clamp(0, BELL_TROLL_FROZEN_FRAMES, frozen_frames);
}

void BellTroll::applyDamage(int32 _damage)
{
    hitpoints -= _damage;
    if(hitpoints < 0 || state == BELL_TROLL_FROZEN) {hitpoints = 0;}
}

void BellTroll::updateHitFlash()
{
    if(frozen_frames) {GameObject::updateHitFlash();}
    else              {Enemy::updateHitFlash();}
}

void BellTroll::setHitFlash()
{
    if(frozen_frames) {GameObject::setHitFlash();}
    else              {Enemy::setHitFlash();}
}

void BellTroll::setHitFlash(int32 frames)
{
    if(frozen_frames) {GameObject::setHitFlash(frames);}
    else              {Enemy::setHitFlash(frames);}
}

//////////////////////////////
// State Function Overrides //
//////////////////////////////

void BellTroll::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                                   const bn::regular_bg_ptr&                      bg_ptr, 
                                   const bn::span<const bn::regular_bg_map_cell>& cells,
                                   const bn::regular_bg_item&                     bg_item,
                                   const bn::camera_ptr&                          camera)
{
    switch(state)
    {
        case IDLE:

            // Run
            rigidbody.addForce(BELL_TROLL_RUN_FORCE);

            // Gravity
            if(!grounded_detected)
            {rigidbody.addForce(GAME_OBJECT_GRAVITY_FORCE);}

            // Update//Reset state
            if(global_bell_struck) {setState(BELL_TROLL_FROZEN);}

        break;

        case BELL_TROLL_FROZEN:

            if(frozen_frames <= 0) {setState(IDLE);}

            // Gravity
            if(!grounded_detected)
            {rigidbody.addForce(GAME_OBJECT_GRAVITY_FORCE);}

            // Reset state
            if(global_bell_struck) 
            {
                frozen_frames      = BELL_TROLL_FROZEN_FRAMES;
                animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
                                                                           0,
                                                                           bn::sprite_items::bell_troll.tiles_item(),
                                                                           2, 2);
            }

            // Warning animation
            if(frozen_frames == BELL_TROLL_TRANSITION_FRAME)
            {
                animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                               2,
                                                                               bn::sprite_items::bell_troll.tiles_item(),
                                                                               1, 1, 2, 2);
            }

        break;

        case OBJECT_HITSTUN:

            updateHitstunState();

            // Update state
            if(global_bell_struck) {setState(BELL_TROLL_FROZEN);}

        break;

        case OBJECT_DEATH:

            updateDeathState();

        break;

        default:
        break;
    }
}

void BellTroll::setState(ObjectState new_state)
{
    state = new_state;

    switch(state)
    {
        case IDLE:

            animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                           2,
                                                                           bn::sprite_items::bell_troll.tiles_item(),
                                                                           0, 0, 1, 1); 

        break;

        case BELL_TROLL_FROZEN:

            frozen_frames = BELL_TROLL_FROZEN_FRAMES;

            animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
                                                                       2,
                                                                       bn::sprite_items::bell_troll.tiles_item(),
                                                                       1, 2, 1, 2, 1, 2, 1, 2);

        break;

        case OBJECT_DEATH:

            // Play default death animation:
            if(!frozen_frames) {playEnemyDeathAnim();}
            // Play block death animation:
            else {rigidbody.removeXForces(); playBlockDeathAnim();}

        break;

        default:
        break;
    }
}

/////////////////////////
// Collision Overrides //
/////////////////////////

void BellTroll::resolveXAxisCollision(const Collider& other_collider)
{
	GameObject::resolveXAxisCollision(other_collider);
	
	// Wall splat check
	if(state != OBJECT_DEATH && !frozen_frames)
	{wallSplatCheck();}

    if(col_x_offset != 0) 
    {
        if(x_dir == RIGHT) {x_dir = LEFT;}
        else               {x_dir = RIGHT;}
    }
}

// Objects
void BellTroll::resolveBounceBellCollision(GameObject& object)
{
    if(collider.isCollision(object.collider))
    {
		if(state == OBJECT_HITSTUN)
		{
            object.applyHit(damage, 0, 0);
            setState(BELL_TROLL_FROZEN);
        }
    }
}

void BellTroll::resolveAutoBounceBellCollision(GameObject& object)
{
	if(collider.isCollision(object.collider))
    {
		if(state == OBJECT_HITSTUN)
		{
            object.applyHit(damage, 0, 0);
            setState(BELL_TROLL_FROZEN);
        }
    }
}

// Tiles
void BellTroll::resolveTileCollision(const bn::regular_bg_ptr&                      bg_ptr, 
                                     const bn::span<const bn::regular_bg_map_cell>& cells,
                                     const bn::regular_bg_item&                     bg_item)
{
    ////////////////////////////////////////
    // Update Variables for state testing //
	////////////////////////////////////////

    grounded_detected = false;

    //////////////////////////////
	// Init Collision Variables //
	//////////////////////////////

	// Get current cell index that object resides in:
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

    for(int32 y = 1; y < 3; y++)
	{
		for(int32 x = -2; x < 3; x++)
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

				resolveHardBlockCollision(other_collider);
			}

            else if(tile_index == H_GEAR_LEFT)
            {
                other_collider = Collider(world_x,
                                          world_y, 
                                          TILE_WIDTH, 
                                          TILE_HEIGHT);
                
                resolveHGearLeftCollision(other_collider);
            }

            else if(tile_index >= H_GEAR_MID_MIN &&
                    tile_index <= H_GEAR_MID_MAX)
            {
                other_collider = Collider(world_x,
                                          world_y, 
                                          TILE_WIDTH, 
                                          TILE_HEIGHT);
                
                resolveHGearMidCollision(other_collider);
            }

            else if(tile_index == H_GEAR_RIGHT)
            {
                other_collider = Collider(world_x,
                                          world_y, 
                                          TILE_WIDTH, 
                                          TILE_HEIGHT);
                
                resolveHGearRightCollision(other_collider);
            }

            else if(tile_index == V_GEAR_TOP)
			{
				other_collider = Collider(world_x,
										  world_y, 
										  TILE_WIDTH, 
										  TILE_HEIGHT);

				resolveVGearTopCollision(other_collider);
			}

			else if(tile_index == V_GEAR_MID_1 ||
					tile_index == V_GEAR_MID_2 ||
					tile_index == V_GEAR_MID_3)
			{
				other_collider = Collider(world_x,
										  world_y, 
										  TILE_WIDTH, 
										  TILE_HEIGHT);

				resolveVGearMidCollision(other_collider);
			}

			else if(tile_index == V_GEAR_BOTTOM)
			{
				other_collider = Collider(world_x,
										  world_y, 
										  TILE_WIDTH, 
										  TILE_HEIGHT);

				resolveVGearBottomCollision(other_collider);
			}

            else if(tile_index >= SPIKE_MIN_INDEX &&
                    tile_index <= SPIKE_MAX_INDEX)
            {
                other_collider = Collider(world_x,
                                          world_y, 
                                          TILE_WIDTH, 
                                          TILE_HEIGHT);
                
                resolveSpikeCollision(other_collider);
            }
            
            else if(tile_index >= CLIMBABLE_MIN_INDEX &&
                    tile_index <= CLIMBABLE_MAX_INDEX)
            {
                other_collider = Collider(world_x,
                                          world_y, 
                                          TILE_WIDTH, 
                                          TILE_HEIGHT);
                
                resolveClimbableCollision(other_collider);
            }

            else if(tile_index >= ONEWAY_BLOCK_MIN_INDEX &&
                    tile_index <= ONEWAY_BLOCK_MAX_INDEX)
            {
                other_collider = Collider(world_x, 
                                          world_y + ONEWAY_BLOCK_COLLIDER_Y_OFFSET, 
                                          TILE_WIDTH, 
                                          ONEWAY_BLOCK_COLLIDER_HEIGHT);

                resolveOneWayBlockCollision(other_collider);
            }
		}
	}

    /////////////////////////////
	// Test for Grounded State //
	/////////////////////////////

	updateTileGrounded(bg_ptr, cells, bg_item);
}

void BellTroll::resolveSpikeCollision(const Collider& other_collider)
{
    updateTestColliders();

    if(collider.isCollision(other_collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(other_collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(other_collider);

        // Resolve Corner Collision // 
        if(col_y_offset == 0 && col_x_offset == 0)
        {resolveCornerCollision(other_collider);}
    }
}