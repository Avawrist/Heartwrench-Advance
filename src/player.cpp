#include "player.h"

///////////////////
// Struct Player //
///////////////////

Player::Player()
{
    // Init Variables //
	object_type = PLAYER;
	x_dir       = RIGHT;
	y_dir       = UP;

    sprite_ptr  = bn::sprite_items::player.create_sprite(0, 0);
	sprite_ptr->set_z_order(PLAYER_Z_ORDER);
	animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
																0,
																bn::sprite_items::player.tiles_item(),
																0,
																0);
	default_palette_ptr = sprite_ptr->palette();

	collider            = Collider(x(), y(), PLAYER_COLLIDER_WIDTH, PLAYER_COLLIDER_HEIGHT);
	collider_x_axis     = collider;
	collider_y_axis     = collider;
	test_collider_right = collider;
	test_collider_left  = collider;
	collider_offset_x   = PLAYER_COLLIDER_OFFSET_X;
	collider_offset_y   = PLAYER_COLLIDER_OFFSET_Y;
    
	state             	 = PLAYER_AIR_NEUTRAL;

	prior_frame_1_pos = bn::point(0, 0);
	prior_frame_2_pos = bn::point(0, 0);
	prior_frame_3_pos = bn::point(0, 0);

    x_speed        	  	 = PLAYER_MIN_X_SPEED;
	phase_destination    = bn::fixed_point(0, 0);
	
	hitpoints                        = PLAYER_STARTING_HITPOINTS;
	attack_buffered_frames           = 0;
	roll_buffered_frames             = 0;
	jump_buffered_frames             = 0;
	remaining_jump_input_frames      = 0;
	air_frames_elapsed               = 0;
	v_collision_grace_frames         = 0;
	late_jump_grace_frames           = 0;
	late_roll_jump_grace_frames      = 0;
	current_phase_frame              = 0;
	hitstop_frames                   = 0;
	update_timer                     = 0;
	
	wall_right_detected      = false;
    wall_left_detected       = false;
	grounded_owp_detected    = false;
	left_wj_eligible         = false;
	right_wj_eligible        = false;
	is_dead                  = false;

	roll_requested   = false;
	jump_requested   = false;
	attack_requested = false;

	hitbox_1_ptr = NULL;
	hitbox_2_ptr = NULL;
	hitbox_3_ptr = NULL;

	phase_dir = PHASE_RIGHT;
	pm_sprite_ptr = bn::sprite_items::phase_marker.create_sprite(0, 0);
	pm_sprite_ptr->set_z_order(PLAYER_Z_ORDER);
	pm_sprite_ptr->set_visible(false);

	jump_effect_sprite_ptr = bn::sprite_items::air_jump_effect.create_sprite(0, 0);
	jump_effect_sprite_ptr->set_z_order(PLAYER_Z_ORDER);
	jump_effect_sprite_ptr->set_visible(false);

	od_sprite_1_ptr = bn::sprite_items::player.create_sprite(0, 0);
	od_sprite_1_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
	od_sprite_1_ptr->set_visible(false);

	od_sprite_2_ptr = bn::sprite_items::player.create_sprite(0, 0);
	od_sprite_2_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
	od_sprite_2_ptr->set_visible(false);

	od_sprite_3_ptr = bn::sprite_items::player.create_sprite(0, 0);
	od_sprite_3_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
	od_sprite_3_ptr->set_visible(false);

	bn::sprite_palette_ptr sprite_od_palette = bn::sprite_palette_items::sprite_od_palette.create_palette();
	od_sprite_1_ptr->set_palette(sprite_od_palette);
	od_sprite_2_ptr->set_palette(sprite_od_palette);
	od_sprite_3_ptr->set_palette(sprite_od_palette);
}

Player::Player(const Player& other) : GameObject(other)
{
	prior_frame_1_pos = other.prior_frame_1_pos;
	prior_frame_2_pos = other.prior_frame_2_pos;
	prior_frame_3_pos = other.prior_frame_3_pos;

    x_speed        	  	 = other.x_speed;
	phase_destination    = other.phase_destination;
	
	attack_buffered_frames           = other.attack_buffered_frames;
	roll_buffered_frames             = other.roll_buffered_frames;
	jump_buffered_frames             = other.jump_buffered_frames;
	remaining_jump_input_frames      = other.remaining_jump_input_frames;
	air_frames_elapsed               = other.air_frames_elapsed;
	v_collision_grace_frames         = other.v_collision_grace_frames;
	late_jump_grace_frames           = other.late_jump_grace_frames;
	late_roll_jump_grace_frames      = other.late_roll_jump_grace_frames;
	current_phase_frame              = other.current_phase_frame;
	hitstop_frames                   = other.hitstop_frames;
	update_timer                     = other.update_timer;

	wall_right_detected      = other.wall_right_detected;
    wall_left_detected       = other.wall_left_detected;
	grounded_owp_detected    = other.grounded_owp_detected;
	left_wj_eligible         = other.left_wj_eligible;
	right_wj_eligible        = other.right_wj_eligible;
	is_dead                  = other.is_dead;

	roll_requested   = other.roll_requested;
	jump_requested   = other.jump_requested;
	attack_requested = other.attack_requested;

	test_collider_right   = other.test_collider_right;
	test_collider_left    = other.test_collider_left;

	if(other.hitbox_1_ptr == NULL) {hitbox_1_ptr = NULL;}
	else {hitbox_1_ptr = new Hitbox(*(other.hitbox_1_ptr));}

	if(other.hitbox_2_ptr == NULL) {hitbox_2_ptr = NULL;}
	else {hitbox_2_ptr = new Hitbox(*(other.hitbox_2_ptr));}	
	
	if(other.hitbox_3_ptr == NULL) {hitbox_3_ptr = NULL;}
	else {hitbox_3_ptr = new Hitbox(*(other.hitbox_3_ptr));}

	phase_dir              = other.phase_dir;
	pm_sprite_ptr          = other.pm_sprite_ptr;
	jump_effect_sprite_ptr = other.jump_effect_sprite_ptr;
	jump_effect_anim_ptr   = other.jump_effect_anim_ptr;
	od_sprite_1_ptr        = other.od_sprite_1_ptr;
	od_sprite_2_ptr        = other.od_sprite_2_ptr;
	od_sprite_3_ptr        = other.od_sprite_3_ptr;
	
}

Player::~Player()
{
	delete hitbox_1_ptr;
	delete hitbox_2_ptr;
	delete hitbox_3_ptr;	

	// Free the phase marker sprite
	pm_sprite_ptr.reset();
	jump_effect_sprite_ptr.reset();
	jump_effect_anim_ptr.reset();
	od_sprite_1_ptr.reset();
	od_sprite_2_ptr.reset();
	od_sprite_3_ptr.reset();
}

Player& Player::operator =(const Player& other)
{
	prior_frame_1_pos = other.prior_frame_1_pos;
	prior_frame_2_pos = other.prior_frame_2_pos;
	prior_frame_3_pos = other.prior_frame_3_pos;

    x_speed        	  	 = other.x_speed;
	phase_destination    = other.phase_destination;
	
	attack_buffered_frames           = other.attack_buffered_frames;
	roll_buffered_frames             = other.roll_buffered_frames;
	jump_buffered_frames             = other.jump_buffered_frames;
	remaining_jump_input_frames      = other.remaining_jump_input_frames;
	air_frames_elapsed               = other.air_frames_elapsed;
	v_collision_grace_frames         = other.v_collision_grace_frames;
	late_jump_grace_frames           = other.late_jump_grace_frames;
	late_roll_jump_grace_frames      = other.late_roll_jump_grace_frames;
	current_phase_frame              = other.current_phase_frame;
	hitstop_frames                   = other.hitstop_frames;
	update_timer                     = other.update_timer;

	wall_right_detected      = other.wall_right_detected;
    wall_left_detected       = other.wall_left_detected;
	grounded_owp_detected    = other.grounded_owp_detected;
	left_wj_eligible         = other.left_wj_eligible;
	right_wj_eligible        = other.right_wj_eligible;
	is_dead                  = other.is_dead;

	roll_requested   = other.roll_requested;
	jump_requested   = other.jump_requested;
	attack_requested = other.attack_requested;

	test_collider_right   = other.test_collider_right;
	test_collider_left    = other.test_collider_left;

	if(other.hitbox_1_ptr == NULL) {hitbox_1_ptr = NULL;}
	else {hitbox_1_ptr = new Hitbox(*(other.hitbox_1_ptr));}

	if(other.hitbox_2_ptr == NULL) {hitbox_2_ptr = NULL;}
	else {hitbox_2_ptr = new Hitbox(*(other.hitbox_2_ptr));}	
	
	if(other.hitbox_3_ptr == NULL) {hitbox_3_ptr = NULL;}
	else {hitbox_3_ptr = new Hitbox(*(other.hitbox_3_ptr));}

	phase_dir              = other.phase_dir;
	pm_sprite_ptr          = other.pm_sprite_ptr;
	jump_effect_sprite_ptr = other.jump_effect_sprite_ptr;
	jump_effect_anim_ptr   = other.jump_effect_anim_ptr;
	od_sprite_1_ptr        = other.od_sprite_1_ptr;
	od_sprite_2_ptr        = other.od_sprite_2_ptr;
	od_sprite_3_ptr        = other.od_sprite_3_ptr;

	return *this;
}

void Player::updateOverdriveState()
{
	// Update Overdrive state
	if(hitpoints >= PLAYER_OD_1_HP_REQUIRED) 
	{
		overdrive = PLAYER_OD_LEVEL_1;

		if(hitpoints >= PLAYER_OD_2_HP_REQUIRED)
		{overdrive = PLAYER_OD_LEVEL_2;}
	}
	else {overdrive = PLAYER_OD_LEVEL_0;}
}

