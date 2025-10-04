#include "star_jar.h"

///////////////////
// Struct StarJar //
///////////////////

StarJar::StarJar()
{
    // Init Assets //
    object_type = STAR_JAR;
    sprite_ptr  = bn::sprite_items::star_jar.create_sprite(0, 0);
    sprite_ptr->set_z_order(ENEMY_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								                                   2,
								                                   bn::sprite_items::star_jar.tiles_item(),
								                                   0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3);

    // Init Variables //
    collider_offset_x = STAR_JAR_COLLIDER_OFFSET_X;
    collider_offset_y = STAR_JAR_COLLIDER_OFFSET_Y;
	collider = Collider(x() + collider_offset_x, 
                        y() + collider_offset_y,
                        STAR_JAR_COLLIDER_WIDTH, 
                        STAR_JAR_COLLIDER_HEIGHT);
    collider_x_axis   = collider;
    collider_y_axis   = collider;

    setState(IDLE);

    hitpoints  = STAR_JAR_HITPOINTS;
    thirty_fps = STAR_JAR_30_FPS;
    damage     = STAR_JAR_VALUE;

	invulnerability_frames = STAR_JAR_STARTING_INVULN_FRAMES;

	rigidbody.addForce(STAR_JAR_SPAWN_FORCE);
}

StarJar::StarJar(const StarJar& other) : GameObject(other)
{

}

StarJar::~StarJar()
{

}

StarJar& StarJar::operator =(const StarJar& other)
{
    return *this;
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

void StarJar::update(const RoomBounds& 							 room_bounds,
                      bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                      const bn::regular_bg_ptr&                      bg_ptr, 
                      const bn::span<const bn::regular_bg_map_cell>& cells,
                      const bn::regular_bg_item&                     bg_item,
                      const bn::camera_ptr&                          camera)
{
    
    /////////////////////////////////
    // Early out for 30FPS Objects //
    /////////////////////////////////

    if((thirty_fps || is_frozen) && (global_timer % 2 == 0)) {return;}

    //////////////////
    // Update State //
    //////////////////

    updateStateMachine(game_objects, bg_ptr, cells, bg_item, camera);

    ////////////////////
    // Update Physics //
    ////////////////////

    updatePhysics();

    ///////////////////////
    // Resolve Collision //
    ///////////////////////

    if(!is_frozen) {resolveCollision(game_objects, bg_ptr, cells, bg_item);}

    //////////////////
    // Update State //
    //////////////////

    //updateState(game_objects, bg_ptr, cells, bg_item);

    /////////////////////
    // Update Hitboxes //
    /////////////////////

    //updateHitboxes(room_bounds, game_objects, bg_ptr, cells, bg_item, camera);

    ///////////////////
    // Update Timers //
    ///////////////////

    updateTimers();

    ///////////////////
    // Check if dead //
    ///////////////////

    //checkIfDead();

    /////////////////////////////
	// Update Sprite Direction //
	/////////////////////////////
	
    //updateSpriteDirection();
    
    ////////////////////////////
    // Correct Sprite Offsets //
    ////////////////////////////
	
    //updateSpriteOffsets();

    //////////////////////////
    // Update HP Bar Visual //
    //////////////////////////

    //updateHPBar();

	//////////////////////
	// Update Hit Flash //
	//////////////////////

	//updateHitFlash();

    ////////////////////
    // Clamp Position //
    ////////////////////

    //clampPosition(room_bounds);

	//////////////////////////////
    // Monitor unloading bounds //
    //////////////////////////////
    
	updateInactiveState(camera);

}

void StarJar::setState(ObjectState new_state)
{
    state = new_state;

    switch(new_state)
    {
        case IDLE:

			animate_action_ptr  = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
																		3,
																		bn::sprite_items::star_jar.tiles_item(),
																		0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3);

        break;

        case OBJECT_HITSTUN:
        break;

        case OBJECT_DEATH:

			global_stars_collected++;
			playDropDeathAnim();

        break;

        default:
        break;
    }
}

//////////////////////////////
// State Function Overrides //
//////////////////////////////

void StarJar::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&   game_objects,
                                const bn::regular_bg_ptr&                      bg_ptr, 
                                const bn::span<const bn::regular_bg_map_cell>& cells,
                                const bn::regular_bg_item&                     bg_item,
                                const bn::camera_ptr&                          camera)
{
    switch(state)
    {
        case IDLE:

            // Gravity
            if(!grounded_detected)
            {rigidbody.addForce(GAME_OBJECT_GRAVITY_FORCE);}

        break;

        case OBJECT_HITSTUN:

            updateHitstunState();

        break;

        case OBJECT_DEATH:

            updateDeathState();

        break;

        default:
        break;
    }
}

/////////////////////////
// Collision Overrides //
/////////////////////////

// ...