void Player::jump()
{
	remaining_jump_input_frames = PLAYER_MAX_JUMP_INPUT_FRAMES;
	late_jump_grace_frames      = 0;
	rigidbody.addForce(PLAYER_JUMP_FORCE);
	//setVerticalStretch();

	// Jump Effect
	if(!grounded_detected) {createAirJumpEffect();}
}

void Player::rollJump()
{
	x_speed = 0;
	rigidbody.removeXForces();
	setState(NONE);

	remaining_jump_input_frames = PLAYER_MAX_JUMP_INPUT_FRAMES;
	late_jump_grace_frames      = 0;
	rigidbody.addForce(PLAYER_ROLL_JUMP_FORCE);
	//setVerticalStretch();

	// Jump Effect
	if(!grounded_detected) {createAirJumpEffect();}
}

void Player::wallJump()
{
	x_speed = 0;

	rigidbody.removeForces();
	rigidbody.addForce(PLAYER_WALL_JUMP_FORCE);
	remaining_jump_input_frames = PLAYER_MAX_JUMP_INPUT_FRAMES;
	air_frames_elapsed = 0;
	
	//setVerticalStretch();
	createWallJumpEffect();
}

void Player::fastFall()
{
	rigidbody.addForce(PLAYER_FAST_GRAVITY_FORCE);
	//sprite_ptr->set_vertical_scale(PLAYER_FALL_STRETCH_V);
	//sprite_ptr->set_horizontal_scale(PLAYER_FALL_STRETCH_H);
}

void Player::createGroundedAttackHitboxes(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects, 
	                                      const bn::camera_ptr&                      camera)
{
	if(game_objects.size() >= MAX_GAME_OBJECTS) {return;}

	delete hitbox_1_ptr;
	hitbox_1_ptr = new Hitbox(bn::point(x().integer() + (PLAYER_ATTACK_GROUND_1_X_OFFSET * x_dir),
								  y().integer() + PLAYER_ATTACK_GROUND_1_Y_OFFSET),
								  PLAYER_ATTACK_GROUND_1_HITSTOP_FRAMES,
								  PLAYER_ATTACK_GROUND_1_HITSTUN_FRAMES,
								  PLAYER_ATTACK_GROUND_1_SCREENSHAKE_FRAMES,
								  PLAYER_ATTACK_GROUND_1_HB_LIFESPAN_FRAMES,
								  PLAYER_ATTACK_GROUND_1_X_KNOCKBACK,
								  PLAYER_ATTACK_GROUND_1_Y_KNOCKBACK,	
								  PLAYER_ATTACK_GROUND_1_KNOCKBACK_DECAY,
								  PLAYER_ATTACK_GROUND_1_HB_WIDTH,
								  PLAYER_ATTACK_GROUND_1_HB_HEIGHT,
								  PLAYER_ATTACK_GROUND_1_DAMAGE,
								  x_dir,
								  y_dir,
								  HITBOX_ATTACK_GROUND_1,
								  PLAYER_ATTACK_GROUND_1_SCREENSHAKE_SEVERITY);

	hitbox_1_ptr->setCamera(camera);

	// Add more hitboxes...
}

void Player::createAirAttack1Hitboxes(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects, 
							  		  const bn::camera_ptr&                      camera)
{
	if(game_objects.size() >= MAX_GAME_OBJECTS) {return;}

	delete hitbox_1_ptr;
	hitbox_1_ptr = new Hitbox(bn::point(x().integer() + (PLAYER_ATTACK_AIR_1_X_OFFSET * x_dir),
								  y().integer() + PLAYER_ATTACK_AIR_1_Y_OFFSET),
								  PLAYER_ATTACK_AIR_1_HITSTOP_FRAMES,
								  PLAYER_ATTACK_AIR_1_HITSTUN_FRAMES,
								  PLAYER_ATTACK_AIR_1_SCREENSHAKE_FRAMES,
								  PLAYER_ATTACK_AIR_1_HB_LIFESPAN_FRAMES,
								  PLAYER_ATTACK_AIR_1_X_KNOCKBACK,
								  PLAYER_ATTACK_AIR_1_Y_KNOCKBACK,	
								  PLAYER_ATTACK_AIR_1_KNOCKBACK_DECAY,
								  PLAYER_ATTACK_AIR_1_HB_WIDTH,
								  PLAYER_ATTACK_AIR_1_HB_HEIGHT,
								  PLAYER_ATTACK_AIR_1_DAMAGE,
								  x_dir,
								  y_dir,
								  HITBOX_ATTACK_AIR_1,
								  PLAYER_ATTACK_AIR_1_SCREENSHAKE_SEVERITY);

	hitbox_1_ptr->setCamera(camera);

	// Add more hitboxes...
}

void Player::createAirJumpEffect()
{
    if(global_tiles_in_VRAM > MAX_SPRITE_TILES) {return;}

	#define AIR_JUMP_EFFECT_OFFSET -1

	jump_effect_sprite_ptr->set_visible(true);
	jump_effect_sprite_ptr->set_position(x(), y() + AIR_JUMP_EFFECT_OFFSET);
	jump_effect_sprite_ptr->set_rotation_angle(0);
	jump_effect_anim_ptr = bn::create_sprite_animate_action_once(jump_effect_sprite_ptr.value(),
																1,
																bn::sprite_items::air_jump_effect.tiles_item(),
																0, 1, 2, 3, 4, 5, 5, 6, 6);
}

void Player::createWallJumpEffect()
{
	if(global_tiles_in_VRAM > MAX_SPRITE_TILES) {return;}

	#define WALL_JUMP_EFFECT_OFFSET -8

	jump_effect_sprite_ptr->set_visible(true);
	jump_effect_sprite_ptr->set_position(x() + ((int32)x_dir * WALL_JUMP_EFFECT_OFFSET), y());
	if(x_dir == LEFT) {jump_effect_sprite_ptr->set_rotation_angle(90);}
	else              {jump_effect_sprite_ptr->set_rotation_angle(270);}
	jump_effect_anim_ptr = bn::create_sprite_animate_action_once(jump_effect_sprite_ptr.value(),
																 1,
																 bn::sprite_items::air_jump_effect.tiles_item(),
																 0, 1, 2, 3, 4, 5, 5, 6, 6);
}

void Player::drawOverdriveEffect()
{
	//if(update_timer % 1 == 0)
	//{
		prior_frame_3_pos = prior_frame_2_pos;
		prior_frame_2_pos = prior_frame_1_pos;
		prior_frame_1_pos.set_x((x() - rigidbody.final_dir.x()).integer());
		prior_frame_1_pos.set_y((y() - rigidbody.final_dir.y()).integer());

		od_sprite_3_ptr->set_tiles(od_sprite_2_ptr->tiles());
		od_sprite_3_ptr->set_horizontal_flip(od_sprite_2_ptr->horizontal_flip());
		od_sprite_3_ptr->set_vertical_flip(od_sprite_2_ptr->vertical_flip());

		od_sprite_2_ptr->set_tiles(od_sprite_1_ptr->tiles());
		od_sprite_2_ptr->set_horizontal_flip(od_sprite_1_ptr->horizontal_flip());
		od_sprite_2_ptr->set_vertical_flip(od_sprite_1_ptr->vertical_flip());
		
		od_sprite_1_ptr->set_tiles(sprite_ptr->tiles());
		od_sprite_1_ptr->set_horizontal_flip(sprite_ptr->horizontal_flip());
		od_sprite_1_ptr->set_vertical_flip(sprite_ptr->vertical_flip());
	//}

	if(overdrive > 0)
	{
		od_sprite_1_ptr->set_position(prior_frame_1_pos.x(), prior_frame_1_pos.y());
		od_sprite_1_ptr->set_visible(true);

		od_sprite_2_ptr->set_position(prior_frame_2_pos.x(), prior_frame_2_pos.y());
		od_sprite_2_ptr->set_visible(true);

		if(overdrive > 1)
		{
			od_sprite_3_ptr->set_position(prior_frame_3_pos.x(), prior_frame_3_pos.y());
			od_sprite_3_ptr->set_visible(true);
		}
	}
	else 
	{
		od_sprite_1_ptr->set_visible(false);
		od_sprite_2_ptr->set_visible(false);
		od_sprite_3_ptr->set_visible(false);
	}
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

void Player::updateHitboxes(const RoomBounds& 							   room_bounds,
                        	bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                        	const bn::regular_bg_ptr&                      bg_ptr, 
                        	const bn::span<const bn::regular_bg_map_cell>& cells,
                        	const bn::regular_bg_item&                     bg_item,
                        	const bn::camera_ptr&                          camera)
{
	if(hitbox_1_ptr != NULL)
	{

		hitbox_1_ptr->setPos(bn::point(x().integer() + (PLAYER_ATTACK_GROUND_1_X_OFFSET * x_dir),
									   y().integer() + (PLAYER_ATTACK_GROUND_1_Y_OFFSET * y_dir)));
		hitbox_1_ptr->update(room_bounds,
						     game_objects,
							 bg_ptr, 
							 cells,
							 bg_item,
							 camera);
		hitbox_1_ptr->draw();

		if(hitbox_1_ptr->is_inactive) 
		{
			delete hitbox_1_ptr;
			hitbox_1_ptr = NULL;
		}	

	}
	
	if(hitbox_2_ptr != NULL) 
	{

		hitbox_2_ptr->setPos(bn::point(x().integer() + (PLAYER_ATTACK_GROUND_1_X_OFFSET * x_dir),
									   y().integer() + (PLAYER_ATTACK_GROUND_1_Y_OFFSET * y_dir)));
		hitbox_2_ptr->update(room_bounds,
							 game_objects,
							 bg_ptr, 
                    		 cells,
                    		 bg_item,
							 camera);
		hitbox_2_ptr->draw();

		if(hitbox_2_ptr->is_inactive) 
		{
			delete hitbox_2_ptr;
			hitbox_2_ptr = NULL;
		}

	}
	
	if(hitbox_3_ptr != NULL)
	{
		hitbox_3_ptr->setPos(bn::point(x().integer() + (PLAYER_ATTACK_GROUND_1_X_OFFSET * x_dir),
									   y().integer() + (PLAYER_ATTACK_GROUND_1_Y_OFFSET * y_dir)));

		hitbox_3_ptr->update(room_bounds,
							 game_objects,
							 bg_ptr, 
                    		 cells,
                    		 bg_item,
							 camera);
		hitbox_3_ptr->draw();
	
		if(hitbox_3_ptr->is_inactive) 
		{
			delete hitbox_3_ptr;
			hitbox_3_ptr = NULL;
		}
	}
}

void Player::updateTimers()
{
	update_timer++;
	if(update_timer >= 60) {update_timer = 0;}

	invulnerability_frames--;
    if(invulnerability_frames < 0) {invulnerability_frames = 0;}

	attack_buffered_frames--;
	attack_buffered_frames = clamp(0, 
		 						   PLAYER_ATTACK_BUFFER_FRAMES, 
								   attack_buffered_frames);

	roll_buffered_frames--;
	roll_buffered_frames = clamp(0, 
		 						 PLAYER_ROLL_BUFFER_FRAMES, 
								 roll_buffered_frames);

	jump_buffered_frames--;
	jump_buffered_frames = clamp(0, 
		 						   PLAYER_JUMP_BUFFER_FRAMES, 
								   jump_buffered_frames);

	v_collision_grace_frames--;
	v_collision_grace_frames = clamp(0, 
									 PLAYER_V_COLLISION_MAX_GRACE_FRAMES,
									 v_collision_grace_frames);

	late_roll_jump_grace_frames--;
	late_roll_jump_grace_frames = clamp(0, 
		                                PLAYER_LATE_ROLL_JUMP_GRACE_FRAMES, 
										late_roll_jump_grace_frames);

	air_frames_elapsed++;
	air_frames_elapsed = clamp(0, 
							   PLAYER_MAX_AIR_FRAMES, 
							   air_frames_elapsed);
							   
	if(grounded_detected) {air_frames_elapsed = 0;}

	received_platform_force = false;

	roll_requested   = false;
	jump_requested   = false;
	attack_requested = false;

	if(jump_effect_anim_ptr.has_value())
	{
		if(jump_effect_anim_ptr->done())
		{jump_effect_sprite_ptr->set_visible(false);}
	}
}

void Player::updateTestColliders()
{
	GameObject::updateTestColliders();

	// Update wall right/left test colliders
	test_collider_right.setPos(bn::point(collider.pos().x().integer() + PLAYER_WALL_TEST_RAY_LENGTH, collider.pos().y().integer()));
	test_collider_left.setPos(bn::point(collider.pos().x().integer()  - PLAYER_WALL_TEST_RAY_LENGTH, collider.pos().y().integer()));
}

void Player::draw()
{
	// Draw Player
	if(animate_action_ptr.has_value() && !animate_action_ptr->done())
    {
		animate_action_ptr->update();

		// Draw OD Effect
		drawOverdriveEffect();
    }
	global_tiles_in_VRAM += sprite_ptr->tiles().tiles_count();

	// Draw Hit Effect
	if(global_tiles_in_VRAM > MAX_SPRITE_TILES) {return;}
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
	global_tiles_in_VRAM += hit_effect_sprite_ptr->tiles().tiles_count();

	// Draw Splat Effect
	if(global_tiles_in_VRAM > MAX_SPRITE_TILES) {return;}
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
	global_tiles_in_VRAM += splat_effect_sprite_ptr->tiles().tiles_count();

	// Draw Jump effect
	if(global_tiles_in_VRAM > MAX_SPRITE_TILES) {return;}
	if(jump_effect_anim_ptr.has_value())
	{
		if(!jump_effect_anim_ptr->done())
		{jump_effect_anim_ptr->update();}
	}
	global_tiles_in_VRAM += jump_effect_sprite_ptr->tiles().tiles_count();
}

void Player::setCamera(const bn::camera_ptr& camera)
{
	GameObject::setCamera(camera);

	pm_sprite_ptr->set_camera(camera);
	jump_effect_sprite_ptr->set_camera(camera);
	od_sprite_1_ptr->set_camera(camera);
	od_sprite_2_ptr->set_camera(camera);
	//od_sprite_3_ptr->set_camera(camera);
}

void Player::applyHit(int32 _damage, int32 knockback_x_dir, int32 knockback_y_dir)
{
	if(invulnerability_frames) {return;}
    
    // Object invuln:
	invulnerability_frames = PLAYER_HIT_INVULNERABILITY_FRAMES;

    // Global juice
    global_bg_hitflash_frames   = GENERIC_HIT_HITSTOP_FRAMES;
    global_hitstop_frames       = GENERIC_HIT_HITSTOP_FRAMES;
    global_screenshake_frames   = GENERIC_HIT_SCREENSHAKE_FRAMES;
    global_screenshake_severity = GENERIC_HIT_SCREENSHAKE_SEVERITY;

    // Object physics:
    rigidbody.removeForces();
    rigidbody.addForce(Force(bn::fixed_point_t<12>(GENERIC_HIT_X_KNOCKBACK * knockback_x_dir, 
                                                   GENERIC_HIT_Y_KNOCKBACK * knockback_y_dir), 
                                                   GENERIC_HIT_KNOCKBACK_DECAY));

    // Object damage:
    applyDamage(_damage);

    // Object hitstun state:
    hitstun_frames = GENERIC_HIT_HITSTUN_FRAMES;
    setState(OBJECT_HITSTUN);

    // Object juice:
    setHitFlash();
    applyHitEffect(x().integer(),
                   y().integer());
}

//////////////////////////////
// State Function Overrides //
//////////////////////////////

void Player::updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
								const bn::regular_bg_ptr&                      bg_ptr, 
								const bn::span<const bn::regular_bg_map_cell>& cells,
								const bn::regular_bg_item&                     bg_item,
								const bn::camera_ptr&                          camera)
{
	bool clear_to_jump = true;

	//////////////////////////
    // Player State Machine //
    //////////////////////////
	
    switch(state)
    {
		case PLAYER_GROUNDED_NEUTRAL:

			///////////////////////////////////
			// Player Grounded Neutral State //
			///////////////////////////////////
			
			///////////////
			// Get Input //
			///////////////

			// Attack Ground 1
			if(bn::keypad::b_pressed() || attack_buffered_frames)
			{setState(PLAYER_ATTACK_GROUND_1);}

			// Jump
			else if(bn::keypad::a_pressed() || jump_buffered_frames)
			{
				if(late_roll_jump_grace_frames) {rollJump();}
				else {jump();}
			}

			// Roll
			else if(bn::keypad::r_pressed() || roll_buffered_frames) 
			{setState(PLAYER_ROLL);}

			// Add Gravity 
			if(!grounded_detected)
			{
				rigidbody.addForce(PLAYER_GRAVITY_FORCE);
				
				if(air_frames_elapsed >= PLAYER_PROLONGED_AIR_FRAMES_REQUIRED)
				{rigidbody.addForce(PLAYER_PROLONGED_GRAVITY_FORCE);}
			}

		break;
	
		case PLAYER_WALK:

			///////////////////////
			// Player Walk State //
			///////////////////////

			///////////////
			// Get Input //
			///////////////

			// Update walk speed //
			if(bn::keypad::left_released())        
			{
				rigidbody.addForce(PLAYER_X_LEFT_DECAY_FORCE);
				x_speed = PLAYER_MIN_X_SPEED;
			}
			else if (bn::keypad::right_released()) 
			{
				rigidbody.addForce(PLAYER_X_RIGHT_DECAY_FORCE);
				x_speed = PLAYER_MIN_X_SPEED;
			}

			// Simulate friction/momentum
			if((bn::keypad::left_held() || bn::keypad::right_held()))  
			{
				x_speed += X_SPEED_ACC_RATE;
				x_speed = clamp(PLAYER_MIN_X_SPEED, PLAYER_MAX_X_SPEED, x_speed);
			}
			
			// Walk
			if(bn::keypad::left_held())       
			{
				rigidbody.addForce(PLAYER_X_LEFT_FORCE);
				if(!late_roll_jump_grace_frames) {x_dir = LEFT;}
			}

			else if(bn::keypad::right_held()) 
			{
				rigidbody.addForce(PLAYER_X_RIGHT_FORCE); 
				if(!late_roll_jump_grace_frames) {x_dir = RIGHT;}
			}

			// Attack Ground 1
			if(bn::keypad::b_pressed() || attack_buffered_frames)
			{setState(PLAYER_ATTACK_GROUND_1);}

			// Jump
			else if(bn::keypad::a_pressed() || jump_buffered_frames) 
			{
				if(late_roll_jump_grace_frames) {rollJump();}
				else {jump();}
			}

			// Roll
			else if(bn::keypad::r_pressed() || roll_buffered_frames) {setState(PLAYER_ROLL);}

			// Add Gravity 
			if(!grounded_detected)
			{
				rigidbody.addForce(PLAYER_GRAVITY_FORCE);
				
				if(air_frames_elapsed >= PLAYER_PROLONGED_AIR_FRAMES_REQUIRED)
				{rigidbody.addForce(PLAYER_PROLONGED_GRAVITY_FORCE);}
			}

		break;

		case PLAYER_AIR_NEUTRAL:

			//////////////////////////////
			// Player Air Neutral State //
			//////////////////////////////

			///////////////
			// Get Input //
			///////////////

			// Update drift speed //
			// Simulate friction/momentum
			if((bn::keypad::left_held() || bn::keypad::right_held()))  
			{
				x_speed += X_SPEED_ACC_RATE;
				x_speed = clamp(PLAYER_MIN_X_SPEED, PLAYER_MAX_X_SPEED, x_speed);
			}

			// Simulate momentum
			if(bn::keypad::left_released())        
			{
				rigidbody.addForce(PLAYER_X_LEFT_DECAY_FORCE);
				x_speed = PLAYER_MIN_X_SPEED;
			}
			else if (bn::keypad::right_released()) 
			{
				rigidbody.addForce(PLAYER_X_RIGHT_DECAY_FORCE);
				x_speed = PLAYER_MIN_X_SPEED;
			}

			// Drift
			if(bn::keypad::left_held())       
			{rigidbody.addForce(PLAYER_X_LEFT_FORCE); x_dir = LEFT;}

			else if(bn::keypad::right_held())
			{rigidbody.addForce(PLAYER_X_RIGHT_FORCE); x_dir = RIGHT;}

			// Attack Air 1
			if(bn::keypad::b_pressed() || attack_buffered_frames)
			{setState(PLAYER_ATTACK_AIR_1);}

			// Fast Fall
			if(bn::keypad::down_held() && 
			   rigidbody.normalized_dir.y() > 0 && 
			   air_frames_elapsed >= PLAYER_MIN_FAST_FALL_FRAMES)
			{fastFall();}
	
			// Jumps
			if(bn::keypad::a_pressed()) 
			{
				// Wall Jump
				if(right_wj_eligible)
				{
					x_dir = LEFT;
					wallJump();
				}
				else if(left_wj_eligible)
				{
					x_dir = RIGHT;
					wallJump();
				}
				// Late Roll Jump
				else if(late_roll_jump_grace_frames) {rollJump();}
				// Late Jump
				else if(late_jump_grace_frames) {jump();}
				// Buffer Jump
				else {jump_buffered_frames = PLAYER_JUMP_BUFFER_FRAMES;}
			}

			// Buffer Roll
			if(bn::keypad::r_pressed())
			{roll_buffered_frames = PLAYER_ROLL_BUFFER_FRAMES;}

			// High Jump
			if(bn::keypad::a_held() && remaining_jump_input_frames > 0)
			{rigidbody.addForce(PLAYER_SECONDARY_JUMP_FORCE);}

			else if(bn::keypad::a_released()) 
			{remaining_jump_input_frames = 0;}

			// Tertiary Jump
			if(bn::keypad::a_held() && !bn::keypad::down_held())
			{rigidbody.addForce(PLAYER_TERTIARY_JUMP_FORCE);}

			// Add Gravity //
			rigidbody.addForce(PLAYER_GRAVITY_FORCE);
			if(air_frames_elapsed >= PLAYER_PROLONGED_AIR_FRAMES_REQUIRED)
			{rigidbody.addForce(PLAYER_PROLONGED_GRAVITY_FORCE);}

			// Update Remaining Jump Input Frames //
			remaining_jump_input_frames--;
			remaining_jump_input_frames = clamp(0, 
												PLAYER_MAX_JUMP_INPUT_FRAMES, 
												remaining_jump_input_frames);

			// Update grace frames for a late jump //
			late_jump_grace_frames--;
			late_jump_grace_frames = clamp(0, 
										PLAYER_LATE_JUMP_GRACE_FRAMES, 
										late_jump_grace_frames);

		break;

		case PLAYER_WALL_SLIDE_RIGHT:

			///////////////////////////////////
			// Player Wall Slide Right State //
			///////////////////////////////////

			///////////////
			// Get Input //
			///////////////

			// Simulate friction/momentum
			if(bn::keypad::left_held())  
			{
				x_speed += X_SPEED_ACC_RATE;
				x_speed = clamp(PLAYER_MIN_X_SPEED, PLAYER_MAX_X_SPEED, x_speed);
			}
	
			// Drift
			if(bn::keypad::left_held()) 
			{rigidbody.addForce(PLAYER_X_LEFT_FORCE);}

			// Wall Jump
			if(bn::keypad::a_pressed() || jump_buffered_frames)
			{
				x_dir = LEFT;
				wallJump();
			}
			
			// Add Gravity //
			if(update_timer % 2 == 0)
			{rigidbody.addForce(PLAYER_WALL_GRAVITY_FORCE);}
			
		break;
		
		case PLAYER_WALL_SLIDE_LEFT:

			//////////////////////////////////
			// Player Wall Slide Left State //
			//////////////////////////////////

			///////////////
			// Get Input //
			///////////////

			// Simulate friction/momentum
			if(bn::keypad::right_held())  
			{
				x_speed += X_SPEED_ACC_RATE;
				x_speed = clamp(PLAYER_MIN_X_SPEED, PLAYER_MAX_X_SPEED, x_speed);
			}

			// Drift
			if(bn::keypad::right_held())
			{rigidbody.addForce(PLAYER_X_RIGHT_FORCE);}

			// Wall Jump
			if(bn::keypad::a_pressed() || jump_buffered_frames)
			{
				x_dir = RIGHT;
				wallJump();
			}
			
			// Add Gravity //
			if(update_timer % 2 == 0)
			{rigidbody.addForce(PLAYER_WALL_GRAVITY_FORCE);}
					
		break;

		case PLAYER_PHASE_STEP:
			
			{
				// Get current cell index that player resides in:
				int32 half_level_width_pixels  = bg_ptr.dimensions().width()  / 2;
				int32 half_level_height_pixels = bg_ptr.dimensions().height() / 2;
				bn::fixed index_x = (x() + half_level_width_pixels  + collider_offset_x) / TILE_WIDTH;
				bn::fixed index_y = (y() + half_level_height_pixels + collider_offset_y) / TILE_HEIGHT;
				bn::point cell_index = bn::point(index_x.integer(), index_y.integer());

				Collider other_collider;

				// Check if player can cancel the phase:
				for(int32 y = -2; y < 3; y++)
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

						// 2. If the tile is collidable make a temporary collider based on type//

						if(tile_index >= HARD_BLOCK_MIN_INDEX && 
						   tile_index <= HARD_BLOCK_MAX_INDEX)
						{
							
							other_collider = Collider(world_x, 
														world_y, 
														TILE_WIDTH,
														TILE_HEIGHT);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}		
						
						else if(tile_index == LEFT_SHALLOW_SLOPE_1_INDEX)
						{

							other_collider = Collider(world_x, 
														world_y + 3, 
														TILE_WIDTH, 
														TILE_HEIGHT / 4);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}
							
						else if(tile_index == LEFT_SHALLOW_SLOPE_2_INDEX)
						{
							other_collider = Collider(world_x, 
														world_y + 2, 
														TILE_WIDTH, 
														TILE_HEIGHT / 2);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}

						else if(tile_index == LEFT_SHALLOW_SLOPE_3_INDEX)
						{

							other_collider = Collider(world_x, 
														world_y + 1, 
														TILE_WIDTH, 
														TILE_HEIGHT - 2);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}

						else if(tile_index == LEFT_SHALLOW_SLOPE_4_INDEX)
						{

							other_collider = Collider(world_x, 
														world_y, 
														TILE_WIDTH, 
														TILE_HEIGHT);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}

						else if(tile_index == LEFT_STEEP_SLOPE_1_INDEX)
						{
							
							other_collider = Collider(world_x, 
														world_y + 2, 
														TILE_WIDTH, 
														TILE_HEIGHT / 2);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}

						else if(tile_index == LEFT_STEEP_SLOPE_2_INDEX)
						{

							other_collider = Collider(world_x, 
														world_y, 
														TILE_WIDTH, 
														TILE_HEIGHT);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}

						else if(tile_index == RIGHT_SHALLOW_SLOPE_1_INDEX)
						{

							other_collider = Collider(world_x, 
														world_y + 3, 
														TILE_WIDTH, 
														TILE_HEIGHT / 4);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}

						else if(tile_index == RIGHT_SHALLOW_SLOPE_2_INDEX)
						{

							other_collider = Collider(world_x, 
														world_y + 2, 
														TILE_WIDTH, 
														TILE_HEIGHT / 2);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}

						else if(tile_index == RIGHT_SHALLOW_SLOPE_3_INDEX)
						{

							other_collider = Collider(world_x, 
														world_y + 1,
														TILE_WIDTH, 
														TILE_HEIGHT - 2);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}
						
						else if(tile_index == RIGHT_SHALLOW_SLOPE_4_INDEX)
						{

							other_collider = Collider(world_x, 
														world_y,
														TILE_WIDTH, 
														TILE_HEIGHT);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}
						
						else if(tile_index == RIGHT_STEEP_SLOPE_1_INDEX)
						{
						
							other_collider = Collider(world_x, 
														world_y + 2,
														TILE_WIDTH, 
														TILE_HEIGHT / 2);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}
						
						else if(tile_index == RIGHT_STEEP_SLOPE_2_INDEX)
						{

							other_collider = Collider(world_x, 
														world_y,
														TILE_WIDTH, 
														TILE_HEIGHT);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}
						
					}
				}
			}

			///////////////
			// Get Input //
			///////////////

			// Early jump breakout
			if(bn::keypad::a_pressed() &&
			   current_phase_frame >= PLAYER_PHASE_JUMP_LOCKOUT_FRAMES && 
			   clear_to_jump)
			{
				// Jump
				jump();

				// Add force
				if(phase_dir == PHASE_LEFT)
				{rigidbody.addForce(PLAYER_PHASE_STEP_EXIT_FORCE_LEFT);}
				else if(phase_dir == PHASE_RIGHT)
				{rigidbody.addForce(PLAYER_PHASE_STEP_EXIT_FORCE_RIGHT);}

				// Set State
				setState(NONE);

				// Hide Marker
				pm_sprite_ptr->set_visible(false);
			}

			// Keep moving towards phase destination until reached
			else
			{
				// Phasing Up
				if(phase_dir == PHASE_UP && y() >= phase_destination.y())
				{
					setY(y() - PLAYER_PHASE_STEP_SPEED);
					if(y() <= phase_destination.y()) 
					{
						setY(phase_destination.y());
						setState(NONE);
						rigidbody.addForce(PLAYER_PHASE_STEP_EXIT_FORCE_UP);
						late_jump_grace_frames = 0;

						// Stretch
						setVerticalStretch();

						// Hide Marker
						pm_sprite_ptr->set_visible(false);
					}
				}

				// Phasing Down
				else if(phase_dir == PHASE_DOWN && y() <= phase_destination.y())
				{
					setY(y() + PLAYER_PHASE_STEP_SPEED);
					if(y() >= phase_destination.y()) 
					{
						setY(phase_destination.y());
						setState(NONE);
						rigidbody.addForce(PLAYER_PHASE_STEP_EXIT_FORCE_DOWN);

						// Stretch
						setVerticalStretch();

						// Hide Marker
						pm_sprite_ptr->set_visible(false);
					}
				}

				// Phasing Left
				else if(phase_dir == PHASE_LEFT && x() >= phase_destination.x())
				{
					setX(x() - PLAYER_PHASE_STEP_SPEED);
					if(x() <= phase_destination.x()) 
					{
						setX(phase_destination.x());
						setState(NONE);
						rigidbody.addForce(PLAYER_PHASE_STEP_EXIT_FORCE_LEFT);
						x_dir = LEFT;

						// Stretch
						setHorizontalStretch();

						// Hide Marker
						pm_sprite_ptr->set_visible(false);
					}
				}

				// Phasing Right
				else if(phase_dir == PHASE_RIGHT && x() <= phase_destination.x())
				{
					setX(x() + PLAYER_PHASE_STEP_SPEED);
					if(x() >= phase_destination.x()) 
					{
						setX(phase_destination.x());
						setState(NONE);
						rigidbody.addForce(PLAYER_PHASE_STEP_EXIT_FORCE_RIGHT);
						x_dir = RIGHT;

						// Stretch
						setHorizontalStretch();

						// Hide Marker
						pm_sprite_ptr->set_visible(false);
					}
				}
			}

			// Increment phase frame counter
			current_phase_frame++;
			current_phase_frame = clamp(0, 
				                        PLAYER_PHASE_JUMP_LOCKOUT_FRAMES, 
										current_phase_frame);
			
		break;

		case PLAYER_ATTACK_GROUND_1:

			// Create Hitboxes
			if(animate_action_ptr->current_index() == PLAYER_ATTACK_GROUND_1_CREATE_HB_FRAME)
			{createGroundedAttackHitboxes(game_objects, camera);}

			// End condition
			if(animate_action_ptr->done())
			{setState(NONE);}

			///////////////
			// Get Input //
			///////////////

			// Update walk speed //
			if(bn::keypad::left_released())        
			{
				rigidbody.addForce(PLAYER_X_LEFT_DECAY_FORCE);
				x_speed = PLAYER_MIN_X_SPEED;
			}
			else if (bn::keypad::right_released()) 
			{
				rigidbody.addForce(PLAYER_X_RIGHT_DECAY_FORCE);
				x_speed = PLAYER_MIN_X_SPEED;
			}

			// Simulate friction/momentum
			if((bn::keypad::left_held() || bn::keypad::right_held()))  
			{
				x_speed += X_SPEED_ACC_RATE;
				x_speed = clamp(PLAYER_MIN_X_SPEED, PLAYER_MAX_X_SPEED, x_speed);
			}
			
			// Walk
			if(animate_action_ptr->current_index() < PLAYER_ATTACK_GROUND_1_CREATE_HB_FRAME)
			{
				if(bn::keypad::left_held())       
				{rigidbody.addForce(PLAYER_X_LEFT_FORCE); x_dir = LEFT;}

				else if(bn::keypad::right_held()) 
				{rigidbody.addForce(PLAYER_X_RIGHT_FORCE); x_dir = RIGHT;}
			}
			
			// Roll Cancel
			if((roll_requested || bn::keypad::r_pressed()) && 
			   animate_action_ptr->current_index() >= PLAYER_ATTACK_GROUND_1_CREATE_HB_FRAME)
			{setState(PLAYER_ROLL);}

			// Jump Cancel
			if((jump_requested || bn::keypad::a_pressed()) && 
			   animate_action_ptr->current_index() >= PLAYER_ATTACK_GROUND_1_CREATE_HB_FRAME)
			{jump();}

			// Jump Buffer
			//else if(bn::keypad::a_pressed())
			//{jump_buffered_frames = PLAYER_JUMP_BUFFER_FRAMES;}

			// Roll Buffer
			//else if(bn::keypad::r_pressed())
			//{roll_buffered_frames = PLAYER_ROLL_BUFFER_FRAMES;}

			// Add Gravity //
			if(!grounded_detected)
			{
				rigidbody.addForce(PLAYER_GRAVITY_FORCE);

				if(air_frames_elapsed >= PLAYER_PROLONGED_AIR_FRAMES_REQUIRED)
				{rigidbody.addForce(PLAYER_PROLONGED_GRAVITY_FORCE);}
			}

		break;

		case PLAYER_ATTACK_AIR_1:

			///////////////////////////////
			// Player Attack Air 1 State //
			///////////////////////////////

			//////////////////
			// Attack stuff //
			//////////////////

			// Create Hitboxes
			if(animate_action_ptr->current_index() == PLAYER_ATTACK_AIR_1_CREATE_HB_FRAME)
			{createAirAttack1Hitboxes(game_objects, camera);}

			if(animate_action_ptr->done())
			{setState(NONE);}

			///////////////
			// Get Input //
			///////////////

			// Update drift speed //
			// Simulate friction/momentum
			if((bn::keypad::left_held() || bn::keypad::right_held()))  
			{
				x_speed += X_SPEED_ACC_RATE;
				x_speed = clamp(PLAYER_MIN_X_SPEED, PLAYER_MAX_X_SPEED, x_speed);
			}

			// Simulate momentum
			if(bn::keypad::left_released())        
			{
				rigidbody.addForce(PLAYER_X_LEFT_DECAY_FORCE);
				x_speed = PLAYER_MIN_X_SPEED;
			}
			else if (bn::keypad::right_released()) 
			{
				rigidbody.addForce(PLAYER_X_RIGHT_DECAY_FORCE);
				x_speed = PLAYER_MIN_X_SPEED;
			}

			// Drift
			if(animate_action_ptr->current_index() < PLAYER_ATTACK_GROUND_1_CREATE_HB_FRAME)
			{
				if(bn::keypad::left_held())       
				{rigidbody.addForce(PLAYER_X_LEFT_FORCE);}

				else if(bn::keypad::right_held())
				{rigidbody.addForce(PLAYER_X_RIGHT_FORCE);}
			}
			
			// High Jump
			if(bn::keypad::a_held() && remaining_jump_input_frames > 0)
			{rigidbody.addForce(PLAYER_SECONDARY_JUMP_FORCE);}

			else if(bn::keypad::a_released()) 
			{remaining_jump_input_frames = 0;}

			// Roll Cancel
			/*
			if((roll_requested) && 
			        animate_action_ptr->current_index() >= PLAYER_ATTACK_GROUND_1_CREATE_HB_FRAME && 
					grounded_detected)
			{setState(PLAYER_ROLL);}
			*/

			// Buffer Roll
			if(bn::keypad::r_pressed())
			{roll_buffered_frames = PLAYER_ROLL_BUFFER_FRAMES;}

			// Add Gravity //
			if(!grounded_detected)
			{
				rigidbody.addForce(PLAYER_GRAVITY_FORCE);

				if(air_frames_elapsed >= PLAYER_PROLONGED_AIR_FRAMES_REQUIRED)
				{rigidbody.addForce(PLAYER_PROLONGED_GRAVITY_FORCE);}
			}

			// Update Remaining Jump Input Frames //
			remaining_jump_input_frames--;
			remaining_jump_input_frames = clamp(0, 
												PLAYER_MAX_JUMP_INPUT_FRAMES, 
												remaining_jump_input_frames);

			// Update grace frames for a late jump //
			late_jump_grace_frames--;
			late_jump_grace_frames = clamp(0, 
										PLAYER_LATE_JUMP_GRACE_FRAMES, 
										late_jump_grace_frames);

		break;

		case PLAYER_ROLL:

			// Add Roll Force
			rigidbody.addForce(PLAYER_ROLL_FORCE);

			// End condition
			if(animate_action_ptr->done())
			{
				// Reset state
				setState(NONE);

				// Set roll jump grace frames
				late_roll_jump_grace_frames = PLAYER_LATE_ROLL_JUMP_GRACE_FRAMES;

				// Exit Momentum
				x_speed = PLAYER_MAX_X_SPEED;

				if((bn::keypad::left_held()  && x_dir == RIGHT) || 
			       (bn::keypad::right_held() && x_dir == LEFT))
				{x_speed = 0;}  
			}

			///////////////
			// Get Input //
			///////////////
			
			// Distance Influence

			if(bn::keypad::left_held())       
			{
				rigidbody.addForce(PLAYER_X_LEFT_FORCE);
			}

			else if(bn::keypad::right_held()) 
			{
				rigidbody.addForce(PLAYER_X_RIGHT_FORCE); 
			}

			// Roll Jump
			if(bn::keypad::a_pressed()) 
			{rollJump();}

			// Buffer Roll
			if(bn::keypad::r_pressed())
			{roll_buffered_frames = PLAYER_ROLL_BUFFER_FRAMES;}

			// Buffer Attack
			if(bn::keypad::b_pressed())
			{attack_buffered_frames = PLAYER_ATTACK_BUFFER_FRAMES;}

			// Add Gravity //
			if(!grounded_detected)
			{
				rigidbody.addForce(PLAYER_GRAVITY_FORCE);
				if(air_frames_elapsed >= PLAYER_PROLONGED_AIR_FRAMES_REQUIRED)
				{rigidbody.addForce(PLAYER_PROLONGED_GRAVITY_FORCE);}
			}

		break;

		case OBJECT_DEATH:

			updateDeathState();

		break;
		
		default:
		break;
    }
	
	// Reset walljump variables
	left_wj_eligible  = false;
	right_wj_eligible = false;
}

void Player::updateState(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
		                 const bn::regular_bg_ptr&                      bg_ptr,
						 const bn::span<const bn::regular_bg_map_cell>& cells,
					     const bn::regular_bg_item&                     bg_item) 
{
	//////////////////
	// Update State //
	//////////////////

	if(state != PLAYER_ATTACK_GROUND_1 &&
	   state != PLAYER_ATTACK_AIR_1    &&
	   state != PLAYER_PHASE_STEP      &&
	   state != PLAYER_ROLL            &&
	   state != OBJECT_DEATH)
	{
		ObjectState new_state = NONE;

		if(grounded_detected)        
		{
			new_state = PLAYER_GROUNDED_NEUTRAL;

			if(bn::keypad::right_held() || 
			   bn::keypad::left_held()) 
			{new_state = PLAYER_WALK;}
		}
	
		else if(wall_right_detected) 
		{new_state = PLAYER_WALL_SLIDE_RIGHT;}
	
		else if(wall_left_detected)  
		{new_state = PLAYER_WALL_SLIDE_LEFT;}
	
		else 
		{new_state = PLAYER_AIR_NEUTRAL;}
	
		// Set the state

		// If the new state is new and not current state,
		// set the new state.
		if(new_state != state) {setState(new_state);}
	}

	updateOverdriveState();
}

void Player::setState(ObjectState new_state)
{
	// Set State
	state = new_state;

	// Set animations & other state specific variables
	switch(new_state)
	{
		case NONE:
		break;

		case PLAYER_GROUNDED_NEUTRAL:

			late_jump_grace_frames           = PLAYER_LATE_JUMP_GRACE_FRAMES;
			
			animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  		  0,
								  								  		  bn::sprite_items::player.tiles_item(),
								  								  		  PLAYER_NEUTRAL_FRAME,
								  								  		  PLAYER_NEUTRAL_FRAME);

		break;

		case PLAYER_WALK:

			late_jump_grace_frames           = PLAYER_LATE_JUMP_GRACE_FRAMES;

			animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
																	      1,
																	      bn::sprite_items::player.tiles_item(),
																	      1, 2, 3, 4, 5, 6, 7, 8, 9, 
																	      10, 11, 12, 13, 14, 15, 16, 17, 18);

		break;

		case PLAYER_WALL_SLIDE_RIGHT:

			rigidbody.removeForces();
			remaining_jump_input_frames      = 0;
			late_jump_grace_frames           = 0;
			x_dir                            = LEFT;

			animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  2,
								  								  bn::sprite_items::player.tiles_item(),
								  								  42, 42, 42, 43, 43, 43, 44, 44, 44);

		break;

		case PLAYER_WALL_SLIDE_LEFT:
			
			rigidbody.removeForces();
			remaining_jump_input_frames      = 0;
			late_jump_grace_frames           = 0;
			x_dir                            = RIGHT;

			animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  2,
								  								  bn::sprite_items::player.tiles_item(),
								  								  42, 42, 42, 43, 43, 43, 44, 44, 44);

		break;

		case PLAYER_AIR_NEUTRAL:

			animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
																		  0,
																		  bn::sprite_items::player.tiles_item(),
																		  PLAYER_AIR_FRAME_1,
																		  PLAYER_AIR_FRAME_1);

		break;

		case PLAYER_PHASE_STEP:

			rigidbody.removeForces();

			current_phase_frame              = 0;
			remaining_jump_input_frames      = 0;
			late_jump_grace_frames           = PLAYER_LATE_JUMP_GRACE_FRAMES;

			setHorizontalStretch();

			pm_sprite_ptr->set_position(phase_destination.x(), phase_destination.y());
			pm_sprite_ptr->set_visible(true);

		break;

		case PLAYER_ATTACK_GROUND_1:

			animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
																		0,
																		bn::sprite_items::player.tiles_item(),
																		56, 56, 56, 56, 56, 56,
																	    57, 57, 57, 57, 57, 57,
																	    58, 58, 58, 58, 58, 58,
																	    59, 59, 59, 59,
																	    60, 60, 60,
																	    61, 61, 61, 61, 61, 61, 61, 61);

		break;

		case PLAYER_ATTACK_AIR_1:

			animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
																		0,
																		bn::sprite_items::player.tiles_item(),
																		56, 56, 56, 56, 56, 56,
																	    57, 57, 57, 57, 57, 57,
																	    58, 58, 58, 58, 58, 58,
																	    59, 59, 59, 59,
																	    60, 60, 60,
																	    61, 61, 61, 61, 61, 61, 61, 61);

		break;

		case PLAYER_ROLL: 

			x_speed = PLAYER_ROLL_X_SPEED;

			animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
																	   2,
																	   bn::sprite_items::player.tiles_item(),
																	   45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55);

		break;

		case OBJECT_DEATH:

			animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
																		0,
																		bn::sprite_items::player.tiles_item(),
																		21, 22, 23, 24, 25, 26, 27, 28, 29, 
																		28, 29, 30, 31, 32, 33, 34, 35, 36);

		break;

		default:
		break;
	}

}

/////////////////////////
// Collision Overrides //
/////////////////////////

// Level Objects

void Player::resolveTilePassageCollision(GameObject& object)         
{
	if(object.state == TILE_PASSAGE_SHUT &&
	   collider.isCollision(object.collider))
	{
		// Resolve X Axis Collision //
		resolveXAxisCollision(object.collider);

		// Resolve Y Axis Collision //
		resolveYAxisCollision(object.collider);
		v_collision_grace_frames = PLAYER_V_COLLISION_MAX_GRACE_FRAMES * col_y_offset.integer();

		// If there is still collision somehow, must be corner case //
		resolveCornerCollision(object.collider);
	}

	updateTestColliders();

	// Test for, and log grounded collision
	if(object.state == TILE_PASSAGE_SHUT &&
		test_collider.isCollision(object.collider) && 
		rigidbody.normalized_dir.y() >= 0)
	{
		if(rigidbody.final_dir.y() >= PLAYER_MIN_PASSAGE_SPEED)
		{object.setState(TILE_PASSAGE_OPEN);}
		else 
		{
			grounded_detected = true;
			rigidbody.removeYForces();
		}
	}
}

void Player::resolvePhaseOrbUpCollision(GameObject& object)
{
	if(state != PLAYER_PHASE_STEP && collider.isCollision(object.collider))
	{
		phase_destination = ((PhaseOrb*)(&object))->phase_destination;
		phase_dir = PHASE_UP;
		setPos(object.pos());
		setState(PLAYER_PHASE_STEP);
	}
}

void Player::resolvePhaseOrbDownCollision(GameObject& object)
{
	if(state != PLAYER_PHASE_STEP && collider.isCollision(object.collider))
	{
		phase_destination = ((PhaseOrb*)(&object))->phase_destination;
		phase_dir = PHASE_DOWN;
		setPos(object.pos());
		setState(PLAYER_PHASE_STEP);
	}
}

void Player::resolvePhaseOrbLeftCollision(GameObject& object)
{
	if(state != PLAYER_PHASE_STEP && collider.isCollision(object.collider))
	{
		phase_destination = ((PhaseOrb*)(&object))->phase_destination;
		phase_dir = PHASE_LEFT;
		setPos(object.pos());
		setState(PLAYER_PHASE_STEP);
	}
}

void Player::resolvePhaseOrbRightCollision(GameObject& object)
{
	if(state != PLAYER_PHASE_STEP && collider.isCollision(object.collider))
	{
		phase_destination = ((PhaseOrb*)(&object))->phase_destination;
		phase_dir = PHASE_RIGHT;
		setPos(object.pos());
		setState(PLAYER_PHASE_STEP);
	}
}

void Player::resolveFallingPlatformWideCollision(GameObject& object)
{
	if(rigidbody.normalized_dir.y() >= 0 &&
	   collider_y_axis.p4.y() <= object.collider.p1.y() + rigidbody.final_dir.y())
	{
		// Resolve Collision //
		while(collider_y_axis.isCollision(object.collider))
		{
			collider_y_axis.setY(collider_y_axis.y() - 1);
			setY(this->y() - 1);
		}

		updateTestColliders();

		// Test for, and log grounded collision
		if(test_collider.isCollision(object.collider) &&
		   rigidbody.normalized_dir.y() >= 0)
		{
			grounded_detected = true;
			rigidbody.removeYForces();
			
			// Trigger the falling platform
			if(object.state != FALLING_PLATFORM_WIDE_FALLING)
			{
				setY(this->y() - 1); // One pixel adjustment to deal with 
									 // player slipping off platform on frame 1
				object.setState(FALLING_PLATFORM_WIDE_FALLING);
			}
		}
	}
}

void Player::resolveFallingPlatformThinCollision(GameObject& object) 
{
	if(rigidbody.normalized_dir.y() >= 0 &&
	   collider_y_axis.p4.y() <= object.collider.p1.y() + rigidbody.final_dir.y())
	{
		// Resolve Collision //
		while(collider_y_axis.isCollision(object.collider))
		{
			collider_y_axis.setY(collider_y_axis.y() - 1);
			setY(this->y() - 1);
		}

		updateTestColliders();

		// Test for, and log grounded collision
		if(test_collider.isCollision(object.collider) &&
		   rigidbody.normalized_dir.y() >= 0)
		{
			grounded_detected = true;
			rigidbody.removeYForces();
			
			// Trigger the falling platform
			if(object.state != FALLING_PLATFORM_THIN_FALLING)
			{
				setY(this->y() - 1); // One pixel adjustment to deal with 
									 // player slipping off platform on frame 1
				object.setState(FALLING_PLATFORM_THIN_FALLING);
			}
		}
	}
}

void Player::resolvePushBlockCollision(GameObject& object)
{
	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

        // If there is still collision somehow, must be corner case //
        resolveCornerCollision(object.collider);
    }

	updateTestColliders();

	// Test for, and log grounded collision
	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{	
		grounded_detected = true;
		rigidbody.removeYForces();
	}
	
	// Test for wall riding on right side
	if(test_collider_right.isCollision(object.collider))
	{
		right_wj_eligible = true;

		if(rigidbody.normalized_dir.y() >= 0 &&
		   bn::keypad::right_held() && !bn::keypad::down_held())
		{
			wall_right_detected = true;

			// Add some Y force from the pushblock to the player if sliding
			int32 y_force = object.rigidbody.normalized_dir.y().integer();
			rigidbody.addForce(Force(bn::fixed_point_t<12>(0, y_force), 1));
		}
	}

	// Test for wall riding on left side
	if(test_collider_left.isCollision(object.collider))
	{
		left_wj_eligible = true;

		if(rigidbody.normalized_dir.y() >= 0 &&
			bn::keypad::left_held() && !bn::keypad::down_held()) 
		{
			wall_left_detected = true;

			// Add some Y force from the pushblock to the player if sliding
			int32 y_force = object.rigidbody.normalized_dir.y().integer();
			rigidbody.addForce(Force(bn::fixed_point_t<12>(0, y_force), 1));
		}
	}
	
}

void Player::resolvePushBlockMiniCollision(GameObject& object)
{
	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

        // If there is still collision somehow, must be corner case //
        resolveCornerCollision(object.collider);
    }

	updateTestColliders();

	// Test for, and log grounded collision
	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{		
		grounded_detected = true;
		rigidbody.removeYForces();
	}
	
	// Test for wall riding on right side
	if(test_collider_right.isCollision(object.collider))
	{
		right_wj_eligible = true;

		if(rigidbody.normalized_dir.y() >= 0 &&
		   bn::keypad::right_held() && !bn::keypad::down_held())
		{
			wall_right_detected = true;

			// Add some Y force from the pushblock to the player if sliding
			int32 y_force = object.rigidbody.normalized_dir.y().integer();
			rigidbody.addForce(Force(bn::fixed_point_t<12>(0, y_force), 1));
		}
	}

	// Test for wall riding on left side
	if(test_collider_left.isCollision(object.collider))
	{
		left_wj_eligible = true;

		if(rigidbody.normalized_dir.y() >= 0 &&
			bn::keypad::left_held() && !bn::keypad::down_held()) 
		{
			wall_left_detected = true;

			// Add some Y force from the pushblock to the player if sliding
			int32 y_force = object.rigidbody.normalized_dir.y().integer();
			rigidbody.addForce(Force(bn::fixed_point_t<12>(0, y_force), 1));
		}
	}
	
}

void Player::resolveAutoPlatformCollision(GameObject& object)
{
	if(rigidbody.normalized_dir.y() >= 0 &&
	   collider_y_axis.p4.y() <= object.collider.p1.y() + rigidbody.final_dir.y())
	{
		// Resolve Collision //
		while(collider_y_axis.isCollision(object.collider))
		{
			collider_y_axis.setY(collider_y_axis.y() - 1);
			setY(this->y() - 1);
		}

		updateTestColliders();

		// Test for, and log grounded collision
		if(test_collider.isCollision(object.collider) &&
			rigidbody.normalized_dir.y() >= 0)
		{
			grounded_detected = true;
			rigidbody.removeYForces();
		}

	}
}

void Player::resolveSmashBlockLargeCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

        // If there is still collision somehow, must be corner case //
        resolveCornerCollision(object.collider);
    }

	updateTestColliders();

	// Test for, and log grounded collision
	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{	
		grounded_detected = true;
		rigidbody.removeYForces();
	}
	
	// Test for wall riding on right side
	if(test_collider_right.isCollision(object.collider))
	{
		right_wj_eligible = true;

		if(rigidbody.normalized_dir.y() >= 0 &&
		   bn::keypad::right_held() && !bn::keypad::down_held())
		{wall_right_detected = true;}
	}

	// Test for wall riding on left side
	if(test_collider_left.isCollision(object.collider))
	{
		left_wj_eligible = true;

		if(rigidbody.normalized_dir.y() >= 0 &&
			bn::keypad::left_held() && !bn::keypad::down_held()) 
		{wall_left_detected = true;}
	}
}

void Player::resolveSmashBlockMiniCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}
	
	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

        // If there is still collision somehow, must be corner case //
        resolveCornerCollision(object.collider);
    }

	updateTestColliders();

	// Test for, and log grounded collision
	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{	
		grounded_detected = true;
		rigidbody.removeYForces();
	}
	
	// Test for wall riding on right side
	if(test_collider_right.isCollision(object.collider))
	{
		right_wj_eligible = true;

		if(rigidbody.normalized_dir.y() >= 0 &&
		   bn::keypad::right_held() && !bn::keypad::down_held())
		{wall_right_detected = true;}
	}

	// Test for wall riding on left side
	if(test_collider_left.isCollision(object.collider))
	{
		left_wj_eligible = true;

		if(rigidbody.normalized_dir.y() >= 0 &&
			bn::keypad::left_held() && !bn::keypad::down_held()) 
		{wall_left_detected = true;}
	}
}

// Level Enemies
void Player::resolveThornColumnCollision(GameObject& object)  
{
	int32 thorn_collision_x_offset = collider.getCollisionXOffset(object.collider, rigidbody.normalized_dir.x()).integer();
	
	if(state != PLAYER_ROLL)
	{
	   if(collider.isCollision(object.collider) && 
	      hitpoints > 0)
		{
			int32 knockback_x_dir = abs(thorn_collision_x_offset) / thorn_collision_x_offset;
			applyHit(object.damage, knockback_x_dir, 0);
		}

		resolveXAxisCollision(object.collider);
		resolveYAxisCollision(object.collider);
	}
}

void Player::resolveThornBarCollision(GameObject& object)       
{	
	if(state != PLAYER_ROLL)
	{		
		if(collider.isCollision(object.collider) &&
	  	   hitpoints > 0)
		{applyHit(object.damage, 0, 0);}

		resolveXAxisCollision(object.collider);
		resolveYAxisCollision(object.collider);
	}
}

void Player::resolveGroundGhoulCollision(GameObject& object)
{
	if(object.state == OBJECT_HITSTUN || object.state == OBJECT_DEATH) {return;}
	
	if(hitpoints > 0 && 
	   state != PLAYER_ROLL &&
	   collider.isCollision(object.collider))
	{
		int32 knockback_x_dir;

		if(x() > object.x()) {knockback_x_dir = 1;}
		else				 {knockback_x_dir = -1;}

		applyHit(object.damage, knockback_x_dir, 0);
	}
}

// Tiles
void Player::resolveTileCollision(const bn::regular_bg_ptr&                      bg_ptr, 
								  const bn::span<const bn::regular_bg_map_cell>& cells,
								  const bn::regular_bg_item&                     bg_item)
{

	////////////////////////////////////////
    // Update Variables for state testing //
	////////////////////////////////////////
    wall_right_detected   = false;
    wall_left_detected    = false;
	grounded_owp_detected = false;
	grounded_detected     = false;

	///////////////////////////
	// Update test colliders //
	///////////////////////////

	// X and Y axis colliders
	collider_x_axis.setPos(collider.x(), collider.y() - rigidbody.final_dir.y());
	collider_y_axis.setPos(collider.x() - rigidbody.final_dir.x(), collider.y());

	// Wall slide test colliders
	test_collider_right.setPos(collider.x() + PLAYER_WALL_TEST_RAY_LENGTH,
                               collider.y());
	test_collider_left.setPos(collider.x() - PLAYER_WALL_TEST_RAY_LENGTH,
                              collider.y());

	// Grounded test collider
	test_collider = Collider(collider.x(), 
	                         collider.y() + GAME_OBJECT_GROUND_RAY_LENGTH,
							 collider.width, 
							 collider.height);

	// Placeholder collider for other objects
	Collider other_collider;

	///////////////////////////////////////////////////
	// Get current cell index that player resides in //
	///////////////////////////////////////////////////
	int32 half_level_width_pixels  = bg_ptr.dimensions().width() / 2;
	int32 half_level_height_pixels = bg_ptr.dimensions().height() / 2;
	bn::fixed index_x = (x() + half_level_width_pixels)  / TILE_WIDTH;
	bn::fixed index_y = (y() + half_level_height_pixels) / TILE_HEIGHT;
	bn::point cell_index = bn::point(index_x.integer(), index_y.integer());

	if(state != PLAYER_PHASE_STEP)
	{
		// Check cells for collision in the direction the player is facing
		int32 x_check_dir; 
		if(x_dir == LEFT) {x_check_dir = -1;}
		else {x_check_dir = 1;}

		for(int32 y = -2; y < 3; y++)
		{
			for(int32 x = 1 * (x_check_dir * -1); x != 2 * x_check_dir; x += x_check_dir)
			{
				// 1. Get tile type at index //
				int32 check_index_x = cell_index.x() + x;
				int32 check_index_y = cell_index.y() + y;

				// Determine world coords in case we need to make a collider.
				int32 world_x = ((check_index_x * TILE_WIDTH)  - half_level_width_pixels)  + (TILE_WIDTH / 2);
				int32 world_y = ((check_index_y * TILE_HEIGHT) - half_level_height_pixels) + (TILE_HEIGHT / 2);

				uint32 tile_index = getTileAtBGIndex(check_index_x, 
													 check_index_y, 
													 bg_ptr, 
													 cells, 
													 bg_item);
	
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
					if(getTileAtBGIndex(check_index_x + x_check_dir, check_index_y, 
										bg_ptr, cells, bg_item) >= HARD_BLOCK_MIN_INDEX && 
						getTileAtBGIndex(check_index_x + x_check_dir, check_index_y, 
										bg_ptr, cells, bg_item) <= HARD_BLOCK_MAX_INDEX)
					{
						block_w_offset = TILE_WIDTH;
						block_x_offset = (TILE_WIDTH / 2) * x_check_dir;
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

				else if(tile_index == UP_SPIKE_BLOCK_1_INDEX ||
                    	tile_index == UP_SPIKE_BLOCK_2_INDEX)
				{
					other_collider = Collider(world_x,
											  world_y, 
											  TILE_WIDTH, 
											  TILE_HEIGHT);
					
					resolveUpSpikeCollision(other_collider);
				}

				else if(tile_index == DOWN_SPIKE_BLOCK_1_INDEX ||
						tile_index == DOWN_SPIKE_BLOCK_2_INDEX)
				{
					other_collider = Collider(world_x,
											world_y, 
											TILE_WIDTH, 
											TILE_HEIGHT);
					
					resolveDownSpikeCollision(other_collider);
				}

				else if(tile_index == LEFT_SPIKE_BLOCK_1_INDEX ||
						tile_index == LEFT_SPIKE_BLOCK_2_INDEX)
				{
					other_collider = Collider(world_x,
											world_y, 
											TILE_WIDTH, 
											TILE_HEIGHT);
					
					resolveLeftSpikeCollision(other_collider);
				}

				else if(tile_index == RIGHT_SPIKE_BLOCK_1_INDEX || 
						tile_index == RIGHT_SPIKE_BLOCK_2_INDEX)
				{
					other_collider = Collider(world_x,
											world_y, 
											TILE_WIDTH,
											TILE_HEIGHT);
					
					resolveRightSpikeCollision(other_collider);
				}
				
				else if(tile_index == LEFT_SHALLOW_SLOPE_1_INDEX)
				{
					other_collider = Collider(world_x, 
											  world_y + 3, 
											  TILE_WIDTH, 
											  TILE_HEIGHT / 4);

					resolveLeftShallowSlope1Collision(other_collider, world_y);
				}
					
				else if(tile_index == LEFT_SHALLOW_SLOPE_2_INDEX)
				{
					other_collider = Collider(world_x, 
												world_y + 2, 
												TILE_WIDTH, 
												TILE_HEIGHT / 2);

					resolveLeftShallowSlope2Collision(other_collider, world_y);
				}

				else if(tile_index == LEFT_SHALLOW_SLOPE_3_INDEX)
				{
					other_collider = Collider(world_x, 
												world_y + 1, 
												TILE_WIDTH, 
												TILE_HEIGHT - 2);

					resolveLeftShallowSlope3Collision(other_collider, world_y);
				}

				else if(tile_index == LEFT_SHALLOW_SLOPE_4_INDEX)
				{
					other_collider = Collider(world_x, 
												world_y, 
												TILE_WIDTH, 
												TILE_HEIGHT);

					resolveLeftShallowSlope4Collision(other_collider, world_y);
				}

				else if(tile_index == LEFT_STEEP_SLOPE_1_INDEX)
				{
					
					other_collider = Collider(world_x, 
											world_y + 2, 
											TILE_WIDTH, 
											TILE_HEIGHT / 2);

					resolveLeftSteepSlope1Collision(other_collider, world_y);
				}

				else if(tile_index == LEFT_STEEP_SLOPE_2_INDEX)
				{
					other_collider = Collider(world_x, 
											world_y, 
											TILE_WIDTH, 
											TILE_HEIGHT);

					resolveLeftSteepSlope2Collision(other_collider, world_y);
				}

				else if(tile_index == RIGHT_SHALLOW_SLOPE_1_INDEX)
				{
					other_collider = Collider(world_x, 
												world_y + 3, 
												TILE_WIDTH, 
												TILE_HEIGHT / 4);

					resolveRightShallowSlope1Collision(other_collider, world_y);
				}

				else if(tile_index == RIGHT_SHALLOW_SLOPE_2_INDEX)
				{
					other_collider = Collider(world_x, 
												world_y + 2, 
												TILE_WIDTH, 
												TILE_HEIGHT / 2);

					resolveRightShallowSlope2Collision(other_collider, world_y);
				}

				else if(tile_index == RIGHT_SHALLOW_SLOPE_3_INDEX)
				{
					other_collider = Collider(world_x, 
												world_y + 1,
												TILE_WIDTH, 
												TILE_HEIGHT - 2);

					resolveRightShallowSlope3Collision(other_collider, world_y);
				}
				
				else if(tile_index == RIGHT_SHALLOW_SLOPE_4_INDEX)
				{
					other_collider = Collider(world_x, 
												world_y,
												TILE_WIDTH, 
												TILE_HEIGHT);

					resolveRightShallowSlope4Collision(other_collider, world_y);
				}
				
				else if(tile_index == RIGHT_STEEP_SLOPE_1_INDEX)
				{
					other_collider = Collider(world_x, 
												world_y + 2,
												TILE_WIDTH, 
												TILE_HEIGHT / 2);

					resolveRightSteepSlope1Collision(other_collider, world_y);
				}
				
				else if(tile_index == RIGHT_STEEP_SLOPE_2_INDEX)
				{
					other_collider = Collider(world_x, 
											  world_y,
											  TILE_WIDTH, 
											  TILE_HEIGHT);

					resolveRightSteepSlope2Collision(other_collider, world_y);
				}
				
				else if(tile_index >= ONEWAY_BLOCK_MIN_INDEX &&
						tile_index <= ONEWAY_BLOCK_MAX_INDEX)
				{
					other_collider = Collider(world_x, 
											  world_y + ONEWAYBLOCK_COLLIDER_Y_OFFSET, 
											  TILE_WIDTH, 
											  ONEWAYBLOCK_COLLIDER_HEIGHT);

					resolveOneWayBlockCollision(other_collider);
				}
			}
		}
	}
}

void Player::resolveHardBlockCollision(const Collider& other_collider)
{
	if(collider.isCollision(other_collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(other_collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(other_collider);
        v_collision_grace_frames = PLAYER_V_COLLISION_MAX_GRACE_FRAMES * col_y_offset.integer();

        // If there is still collision somehow, must be corner case //
        resolveCornerCollision(other_collider);
    }

	// Update test colliders since the player has moved.
	updateTestColliders();

	// Test for, and log grounded collision
	if(test_collider.isCollision(other_collider) &&
		rigidbody.normalized_dir.y() >= 0)
	{
		grounded_detected = true;
		rigidbody.removeYForces();
	}

	// Test for wall riding on right side
	if(test_collider_right.isCollision(other_collider))
	{
		right_wj_eligible = true;

		if(rigidbody.normalized_dir.y() >= 0 &&
		   bn::keypad::right_held() && 
		   !bn::keypad::down_held()) 
		{wall_right_detected = true;}
	}
	
	// Test for wall riding on left side
	if(test_collider_left.isCollision(other_collider))
	{
		left_wj_eligible = true;

		if(rigidbody.normalized_dir.y() >= 0 &&
			bn::keypad::left_held() &&
			!bn::keypad::down_held()) 
		{wall_left_detected = true;}
	}
}

void Player::resolveOneWayBlockCollision(const Collider& other_collider)
{
	if(rigidbody.normalized_dir.y() >= 0 &&
		collider_y_axis.p4.y() <= other_collider.p1.y() + rigidbody.final_dir.y())
	{
		// Resolve Collision //
		while(collider_y_axis.isCollision(other_collider))
		{
			collider_y_axis.setY(collider_y_axis.y() - 1);
			setY(this->y() - 1);
		}

		updateTestColliders();

		// Test for, and log grounded collision
		if(test_collider.isCollision(other_collider))
		{
			grounded_detected = true;
			rigidbody.removeYForces();
		}
	}
}
