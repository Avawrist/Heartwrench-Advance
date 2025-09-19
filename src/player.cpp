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

    sprite_ptr  = bn::sprite_items::player_idle.create_sprite(0, 0);
	sprite_ptr->set_z_order(PLAYER_Z_ORDER);
	animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
																  2,
																  bn::sprite_items::player_idle.tiles_item(),
																  0, 0, 0, 0, 
																  1, 1, 1, 
																  2, 2, 2, 2, 
																  3, 3, 3);
	default_palette_ptr = sprite_ptr->palette();

	collider            = Collider(x(), y(), PLAYER_COLLIDER_WIDTH, PLAYER_COLLIDER_HEIGHT);
	collider_x_axis     = collider;
	collider_y_axis     = collider;
	collider_offset_x   = PLAYER_COLLIDER_OFFSET_X;
	collider_offset_y   = PLAYER_COLLIDER_OFFSET_Y;
    
    x_speed        	  	 = PLAYER_MIN_X_SPEED;
	phase_destination    = bn::fixed_point(0, 0);
	ow_target_pos        = bn::fixed_point(global_ow_player_location_x,
		                                   global_ow_player_location_y);

	hitpoints                        = PLAYER_GEARTING_HITPOINTS;
	spin_buffered_frames             = 0;
	jump_buffered_frames             = 0;
	remaining_jump_input_frames      = 0;
	air_frames_elapsed               = 0;
	v_collision_grace_frames         = 0;
	late_jump_grace_frames           = 0;
	late_spin_jump_grace_frames      = 0;
	current_phase_frame              = 0;
	hitstop_frames                   = 0;
	spin_effect_frames               = 0;
	spin_effect_offset_multiplier    = 0;
	currency_collected				 = 0;
	climb_regrab_cooldown            = 0;

	current_climb_index = 0;

	max_hp = PLAYER_MAX_HITPOINTS;
	
	overdrive                = false;
	wall_right_detected      = false;
    wall_left_detected       = false;
	climbable_detected       = false;
	left_wj_eligible         = false;
	right_wj_eligible        = false;
	is_dead                  = false;
	troll_tolls_highlighted  = false;

	a_requested   = false;
	b_requested   = false;
	r_requested   = false;

	hitbox_1_ptr = NULL;
	hitbox_2_ptr = NULL;
	hitbox_3_ptr = NULL;

	phase_dir = PHASE_RIGHT;
	pm_sprite_ptr = bn::sprite_items::phase_marker.create_sprite(0, 0);
	pm_sprite_ptr->set_z_order(PLAYER_Z_ORDER);
	pm_sprite_ptr->set_visible(false);

	jump_effect_sprite_ptr = bn::sprite_items::wall_jump_effect.create_sprite(0, 0);
	jump_effect_sprite_ptr->set_z_order(PLAYER_Z_ORDER);
	jump_effect_sprite_ptr->set_visible(false);

	spin_effect_sprite_1_ptr = bn::sprite_items::player_idle.create_sprite(0, 0);
	spin_effect_sprite_1_ptr->set_z_order(GAME_OBJECT_Z_ORDER);
	spin_effect_sprite_1_ptr->set_visible(false);

	spin_effect_sprite_2_ptr = bn::sprite_items::player_idle.create_sprite(0, 0);
	spin_effect_sprite_2_ptr->set_z_order(SPLAT_EFFECT_Z_ORDER);
	spin_effect_sprite_2_ptr->set_visible(false);

	spin_effect_sprite_3_ptr = bn::sprite_items::player_idle.create_sprite(0, 0);
	spin_effect_sprite_3_ptr->set_z_order(SPIN_EFFECT_Z_ORDER);
	spin_effect_sprite_3_ptr->set_visible(false);

	bn::sprite_palette_ptr sprite_spin_effect_palette = bn::sprite_palette_items::sprite_spin_effect_palette.create_palette();
	spin_effect_sprite_1_ptr->set_palette(sprite_spin_effect_palette);
	spin_effect_sprite_2_ptr->set_palette(sprite_spin_effect_palette);
	spin_effect_sprite_3_ptr->set_palette(sprite_spin_effect_palette);

	setState(PLAYER_ENTRANCE);
}

Player::Player(const Player& other) : GameObject(other)
{
    x_speed        	  	 = other.x_speed;
	phase_destination    = other.phase_destination;
	ow_target_pos        = other.ow_target_pos;
	
	spin_buffered_frames             = other.spin_buffered_frames;
	jump_buffered_frames             = other.jump_buffered_frames;
	remaining_jump_input_frames      = other.remaining_jump_input_frames;
	air_frames_elapsed               = other.air_frames_elapsed;
	v_collision_grace_frames         = other.v_collision_grace_frames;
	late_jump_grace_frames           = other.late_jump_grace_frames;
	late_spin_jump_grace_frames      = other.late_spin_jump_grace_frames;
	current_phase_frame              = other.current_phase_frame;
	hitstop_frames                   = other.hitstop_frames;
	spin_effect_frames               = other.spin_effect_frames;
	spin_effect_offset_multiplier    = other.spin_effect_offset_multiplier;
	currency_collected               = other.currency_collected;
	climb_regrab_cooldown            = other.climb_regrab_cooldown;

	current_climb_index              = other.current_climb_index;

	overdrive                = other.overdrive;
	r_climb                  = other.r_climb;
	wall_right_detected      = other.wall_right_detected;
    wall_left_detected       = other.wall_left_detected;
	climbable_detected       = other.climbable_detected;
	left_wj_eligible         = other.left_wj_eligible;
	right_wj_eligible        = other.right_wj_eligible;
	is_dead                  = other.is_dead;
	troll_tolls_highlighted  = other.troll_tolls_highlighted;

	a_requested   = other.a_requested;
	b_requested   = other.b_requested;
	r_requested   = other.r_requested;

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

	spin_effect_sprite_1_ptr        = other.spin_effect_sprite_1_ptr;
	spin_effect_sprite_2_ptr        = other.spin_effect_sprite_2_ptr;
	spin_effect_sprite_3_ptr        = other.spin_effect_sprite_3_ptr;
	
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

	spin_effect_sprite_1_ptr.reset();
	spin_effect_sprite_2_ptr.reset();
	spin_effect_sprite_3_ptr.reset();
}

Player& Player::operator =(const Player& other)
{
    x_speed        	  	 = other.x_speed;
	phase_destination    = other.phase_destination;
	ow_target_pos        = other.ow_target_pos;
	
	spin_buffered_frames             = other.spin_buffered_frames;
	jump_buffered_frames             = other.jump_buffered_frames;
	remaining_jump_input_frames      = other.remaining_jump_input_frames;
	air_frames_elapsed               = other.air_frames_elapsed;
	v_collision_grace_frames         = other.v_collision_grace_frames;
	late_jump_grace_frames           = other.late_jump_grace_frames;
	late_spin_jump_grace_frames      = other.late_spin_jump_grace_frames;
	current_phase_frame              = other.current_phase_frame;
	hitstop_frames                   = other.hitstop_frames;
	spin_effect_frames               = other.spin_effect_frames;
	spin_effect_offset_multiplier    = other.spin_effect_offset_multiplier;
	currency_collected               = other.currency_collected;
	climb_regrab_cooldown            = other.climb_regrab_cooldown;

	current_climb_index              = other.current_climb_index;

	overdrive                = other.overdrive;
	r_climb                  = other.r_climb;
	wall_right_detected      = other.wall_right_detected;
    wall_left_detected       = other.wall_left_detected;
	climbable_detected       = other.climbable_detected;
	left_wj_eligible         = other.left_wj_eligible;
	right_wj_eligible        = other.right_wj_eligible;
	is_dead                  = other.is_dead;
	troll_tolls_highlighted  = other.troll_tolls_highlighted;

	a_requested   = other.a_requested;
	b_requested   = other.b_requested;
	r_requested   = other.r_requested;

	if(other.hitbox_1_ptr == NULL) {hitbox_1_ptr = NULL;}
	else {hitbox_1_ptr = new Hitbox(*(other.hitbox_1_ptr));}

	if(other.hitbox_2_ptr == NULL) {hitbox_2_ptr = NULL;}
	else {hitbox_2_ptr = new Hitbox(*(other.hitbox_2_ptr));}	
	
	if(other.hitbox_3_ptr == NULL) {hitbox_3_ptr = NULL;}
	else {hitbox_3_ptr = new Hitbox(*(other.hitbox_3_ptr));}

	phase_dir                       = other.phase_dir;
	pm_sprite_ptr                   = other.pm_sprite_ptr;
	jump_effect_sprite_ptr          = other.jump_effect_sprite_ptr;
	jump_effect_anim_ptr            = other.jump_effect_anim_ptr;
	spin_effect_sprite_1_ptr        = other.spin_effect_sprite_1_ptr;
	spin_effect_sprite_2_ptr        = other.spin_effect_sprite_2_ptr;
	spin_effect_sprite_3_ptr        = other.spin_effect_sprite_3_ptr;

	return *this;
}

void Player::jump()
{
	remaining_jump_input_frames = PLAYER_MAX_JUMP_INPUT_FRAMES;
	late_jump_grace_frames      = 0;
	rigidbody.addForce(PLAYER_JUMP_FORCE);

	setJumpAnimation();

	// SFX
	bn::sound_items::jump.play();
}

void Player::spinJump()
{
	if(!late_jump_grace_frames) {return;}

	remaining_jump_input_frames = PLAYER_MAX_JUMP_INPUT_FRAMES;
	late_jump_grace_frames      = 0;
	rigidbody.addForce(PLAYER_JUMP_FORCE);

	// SFX
	bn::sound_items::jump.play();
}

void Player::bellJump()
{
	global_bell_struck = false;

	if(grounded_detected)
	{
		setY(y() + PLAYER_GROUNDED_BELL_JUMP_SUPPLEMENT);
		grounded_detected = false;
	}

	rigidbody.removeForces();
	air_frames_elapsed          = 0;
	remaining_jump_input_frames	= 0;
	late_jump_grace_frames      = 0;
	setState(NONE);
	resetHitboxes();

	rigidbody.addForce(PLAYER_BELL_JUMP_FORCE);

	setBellJumpAnimation();
}

/*
void Player::wallJump()
{
	x_speed = 0;

	rigidbody.removeForces();
	rigidbody.addForce(PLAYER_WALL_JUMP_FORCE);
	remaining_jump_input_frames = PLAYER_MAX_JUMP_INPUT_FRAMES;
	air_frames_elapsed = 0;
	
	createWallJumpEffect();
}
*/

void Player::fastFall()
{
	rigidbody.addForce(PLAYER_FAST_GRAVITY_FORCE);
	//sprite_ptr->set_vertical_scale(PLAYER_FALL_STRETCH_V);
	//sprite_ptr->set_horizontal_scale(PLAYER_FALL_STRETCH_H);
}

void Player::createSpinAttack1Hitbox(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects, 
									 const bn::camera_ptr&                      camera)
{
	// Hitbox 1
	if(game_objects.size() >= MAX_GAME_OBJECTS) {return;}

	delete hitbox_1_ptr;
	hitbox_1_ptr = new Hitbox(bn::point(x().integer() + PLAYER_SPIN_1_X_OFFSET,
								        y().integer() + PLAYER_SPIN_1_Y_OFFSET),
							  PLAYER_SPIN_1_HITSTOP_FRAMES,
							  PLAYER_SPIN_1_HITSTUN_FRAMES,
							  PLAYER_SPIN_1_SCREENSHAKE_FRAMES,
							  PLAYER_SPIN_1_HB_LIFESPAN_FRAMES,
							  PLAYER_SPIN_1_X_KNOCKBACK,
							  PLAYER_SPIN_1_Y_KNOCKBACK,	
							  PLAYER_SPIN_1_KNOCKBACK_DECAY,
							  PLAYER_SPIN_1_HB_WIDTH,
							  PLAYER_SPIN_1_HB_HEIGHT,
							  PLAYER_SPIN_1_DAMAGE,
							  RIGHT,
							  y_dir,
							  HITBOX_SPIN_1,
							  PLAYER_SPIN_1_SCREENSHAKE_SEVERITY);

	hitbox_1_ptr->setCamera(camera);
}

void Player::createSpinAttack2Hitbox(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects, 
									 const bn::camera_ptr&                      camera)
{
	if(game_objects.size() >= MAX_GAME_OBJECTS) {return;}

	delete hitbox_2_ptr;
	hitbox_2_ptr = new Hitbox(bn::point(x().integer() + PLAYER_SPIN_2_X_OFFSET,
								        y().integer() + PLAYER_SPIN_2_Y_OFFSET),
							  PLAYER_SPIN_2_HITSTOP_FRAMES,
							  PLAYER_SPIN_2_HITSTUN_FRAMES,
							  PLAYER_SPIN_2_SCREENSHAKE_FRAMES,
							  PLAYER_SPIN_2_HB_LIFESPAN_FRAMES,
							  PLAYER_SPIN_2_X_KNOCKBACK,
							  PLAYER_SPIN_2_Y_KNOCKBACK,	
							  PLAYER_SPIN_2_KNOCKBACK_DECAY,
							  PLAYER_SPIN_2_HB_WIDTH,
							  PLAYER_SPIN_2_HB_HEIGHT,
							  PLAYER_SPIN_2_DAMAGE,
							  LEFT,
							  y_dir,
							  HITBOX_SPIN_2,
							  PLAYER_SPIN_2_SCREENSHAKE_SEVERITY);

	hitbox_2_ptr->setCamera(camera);
}

void Player::createWallJumpEffect()
{
	if(global_tiles_in_VRAM > MAX_SPRITE_TILES) {return;}

	#define WALL_JUMP_EFFECT_OFFSET -4

	jump_effect_sprite_ptr->set_visible(true);
	jump_effect_sprite_ptr->set_position(x() + ((int32)x_dir * WALL_JUMP_EFFECT_OFFSET), y());
	jump_effect_anim_ptr = bn::create_sprite_animate_action_once(jump_effect_sprite_ptr.value(),
																 1,
																 bn::sprite_items::wall_jump_effect.tiles_item(),
																 0, 0, 1, 1, 2, 2, 3, 3, 4, 4);
}

void Player::drawSpinEffect()
{	
	if(!overdrive)
	{
		spin_effect_sprite_1_ptr->set_visible(false);
		spin_effect_sprite_2_ptr->set_visible(false);
		spin_effect_sprite_3_ptr->set_visible(false);

		return;
	}

	if(spin_effect_frames)
	{
		if(global_timer % 3 == 0)
		{spin_effect_offset_multiplier++;}

		spin_effect_sprite_1_ptr->set_visible(true);
		spin_effect_sprite_2_ptr->set_visible(true);
		spin_effect_sprite_3_ptr->set_visible(true);
	}
	else 
	{
		if(global_timer % 3 == 0)
		{spin_effect_offset_multiplier--;}

		if(spin_effect_offset_multiplier <= 0)
		{
			spin_effect_sprite_1_ptr->set_visible(false);
			spin_effect_sprite_2_ptr->set_visible(false);
			spin_effect_sprite_3_ptr->set_visible(false);
		}
	}
	
	#define MIN_EFFECT_OFFSET_MULTIPLIER 1
	#define MAX_EFFECT_OFFSET_MULTIPLIER 4

	spin_effect_offset_multiplier = clamp(MIN_EFFECT_OFFSET_MULTIPLIER, 
		                                  MAX_EFFECT_OFFSET_MULTIPLIER,
										  spin_effect_offset_multiplier);
	
	if(global_timer % spin_effect_offset_multiplier == 0)
	{
		spin_effect_sprite_3_ptr->set_position(spin_effect_sprite_2_ptr->position());
		spin_effect_sprite_3_ptr->set_tiles(spin_effect_sprite_2_ptr->tiles());
		spin_effect_sprite_3_ptr->set_horizontal_flip(spin_effect_sprite_2_ptr->horizontal_flip());
		spin_effect_sprite_3_ptr->set_vertical_flip(spin_effect_sprite_2_ptr->vertical_flip());

		spin_effect_sprite_2_ptr->set_position(spin_effect_sprite_1_ptr->position());
		spin_effect_sprite_2_ptr->set_tiles(spin_effect_sprite_1_ptr->tiles());
		spin_effect_sprite_2_ptr->set_horizontal_flip(spin_effect_sprite_1_ptr->horizontal_flip());
		spin_effect_sprite_2_ptr->set_vertical_flip(spin_effect_sprite_1_ptr->vertical_flip());

		spin_effect_sprite_1_ptr->set_x((x()).integer());
		spin_effect_sprite_1_ptr->set_y((y()).integer());
		spin_effect_sprite_1_ptr->set_tiles(sprite_ptr->tiles());
		spin_effect_sprite_1_ptr->set_horizontal_flip(sprite_ptr->horizontal_flip());
		spin_effect_sprite_1_ptr->set_vertical_flip(sprite_ptr->vertical_flip());
	}
}

void Player::resetHitboxes()
{
	delete hitbox_1_ptr;
	hitbox_1_ptr = NULL;

	delete hitbox_2_ptr;
	hitbox_2_ptr = NULL;

	delete hitbox_3_ptr;
	hitbox_3_ptr = NULL;
}

void Player::setIdleAnimation()
{
	bn::optional<bn::sprite_ptr> temp_sprite_ptr = bn::sprite_items::player_idle.create_sprite(sprite_ptr->x().integer(), 
																				 			   sprite_ptr->y().integer());
	temp_sprite_ptr->set_camera(sprite_ptr->camera());
	temp_sprite_ptr->set_z_order(sprite_ptr->z_order());
	temp_sprite_ptr->set_visible(sprite_ptr->visible());

	sprite_ptr->swap(temp_sprite_ptr.value());

	temp_sprite_ptr.reset();

	animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
																  2,
																  bn::sprite_items::player_idle.tiles_item(),
																  0, 0, 0, 0, 
																  1, 1, 1, 
																  2, 2, 2, 2, 
																  3, 3, 3);
}

void Player::setWalkAnimation()
{
	bn::optional<bn::sprite_ptr> temp_sprite_ptr = bn::sprite_items::player_walk.create_sprite(sprite_ptr->x().integer(), 
																				 			   sprite_ptr->y().integer());
	temp_sprite_ptr->set_camera(sprite_ptr->camera());
	temp_sprite_ptr->set_z_order(sprite_ptr->z_order());
	temp_sprite_ptr->set_visible(sprite_ptr->visible());

	sprite_ptr->swap(temp_sprite_ptr.value());

	temp_sprite_ptr.reset();

	animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
																  2,
																  bn::sprite_items::player_walk.tiles_item(),
																  0, 0, 0,
																  1, 1, 1,
																  2, 2, 2,
																  3, 3, 3,
																  4, 4, 4,
																  5, 5, 5);
}

void Player::setJumpAnimation()
{
	bn::optional<bn::sprite_ptr> temp_sprite_ptr = bn::sprite_items::player_jump.create_sprite(sprite_ptr->x().integer(), 
																				 			   sprite_ptr->y().integer());
	temp_sprite_ptr->set_camera(sprite_ptr->camera());
	temp_sprite_ptr->set_z_order(sprite_ptr->z_order());
	temp_sprite_ptr->set_visible(sprite_ptr->visible());

	sprite_ptr->swap(temp_sprite_ptr.value());

	temp_sprite_ptr.reset();

	animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
															   2,
															   bn::sprite_items::player_jump.tiles_item(),
															   0, 0, 0, 
															   1, 1, 1, 
															   2, 2, 2);
}

void Player::setFallAnimation()
{
	bn::optional<bn::sprite_ptr> temp_sprite_ptr = bn::sprite_items::player_jump.create_sprite(sprite_ptr->x().integer(), 
																				 			   sprite_ptr->y().integer());
	temp_sprite_ptr->set_camera(sprite_ptr->camera());
	temp_sprite_ptr->set_z_order(sprite_ptr->z_order());
	temp_sprite_ptr->set_visible(sprite_ptr->visible());

	sprite_ptr->swap(temp_sprite_ptr.value());

	temp_sprite_ptr.reset();

	animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
															   0,
															   bn::sprite_items::player_jump.tiles_item(),
															   2, 2);
}

void Player::setBellJumpAnimation()
{
	bn::optional<bn::sprite_ptr> temp_sprite_ptr = bn::sprite_items::player_bell_jump.create_sprite(sprite_ptr->x().integer(), 
																				 	  				sprite_ptr->y().integer());
	temp_sprite_ptr->set_camera(sprite_ptr->camera());
	temp_sprite_ptr->set_z_order(sprite_ptr->z_order());
	temp_sprite_ptr->set_visible(sprite_ptr->visible());

	sprite_ptr->swap(temp_sprite_ptr.value());

	temp_sprite_ptr.reset();

	animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
																3,
																bn::sprite_items::player_bell_jump.tiles_item(),
																0, 2, 3, 4,
																1, 2, 3, 4,
																5, 5, 5, 
																6, 6, 6);
}

void Player::setSpinAttackAnimation()
{
	bn::optional<bn::sprite_ptr> temp_sprite_ptr = bn::sprite_items::player_spin_attack.create_sprite(sprite_ptr->x().integer(), 
																				 	    			  sprite_ptr->y().integer());
	temp_sprite_ptr->set_camera(sprite_ptr->camera());
	temp_sprite_ptr->set_z_order(sprite_ptr->z_order());
	temp_sprite_ptr->set_visible(sprite_ptr->visible());

	sprite_ptr->swap(temp_sprite_ptr.value());

	temp_sprite_ptr.reset();

	animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
															   0,
															   bn::sprite_items::player_spin_attack.tiles_item(),
															   0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3,
															   0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3,
															   0, 0, 0, 0, 0);
}

void Player::setDeathAnimation()
{
	bn::optional<bn::sprite_ptr> temp_sprite_ptr = bn::sprite_items::player_death.create_sprite(sprite_ptr->x().integer(), 
																				  				sprite_ptr->y().integer());
	temp_sprite_ptr->set_camera(sprite_ptr->camera());
	temp_sprite_ptr->set_z_order(sprite_ptr->z_order());
	temp_sprite_ptr->set_visible(sprite_ptr->visible());

	sprite_ptr->swap(temp_sprite_ptr.value());

	temp_sprite_ptr.reset();

	animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
															   1,
															   bn::sprite_items::player_death.tiles_item(),
															   0, 0, 0, 1, 1, 1, 2, 2, 2, 
															   3, 3, 3, 4, 4, 4, 5, 5, 5);
}

void Player::setClimbAnimation()
{
	bn::optional<bn::sprite_ptr> temp_sprite_ptr = bn::sprite_items::player_climb.create_sprite(sprite_ptr->x().integer(), 
																				  				sprite_ptr->y().integer());
	temp_sprite_ptr->set_camera(sprite_ptr->camera());
	temp_sprite_ptr->set_z_order(sprite_ptr->z_order());
	temp_sprite_ptr->set_visible(sprite_ptr->visible());

	sprite_ptr->swap(temp_sprite_ptr.value());

	temp_sprite_ptr.reset();

	animate_action_ptr  = bn::create_sprite_animate_action_once(sprite_ptr.value(),
																0,
																bn::sprite_items::player_climb.tiles_item(),
																0, 0);
}

void Player::setOWIdleAnimation()
{
	bn::optional<bn::sprite_ptr> temp_sprite_ptr = bn::sprite_items::player_ow.create_sprite(sprite_ptr->x().integer(), 
																				  			 sprite_ptr->y().integer());
	temp_sprite_ptr->set_camera(sprite_ptr->camera());
	temp_sprite_ptr->set_z_order(sprite_ptr->z_order());
	temp_sprite_ptr->set_visible(sprite_ptr->visible());

	sprite_ptr->swap(temp_sprite_ptr.value());

	temp_sprite_ptr.reset();

	animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
															   0,
															   bn::sprite_items::player_ow.tiles_item(),
															   PLAYER_OW_DOWN_FRAME, PLAYER_OW_DOWN_FRAME);
}

void Player::setEntranceAnimation()
{
	bn::optional<bn::sprite_ptr> temp_sprite_ptr = bn::sprite_items::player_ow.create_sprite(sprite_ptr->x().integer(), 
																				  			 sprite_ptr->y().integer());
	temp_sprite_ptr->set_camera(sprite_ptr->camera());
	temp_sprite_ptr->set_z_order(sprite_ptr->z_order());
	temp_sprite_ptr->set_visible(sprite_ptr->visible());

	sprite_ptr->swap(temp_sprite_ptr.value());

	temp_sprite_ptr.reset();

	animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
															   1,
															   bn::sprite_items::player_entrance.tiles_item(),
															   0, 0, 1, 1, 2, 2, 3, 3, 
															   4, 4, 5, 5, 6, 6, 7, 7);	
}

void Player::setGetAnimation()
{
	bn::optional<bn::sprite_ptr> temp_sprite_ptr = bn::sprite_items::player_get.create_sprite(sprite_ptr->x().integer(), 
																				  			  sprite_ptr->y().integer());
	temp_sprite_ptr->set_camera(sprite_ptr->camera());
	temp_sprite_ptr->set_z_order(sprite_ptr->z_order());
	temp_sprite_ptr->set_visible(sprite_ptr->visible());

	sprite_ptr->swap(temp_sprite_ptr.value());

	temp_sprite_ptr.reset();

	animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
															   1,
															   bn::sprite_items::player_get.tiles_item(),
														   	   0, 0, 2, 2, 3, 3, 2, 2,
														       1, 1, 2, 2, 3, 3, 2, 2, 1, 1);
}

void Player::setGetExtendedAnimation()
{
	bn::optional<bn::sprite_ptr> temp_sprite_ptr = bn::sprite_items::player_get.create_sprite(sprite_ptr->x().integer(), 
																				  			  sprite_ptr->y().integer());
	temp_sprite_ptr->set_camera(sprite_ptr->camera());
	temp_sprite_ptr->set_z_order(sprite_ptr->z_order());
	temp_sprite_ptr->set_visible(sprite_ptr->visible());

	sprite_ptr->swap(temp_sprite_ptr.value());

	temp_sprite_ptr.reset();

	animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
															   1,
															   bn::sprite_items::player_get.tiles_item(),
														   	   1, 1, 2, 2, 3, 3, 2, 2,
														       1, 1, 2, 2, 3, 3, 2, 2,
															   1, 1, 2, 2, 3, 3, 2, 2,
															   1, 1, 2, 2, 3, 3, 2, 2);
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
		hitbox_1_ptr->setPos(bn::point(x().integer() + PLAYER_SPIN_1_X_OFFSET,
									   y().integer() + PLAYER_SPIN_1_Y_OFFSET));
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
		hitbox_2_ptr->setPos(bn::point(x().integer() + PLAYER_SPIN_2_X_OFFSET,
									   y().integer() + PLAYER_SPIN_2_Y_OFFSET));
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
	invulnerability_frames--;
    if(invulnerability_frames < 0) {invulnerability_frames = 0;}

	// Update grace frames for a late jump //
	if(!grounded_detected)
	{
		late_jump_grace_frames--;
		late_jump_grace_frames = clamp(0, 
									   PLAYER_LATE_JUMP_GRACE_FRAMES, 
									   late_jump_grace_frames);
	}
	else
	{
		late_jump_grace_frames = PLAYER_LATE_JUMP_GRACE_FRAMES;
	}

	// Update Remaining Jump Input Frames //
	remaining_jump_input_frames--;
	remaining_jump_input_frames = clamp(0, 
										PLAYER_MAX_JUMP_INPUT_FRAMES, 
										remaining_jump_input_frames);

	spin_buffered_frames--;
	spin_buffered_frames = clamp(0, 
		 						 PLAYER_SPIN_BUFFER_FRAMES, 
								 spin_buffered_frames);

	jump_buffered_frames--;
	jump_buffered_frames = clamp(0, 
		 						   PLAYER_JUMP_BUFFER_FRAMES, 
								   jump_buffered_frames);

	v_collision_grace_frames--;
	v_collision_grace_frames = clamp(0, 
									 PLAYER_V_COLLISION_MAX_GRACE_FRAMES,
									 v_collision_grace_frames);
	air_frames_elapsed++;
	air_frames_elapsed = clamp(0, 
							   PLAYER_MAX_AIR_FRAMES, 
							   air_frames_elapsed);

	spin_effect_frames--;
	spin_effect_frames = clamp(0, 
		                       PLAYER_MAX_SPIN_EFFECT_FRAMES, 
							   spin_effect_frames);

	climb_regrab_cooldown--;
	climb_regrab_cooldown = clamp(0, 
		 						  PLAYER_CLIMB_REGRAB_CD_FRAMES, 
								  climb_regrab_cooldown);
							   
	if(grounded_detected || state == PLAYER_CLIMB) {air_frames_elapsed = 0;}

	received_platform_force = false;

	a_requested   = false;
	b_requested   = false;
	r_requested   = false;

	if(jump_effect_anim_ptr.has_value())
	{
		if(jump_effect_anim_ptr->done())
		{jump_effect_sprite_ptr->set_visible(false);}
	}
}

void Player::updateHitFlash()
{
    if(hit_flash_frames || (invulnerability_frames && (invulnerability_frames % 3 == 0)))
    {
        bn::sprite_palette_ptr new_palette = bn::sprite_palette_items::player_flash_palette.create_palette();

        if(sprite_ptr.has_value())
        {sprite_ptr->set_palette(new_palette);}
    }
    else {if(sprite_ptr.has_value()) {sprite_ptr->set_palette(default_palette_ptr.value());}}

    hit_flash_frames--;
    if(hit_flash_frames < 0)
    {hit_flash_frames = 0;}
}

void Player::draw()
{
	// Draw Player
	if(animate_action_ptr.has_value() && !animate_action_ptr->done())
    {
		animate_action_ptr->update();
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

	// Draw Spin Effect
	drawSpinEffect();
}

void Player::hideSprites()
{
	GameObject::hideSprites();

	pm_sprite_ptr->set_visible(false);
	jump_effect_sprite_ptr->set_visible(false);
	spin_effect_sprite_1_ptr->set_visible(false);
	spin_effect_sprite_2_ptr->set_visible(false);
	spin_effect_sprite_3_ptr->set_visible(false);
}
	
void Player::revealSprites()
{
	GameObject::revealSprites();

	pm_sprite_ptr->set_visible(true);
	jump_effect_sprite_ptr->set_visible(true);
}

void Player::setCamera(const bn::camera_ptr& camera)
{
	GameObject::setCamera(camera);

	pm_sprite_ptr->set_camera(camera);
	jump_effect_sprite_ptr->set_camera(camera);
	spin_effect_sprite_1_ptr->set_camera(camera);
	spin_effect_sprite_2_ptr->set_camera(camera);
	spin_effect_sprite_3_ptr->set_camera(camera);
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

	// SFX
	bn::sound_items::player_hit.play();
}

void Player::setHitFlash()
{
	hit_flash_frames = GAME_OBJECT_MAX_HIT_FLASH_FRAMES;

    bn::sprite_palette_ptr sprite_palette = bn::sprite_palette_items::player_flash_palette.create_palette();

    if(sprite_ptr.has_value())
    {sprite_ptr->set_palette(sprite_palette);}
}

void Player::setHitFlash(int32 frames)
{
    if(frames > GAME_OBJECT_MAX_HIT_FLASH_FRAMES) 
    {frames = GAME_OBJECT_MAX_HIT_FLASH_FRAMES;}

    hit_flash_frames = frames;

    bn::sprite_palette_ptr sprite_palette = bn::sprite_palette_items::player_flash_palette.create_palette();
    
    if(sprite_ptr.has_value())
    {sprite_ptr->set_palette(sprite_palette);}
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

			// Jump
			if(bn::keypad::a_pressed() || a_requested || jump_buffered_frames)
			{jump();}

			// Bell Jump
			else if(global_bell_struck) {bellJump();}

			// Spin Attack
			else if(bn::keypad::b_pressed() || b_requested || spin_buffered_frames) 
			{setState(PLAYER_SPIN_ATTACK);}

			/////////////////
			// Add Gravity //
			/////////////////

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
				if(!late_spin_jump_grace_frames) {x_dir = LEFT;}
			}

			else if(bn::keypad::right_held()) 
			{
				rigidbody.addForce(PLAYER_X_RIGHT_FORCE); 
				if(!late_spin_jump_grace_frames) {x_dir = RIGHT;}
			}

			// Jump
			if(bn::keypad::a_pressed() || a_requested || jump_buffered_frames) {jump();}

			// Bell Jump
			else if(global_bell_struck) {bellJump();}

			// Spin Attack
			else if(bn::keypad::b_pressed() || b_requested || spin_buffered_frames) {setState(PLAYER_SPIN_ATTACK);}

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

			//////////////////////
			// Update Air Frame //
			//////////////////////
			
			if(animate_action_ptr->done() || (animate_action_ptr->update_forever() && rigidbody.normalized_dir.y() > 0))
			{setFallAnimation();}

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
	
			// Jumps
			if(bn::keypad::a_pressed()) 
			{
				// Right Wall Jump
				//if(right_wj_eligible)
				//{
					//x_dir = LEFT;
					//wallJump();
				//}

				// Left Wall Jump
				//else if(left_wj_eligible)
				//{
					//x_dir = RIGHT;
					//wallJump();
				//}

				// Late Jump
				if(late_jump_grace_frames) {jump();}

				// Buffer Jump
				else {jump_buffered_frames = PLAYER_JUMP_BUFFER_FRAMES;}
			}

			// Bell Jump
			else if(global_bell_struck) {bellJump();}

			// Spin
			else if(bn::keypad::b_pressed() || b_requested || spin_buffered_frames)
			{setState(PLAYER_SPIN_ATTACK);}

			// High Jump
			if(bn::keypad::a_held() && remaining_jump_input_frames > 0)
			{rigidbody.addForce(PLAYER_SECONDARY_JUMP_FORCE);}

			else if(bn::keypad::a_released()) 
			{remaining_jump_input_frames = 0;}

			// Jump Regrab
			if(bn::keypad::a_held() && rigidbody.normalized_dir.y() > 0)
			{rigidbody.addForce(PLAYER_TERTIARY_JUMP_FORCE);}

			/////////////////
			// Add Gravity //
			/////////////////

			rigidbody.addForce(PLAYER_GRAVITY_FORCE);
			if(air_frames_elapsed >= PLAYER_PROLONGED_AIR_FRAMES_REQUIRED)
			{rigidbody.addForce(PLAYER_PROLONGED_GRAVITY_FORCE);}

		break;
		
		/*
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
			if(global_timer % 2 == 0)
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
			if(global_timer % 2 == 0)
			{rigidbody.addForce(PLAYER_WALL_GRAVITY_FORCE);}
					
		break;
		*/

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
						
						/*
						else if(tile_index == SHALLOW_SLOPE_1_INDEX)
						{

							other_collider = Collider(world_x, 
													  world_y + 3, 
													  TILE_WIDTH, 
													  TILE_HEIGHT / 4);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}
							
						else if(tile_index == SHALLOW_SLOPE_2_INDEX)
						{
							other_collider = Collider(world_x, 
													  world_y + 2, 
													  TILE_WIDTH, 
													  TILE_HEIGHT / 2);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}

						else if(tile_index == SHALLOW_SLOPE_3_INDEX)
						{

							other_collider = Collider(world_x, 
													  world_y + 1, 
													  TILE_WIDTH, 
													  TILE_HEIGHT - 2);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}

						else if(tile_index == SHALLOW_SLOPE_4_INDEX)
						{

							other_collider = Collider(world_x, 
													  world_y, 
													  TILE_WIDTH, 
													  TILE_HEIGHT);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}

						else if(tile_index == STEEP_SLOPE_1_INDEX)
						{
							
							other_collider = Collider(world_x, 
													  world_y + 2, 
													  TILE_WIDTH, 
													  TILE_HEIGHT / 2);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}

						else if(tile_index == STEEP_SLOPE_2_INDEX)
						{

							other_collider = Collider(world_x, 
													  world_y, 
													  TILE_WIDTH, 
													  TILE_HEIGHT);

							if(collider.isCollision(other_collider))
							{clear_to_jump = false;}

						}
						*/
					}
				}
			}

			///////////////
			// Get Input //
			///////////////

			// Early jump breakout
			if((bn::keypad::a_pressed() || a_requested || jump_buffered_frames) &&
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

		case PLAYER_SPIN_ATTACK:

			/////////////////////
			// Create Hitboxes //
			/////////////////////

			// HB 1
			if(animate_action_ptr->current_index() == PLAYER_SPIN_1_CREATE_HB_FRAME)
			{createSpinAttack1Hitbox(game_objects, camera);}

			// HB 2
			if(animate_action_ptr->current_index() == PLAYER_SPIN_2_CREATE_HB_FRAME)
			{createSpinAttack2Hitbox(game_objects, camera);}

			// End State
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
			if(bn::keypad::left_held())       
			{rigidbody.addForce(PLAYER_X_LEFT_FORCE);}

			else if(bn::keypad::right_held()) 
			{rigidbody.addForce(PLAYER_X_RIGHT_FORCE);}

			// Spin Jump
			if((bn::keypad::a_pressed() || a_requested || jump_buffered_frames)) {spinJump();}

			// Bell Jump
			else if(global_bell_struck) {bellJump();}

			// Spin Buffer
			else if(bn::keypad::b_pressed() || b_requested)
			{spin_buffered_frames = PLAYER_SPIN_BUFFER_FRAMES;}

			// High Jump
			if(bn::keypad::a_held() && remaining_jump_input_frames > 0)
			{rigidbody.addForce(PLAYER_SECONDARY_JUMP_FORCE);}

			else if(bn::keypad::a_released()) 
			{remaining_jump_input_frames = 0;}

			// Jump Regrab
			if(bn::keypad::a_held() && rigidbody.normalized_dir.y() > 0)
			{rigidbody.addForce(PLAYER_TERTIARY_JUMP_FORCE);}

			/////////////////
			// Add Gravity //
			/////////////////

			rigidbody.addForce(PLAYER_GRAVITY_FORCE);
			if(air_frames_elapsed >= PLAYER_PROLONGED_AIR_FRAMES_REQUIRED)
			{rigidbody.addForce(PLAYER_PROLONGED_GRAVITY_FORCE);}

		break;

		case PLAYER_CLIMB:

			/////////////
			// Animate //
			/////////////

			// Wrap index
			if(current_climb_index >= PLAYER_MAX_CLIMB_INDEX + 1)
			{current_climb_index = PLAYER_MIN_CLIMB_INDEX;}

			animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
																	   0,
																	   bn::sprite_items::player_climb.tiles_item(),
																	   current_climb_index.floor_integer(), 
																	   current_climb_index.floor_integer());

			///////////////
			// Get Input //
			///////////////

			// X Axis climb
			if(bn::keypad::left_held()) 	  
			{
				x_dir = LEFT;
				current_climb_index += 0.1;  
				rigidbody.addForce(PLAYER_CLIMB_LEFT_FORCE);
			}
			else if(bn::keypad::right_held()) 
			{
				x_dir = RIGHT; 
				current_climb_index += 0.1;
				rigidbody.addForce(PLAYER_CLIMB_RIGHT_FORCE);
			}

			// Y Axis climb
			if(bn::keypad::up_held()) 		 
			{
				current_climb_index += 0.1;
				rigidbody.addForce(PLAYER_CLIMB_UP_FORCE);
			}
			else if(bn::keypad::down_held()) 
			{
				current_climb_index += 0.1;
				rigidbody.addForce(PLAYER_CLIMB_DOWN_FORCE);
			}

			// Jump
			if(bn::keypad::a_pressed() || a_requested || jump_buffered_frames)
			{
				climb_regrab_cooldown = PLAYER_CLIMB_REGRAB_CD_FRAMES;
				jump(); 
				setState(NONE);
			}

			// Bell Jump
			else if(global_bell_struck) {bellJump();}

			///////////////
			// End State //
			///////////////

			if(!climbable_detected)
			{
				late_jump_grace_frames = PLAYER_LATE_JUMP_GRACE_FRAMES; 
				setState(NONE);
			}

			if(r_climb && !bn::keypad::r_held())
			{
				r_climb = false;

				late_jump_grace_frames = PLAYER_LATE_JUMP_GRACE_FRAMES; 
				setState(NONE);
			}

		break;

		case PLAYER_OW:

			/////////////////////
			// Update Position //
			/////////////////////

			if(x().integer() < ow_target_pos.x())      {setX(x().integer() + PLAYER_OW_SPEED);}
			else if(x().integer() > ow_target_pos.x()) {setX(x().integer() - PLAYER_OW_SPEED);}

			if(y().integer() < ow_target_pos.y())      {setY(y().integer() + PLAYER_OW_SPEED);}
			else if(y().integer() > ow_target_pos.y()) {setY(y().integer() - PLAYER_OW_SPEED);}

			///////////////
			// Get Input //
			///////////////

			if(pos() == ow_target_pos)
			{
				if(bn::keypad::left_held())
				{
					animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
																			   0,
																			   bn::sprite_items::player_ow.tiles_item(),
																			   PLAYER_OW_LEFT_FRAME, PLAYER_OW_LEFT_FRAME);

					ow_target_pos.set_x(ow_target_pos.x().integer() - PLAYER_OW_STEP_DISTANCE);
				}

				else if(bn::keypad::right_held())
				{
					animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
															0,
															bn::sprite_items::player_ow.tiles_item(),
															PLAYER_OW_RIGHT_FRAME, PLAYER_OW_RIGHT_FRAME);

					ow_target_pos.set_x(ow_target_pos.x().integer() + PLAYER_OW_STEP_DISTANCE);
				}

				else if(bn::keypad::up_held())
				{
					animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
																				0,
																				bn::sprite_items::player_ow.tiles_item(),
																				PLAYER_OW_UP_FRAME, PLAYER_OW_UP_FRAME);

					if(!troll_tolls_highlighted || global_troll_tolls_complete)
					{
						ow_target_pos.set_y(ow_target_pos.y().integer() - PLAYER_OW_STEP_DISTANCE); 					
					}
				}

				else if(bn::keypad::down_held())
				{
					animate_action_ptr = bn::create_sprite_animate_action_once(sprite_ptr.value(),
															0,
															bn::sprite_items::player_ow.tiles_item(),
															PLAYER_OW_DOWN_FRAME, PLAYER_OW_DOWN_FRAME);

					ow_target_pos.set_y(ow_target_pos.y().integer() + PLAYER_OW_STEP_DISTANCE);
				}
			}

		break;

		case PLAYER_ENTRANCE:

			if(animate_action_ptr->done()) {setState(NONE);}

		break;

		case PLAYER_GET:

			if(animate_action_ptr->done())
			{
				air_frames_elapsed = 0;
				setState(NONE);
			}

		break;

		case PLAYER_GET_EXTENDED:

			if(animate_action_ptr->done())
			{setState(PLAYER_GET_EXTENDED);}

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

	// Reset climb variable
	climbable_detected = false;
}

void Player::updateState(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
		                 const bn::regular_bg_ptr&                      bg_ptr,
						 const bn::span<const bn::regular_bg_map_cell>& cells,
					     const bn::regular_bg_item&                     bg_item) 
{
	//////////////////
	// Update State //
	//////////////////

	overdrive = false;
	if(global_level_currency >= PLAYER_OD_CURRENCY_REQUIRED) {overdrive = true;}

	if(state == NONE                        ||
	   state == OBJECT_HITSTUN              ||
	   state == PLAYER_GROUNDED_NEUTRAL     ||
	   state == PLAYER_WALK                 ||
	   state == PLAYER_AIR_NEUTRAL)
	{
		ObjectState new_state = NONE;

		if(grounded_detected)        
		{
			new_state = PLAYER_GROUNDED_NEUTRAL;

			if(bn::keypad::right_held() || 
			   bn::keypad::left_held()) 
			{new_state = PLAYER_WALK;}
		}
	
		//else if(wall_right_detected) 
		//{new_state = PLAYER_WALL_SLIDE_RIGHT;}
	
		//else if(wall_left_detected)  
		//{new_state = PLAYER_WALL_SLIDE_LEFT;}
	
		else 
		{new_state = PLAYER_AIR_NEUTRAL;}
	
		// Set the state

		// If the new state is new and not current state,
		// set the new state.
		if(new_state != state) {setState(new_state);}
	}
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
			
			setIdleAnimation();

		break;

		case PLAYER_WALK:

			setWalkAnimation();

		break;

		/*
		case PLAYER_WALL_SLIDE_RIGHT:

			rigidbody.removeForces();
			remaining_jump_input_frames      = 0;
			late_jump_grace_frames           = 0;
			x_dir                            = LEFT;

			animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  0,
								  								  bn::sprite_items::player.tiles_item(),
								  								  0, 0);

		break;

		case PLAYER_WALL_SLIDE_LEFT:
			
			rigidbody.removeForces();
			remaining_jump_input_frames      = 0;
			late_jump_grace_frames           = 0;
			x_dir                            = RIGHT;

			animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  0,
								  								  bn::sprite_items::player.tiles_item(),
								  								  0, 0);

		break;
		*/

		case PLAYER_AIR_NEUTRAL:
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

		case PLAYER_SPIN_ATTACK:

			spin_effect_frames = PLAYER_MAX_SPIN_EFFECT_FRAMES;
			if(bn::keypad::right_held() || bn::keypad::left_held())
			{rigidbody.addForce(PLAYER_SPIN_FORCE);}

			setSpinAttackAnimation();

			// SFX
			bn::sound_items::spin_attack.play();

		break;

		case PLAYER_CLIMB:

			current_climb_index = PLAYER_MIN_CLIMB_INDEX;

			setClimbAnimation();

			// SFX
			bn::sound_items::player_climb.play();

		break;

		case PLAYER_OW:

			setOWIdleAnimation();

		break;

		case PLAYER_ENTRANCE:

			setEntranceAnimation();

		break;

		case PLAYER_GET:

			rigidbody.removeForces();
			setGetAnimation();

		break;

		case PLAYER_GET_EXTENDED:

			rigidbody.removeForces();
			setGetExtendedAnimation();

		break;

		case OBJECT_DEATH:

			global_hitstop_frames     = PLAYER_DEATH_HITSTOP_FRAMES;
			global_screenshake_frames = PLAYER_DEATH_HITSTOP_FRAMES;
			
			setDeathAnimation();

			// SFX
			bn::sound_items::player_death.play();

		break;

		default:
		break;
	}

}

/////////////////////////
// Collision Overrides //
/////////////////////////

void Player::resolveCollision(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
							  const bn::regular_bg_ptr&                      bg_ptr, 
							  const bn::span<const bn::regular_bg_map_cell>& cells,
							  const bn::regular_bg_item&                     bg_item)
{
	if(is_frozen) {return;}

    ////////////////////////////
    // Resolve Tile Collision //
    ////////////////////////////

	if(state == PLAYER_OW) {resolveOWTileCollision(bg_ptr, cells, bg_item);}
	else                   {resolveTileCollision(bg_ptr, cells, bg_item);}
	

    //////////////////////////////////
    // Resolve GameObject Collision //
    //////////////////////////////////

    resolveObjectCollision(game_objects);
}

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

		// Resolve Corner Collision //
		if(col_y_offset == 0 && col_x_offset == 0)
        {resolveCornerCollision(object.collider);}
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

        // Resolve Corner Collision //
		if(col_y_offset == 0 && col_x_offset == 0)
        {resolveCornerCollision(object.collider);}
    }

	updateTestColliders();

	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{
		grounded_detected = true;
		rigidbody.removeYForces();
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

        // Resolve Corner Collision //
		if(col_y_offset == 0 && col_x_offset == 0)
        {resolveCornerCollision(object.collider);}
    }

	updateTestColliders();

	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{
		grounded_detected = true;
		rigidbody.removeYForces();
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
		
        // Resolve Corner Collision //
		if(col_y_offset == 0 && col_x_offset == 0)
        {resolveCornerCollision(object.collider);}

		// Smash block with player head //
		if(col_y_offset > 0) 
		{
			object.applyHit(object.hitpoints, 0, 0);
			
			rigidbody.removeYForces();
			sprite_ptr->set_horizontal_scale(PLAYER_HEAD_BONK_H_STRETCH);
			sprite_ptr->set_vertical_scale(PLAYER_HEAD_BONK_V_STRETCH);
		}		
    }

	updateTestColliders();

	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{
		grounded_detected = true;
		rigidbody.removeYForces();
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
		
		// Resolve Corner Collision // 
		if(col_y_offset == 0 && col_x_offset == 0)
        {resolveCornerCollision(object.collider);}

		// Smash block with player head //
		if(col_y_offset > 0) 
		{
			object.applyHit(object.hitpoints, 0, 0);
			rigidbody.removeYForces();
			sprite_ptr->set_horizontal_scale(PLAYER_HEAD_BONK_H_STRETCH);
			sprite_ptr->set_vertical_scale(PLAYER_HEAD_BONK_V_STRETCH);
		}
    }

	updateTestColliders();

	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}

void Player::resolveSmashBlockZigguratLCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);
		
        // Resolve Corner Collision //
		if(col_y_offset == 0 && col_x_offset == 0)
        {resolveCornerCollision(object.collider);}

		// Smash block with player head //
		if(col_y_offset > 0) 
		{
			object.applyHit(object.hitpoints, 0, 0);
			
			rigidbody.removeYForces();
			sprite_ptr->set_horizontal_scale(PLAYER_HEAD_BONK_H_STRETCH);
			sprite_ptr->set_vertical_scale(PLAYER_HEAD_BONK_V_STRETCH);
		}		
    }

	updateTestColliders();

	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}

void Player::resolveSmashBlockZigguratCCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);
		
        // Resolve Corner Collision //
		if(col_y_offset == 0 && col_x_offset == 0)
        {resolveCornerCollision(object.collider);}

		// Smash block with player head //
		if(col_y_offset > 0) 
		{
			object.applyHit(object.hitpoints, 0, 0);
			
			rigidbody.removeYForces();
			sprite_ptr->set_horizontal_scale(PLAYER_HEAD_BONK_H_STRETCH);
			sprite_ptr->set_vertical_scale(PLAYER_HEAD_BONK_V_STRETCH);
		}		
    }

	updateTestColliders();

	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}

void Player::resolveSmashBlockZigguratRCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}

	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);
		
        // Resolve Corner Collision //
		if(col_y_offset == 0 && col_x_offset == 0)
        {resolveCornerCollision(object.collider);}

		// Smash block with player head //
		if(col_y_offset > 0) 
		{
			object.applyHit(object.hitpoints, 0, 0);
			
			rigidbody.removeYForces();
			sprite_ptr->set_horizontal_scale(PLAYER_HEAD_BONK_H_STRETCH);
			sprite_ptr->set_vertical_scale(PLAYER_HEAD_BONK_V_STRETCH);
		}		
    }

	updateTestColliders();

	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}

void Player::resolveHPTotemCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}
	
	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

        // Resolve Corner Collision //
		if(col_y_offset == 0 && col_x_offset == 0)
        {resolveCornerCollision(object.collider);}

		// Smash block with player head //
		if(col_y_offset > 0) 
		{
			object.applyHit(object.hitpoints, 0, 0);
			
			rigidbody.removeYForces();
			sprite_ptr->set_horizontal_scale(PLAYER_HEAD_BONK_H_STRETCH);
			sprite_ptr->set_vertical_scale(PLAYER_HEAD_BONK_V_STRETCH);
		}		
    }

	updateTestColliders();

	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}

void Player::resolveHPDropCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH || (!object.is_frozen && object.invulnerability_frames)) {return;}
	
	if(collider.isCollision(object.collider))
    {
		global_hitstop_frames      = PLAYER_GET_HP_HITSTOP_FRAMES;
		global_hud_hp_flash_frames = HUD_FLASH_FRAMES;

		applyHP(object.damage);
		object.setState(OBJECT_DEATH);

		// SFX
		bn::sound_items::heart_get.play();
	}
}

void Player::resolveStarJarCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH || (!object.is_frozen && object.invulnerability_frames)) {return;}
	
	if(collider.isCollision(object.collider))
    {	
		global_hitstop_frames = PLAYER_GET_STAR_HITSTOP_FRAMES;
		object.setState(OBJECT_DEATH);

		setState(PLAYER_GET);

		// SFX
		bn::sound_items::star_jar_get.play();
	}
}

void Player::resolveSkullDropCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH || (!object.is_frozen && object.invulnerability_frames)) {return;}
	
	if(collider.isCollision(object.collider))
    {	
		global_level_currency += object.damage;

		global_hitstop_frames            = PLAYER_GET_SKULL_HITSTOP_FRAMES;
		global_hud_currency_flash_frames = HUD_FLASH_FRAMES;

		object.setState(OBJECT_DEATH);
	}
}

void Player::resolveGearDropCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH || (!object.is_frozen && object.invulnerability_frames)) {return;}
	
	if(collider.isCollision(object.collider))
    {
		global_level_currency += object.damage;

		global_hitstop_frames            = PLAYER_GET_GEAR_HITSTOP_FRAMES;
		global_hud_currency_flash_frames = HUD_FLASH_FRAMES;

		object.setState(OBJECT_DEATH);

		// SFX
		bn::sound_items::gear_get.play();
	}
}

void Player::resolveScrewCollision(GameObject& object)
{	
	if(object.state == OBJECT_DEATH) {return;}

    if(rigidbody.normalized_dir.y() >= 0 &&
       collider_y_axis.p4.y() <= object.collider.p1.y() + rigidbody.final_dir.y())
	{
        // Resolve Collision //
        while(collider_y_axis.isCollision(object.collider))
        {
            collider_y_axis.setY(collider_y_axis.y() - 1);
            setY(this->y() - 1);
        }
	}

    updateTestColliders();

	if(test_collider.isCollision(object.collider) &&
	   rigidbody.normalized_dir.y() >= 0          &&
       test_collider.p4.y() <= object.collider.p1.y() + rigidbody.final_dir.y())
	{
		grounded_detected = true;
		rigidbody.removeYForces();

		// Screw down
		if(state == PLAYER_SPIN_ATTACK       &&
		   rigidbody.normalized_dir.x() == 0 &&
		   global_timer % 4             == 0)
		{
			if(x_dir == RIGHT) 
			{
				setY(y() + PLAYER_SCREW_STEP);
				object.setState(SCREW_DOWN);
			}
			else               
			{
				if(object.collider_offset_y > PLAYER_SCREW_MIN_Y_OFFSET) 
				{setY(y() - PLAYER_SCREW_STEP);}
				
				object.setState(SCREW_UP);
			}
		}
	}
}

void Player::resolveFinishSealCollision(GameObject& object)
{
	if(object.state == OBJECT_DEATH) {return;}
	
	if(collider.isCollision(object.collider))
    {
        // Resolve X Axis Collision //
        resolveXAxisCollision(object.collider);

        // Resolve Y Axis Collision //
        resolveYAxisCollision(object.collider);

        // Resolve Corner Collision //
		if(col_y_offset == 0 && col_x_offset == 0)
        {resolveCornerCollision(object.collider);}
    }

	updateTestColliders();

	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{
		grounded_detected = true;
		rigidbody.removeYForces();
	}	
}

// Level Enemies
void Player::resolveThornColumnCollision(GameObject& object)  
{
	int32 thorn_collision_x_offset = collider.getCollisionXOffset(object.collider, rigidbody.normalized_dir.x()).integer();
	
	if(collider.isCollision(object.collider))
	{
		if(hitpoints > 0)
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
	if(collider.isCollision(object.collider) &&
		hitpoints > 0)
	{
		if(hitpoints > 0) {applyHit(object.damage, 0, 0);}

		resolveXAxisCollision(object.collider);
		resolveYAxisCollision(object.collider);
	}

	updateTestColliders();

	if(test_collider.isCollision(object.collider) && 
	   rigidbody.normalized_dir.y() >= 0)
	{
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}

void Player::resolveGroundGhoulCollision(GameObject& object)
{
	if(object.state == OBJECT_HITSTUN || object.state == OBJECT_DEATH) {return;}
	
	if(hitpoints > 0 && collider.isCollision(object.collider))
	{
		int32 knockback_x_dir;

		if(x() > object.x()) {knockback_x_dir = 1;}
		else				 {knockback_x_dir = -1;}

		applyHit(object.damage, knockback_x_dir, 0);
	}
}

void Player::resolveBellTrollCollision(GameObject& object)
{
	if(object.state == OBJECT_HITSTUN || object.state == OBJECT_DEATH) {return;}
	
	switch(object.state)
	{
		case BELL_TROLL_FROZEN:

			if(collider.isCollision(object.collider))
			{
				// Resolve X Axis Collision //
				resolveXAxisCollision(object.collider);

				// Resolve Y Axis Collision //
				resolveYAxisCollision(object.collider);

				// Resolve Corner Collision //
				if(col_y_offset == 0 && col_x_offset == 0)
				{resolveCornerCollision(object.collider);}
			}

			updateTestColliders();

			if(test_collider.isCollision(object.collider) && 
			rigidbody.normalized_dir.y() >= 0)
			{
				grounded_detected = true;
				rigidbody.removeYForces();
			}

		break;

		default:

			if(hitpoints > 0 && collider.isCollision(object.collider))
			{
				int32 knockback_x_dir;

				if(x() > object.x()) {knockback_x_dir = 1;}
				else				 {knockback_x_dir = -1;}

				applyHit(object.damage, knockback_x_dir, 0);
			}

		break;
	}
}

void Player::resolveWingedTrollLCollision(GameObject& object)
{
	if(object.state == OBJECT_HITSTUN || object.state == OBJECT_DEATH) {return;}
	
	if(hitpoints > 0 && collider.isCollision(object.collider))
	{
		int32 knockback_x_dir;

		if(x() > object.x()) {knockback_x_dir =  1;}
		else				 {knockback_x_dir = -1;}

		applyHit(object.damage, knockback_x_dir, 0);
	}
}

void Player::resolveWingedTrollRCollision(GameObject& object)
{
	if(object.state == OBJECT_HITSTUN || object.state == OBJECT_DEATH) {return;}
	
	if(hitpoints > 0 && collider.isCollision(object.collider))
	{
		int32 knockback_x_dir;

		if(x() > object.x()) {knockback_x_dir =  1;}
		else				 {knockback_x_dir = -1;}

		applyHit(object.damage, knockback_x_dir, 0);
	}
}

// Tiles

void Player::resolveOWTileCollision(const bn::regular_bg_ptr&                      bg_ptr,
								    const bn::span<const bn::regular_bg_map_cell>& cells,
								    const bn::regular_bg_item&                     bg_item)
{
	///////////////////////////////////////////////////
	// Get current cell index that player resides in //
	///////////////////////////////////////////////////

	int32 half_level_width_pixels  = OW_WIDTH  / 2;
	int32 half_level_height_pixels = OW_HEIGHT / 2;
	bn::fixed index_x = (ow_target_pos.x() + half_level_width_pixels)  / TILE_WIDTH;
	bn::fixed index_y = (ow_target_pos.y() + half_level_height_pixels) / TILE_HEIGHT;
	bn::point cell_index = bn::point(index_x.integer(), index_y.integer());

	uint32 tile_index = getTileAtBGIndex(cell_index.x(),
										 cell_index.y(),
										 bg_ptr, 
										 cells,
										 bg_item);

	///////////////////////
	// Resolve collision //
	///////////////////////

	if(tile_index >= OW_HARD_BLOCK_MIN_INDEX &&
	   tile_index <= OW_HARD_BLOCK_MAX_INDEX)
	{
		if(x().integer() < ow_target_pos.x()) 	   
		{ow_target_pos.set_x(ow_target_pos.x() - PLAYER_OW_STEP_DISTANCE);}

		else if(x().integer() > ow_target_pos.x()) 
		{ow_target_pos.set_x(ow_target_pos.x() + PLAYER_OW_STEP_DISTANCE);}

		if(y().integer() < ow_target_pos.y()) 	   
		{ow_target_pos.set_y(ow_target_pos.y() - PLAYER_OW_STEP_DISTANCE);}

		else if(y().integer() > ow_target_pos.y()) 
		{ow_target_pos.set_y(ow_target_pos.y() + PLAYER_OW_STEP_DISTANCE);}
	}
	else if(tile_index >= OW_TROLL_TOLLS_MIN_INDEX &&
	  		tile_index <= OW_TROLL_TOLLS_MAX_INDEX)
	{
		troll_tolls_highlighted = true;
	}
	else
	{
		// Reset highlight bools
		troll_tolls_highlighted = false;
	}

	// Record Player's OW position in global variables
	global_ow_player_location_x = x().integer();
	global_ow_player_location_y = y().integer();
}

void Player::resolveTileCollision(const bn::regular_bg_ptr&                      bg_ptr, 
								  const bn::span<const bn::regular_bg_map_cell>& cells,
								  const bn::regular_bg_item&                     bg_item)
{
	////////////////////////////////////////
    // Update Variables for state testing //
	////////////////////////////////////////

	grounded_detected = false;

	///////////////////////////
	// Update test colliders //
	///////////////////////////

	// X and Y axis colliders
	collider_x_axis.setPos(collider.x(), collider.y() - rigidbody.final_dir.y());
	collider_y_axis.setPos(collider.x() - rigidbody.final_dir.x(), collider.y());

	// Placeholder collider for other objects
	Collider other_collider;

	///////////////////////////////////////////////////
	// Get current cell index that player resides in //
	///////////////////////////////////////////////////

	int32 half_level_width_pixels  = bg_ptr.dimensions().width()  / 2;
	int32 half_level_height_pixels = bg_ptr.dimensions().height() / 2;
	bn::fixed index_x = (x() + half_level_width_pixels  + collider_offset_x) / TILE_WIDTH;
	bn::fixed index_y = (y() + half_level_height_pixels + collider_offset_y) / TILE_HEIGHT;
	bn::point cell_index = bn::point(index_x.integer(), index_y.integer());

	// Check cells for collision in the direction the player is facing
	int32 x_check_dir; 
	if(x_dir == LEFT) {x_check_dir = -1;}
	else {x_check_dir = 1;}

	for(int32 y = -1; y < 2; y++)
	{
		for(int32 x = (x_check_dir * -1); x != 2 * x_check_dir; x += x_check_dir)
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

			else if(tile_index >= SPIKE_MIN_INDEX &&
					tile_index <= SPIKE_MAX_INDEX)
			{
				other_collider = Collider(world_x,
											world_y, 
											TILE_WIDTH, 
											TILE_HEIGHT);
				
				resolveSpikeCollision(other_collider);
			}
			
			/*
			else if(tile_index == SHALLOW_SLOPE_1_INDEX)
			{
				other_collider = Collider(world_x, 
											world_y + 3, 
											TILE_WIDTH, 
											TILE_HEIGHT / 4);

				resolveShallowSlope1Collision(other_collider, world_y);
			}
				
			else if(tile_index == SHALLOW_SLOPE_2_INDEX)
			{
				other_collider = Collider(world_x, 
											world_y + 2, 
											TILE_WIDTH, 
											TILE_HEIGHT / 2);

				resolveShallowSlope2Collision(other_collider, world_y);
			}

			else if(tile_index == SHALLOW_SLOPE_3_INDEX)
			{
				other_collider = Collider(world_x, 
											world_y + 1, 
											TILE_WIDTH, 
											TILE_HEIGHT - 2);

				resolveShallowSlope3Collision(other_collider, world_y);
			}

			else if(tile_index == SHALLOW_SLOPE_4_INDEX)
			{
				other_collider = Collider(world_x, 
											world_y, 
											TILE_WIDTH, 
											TILE_HEIGHT);

				resolveShallowSlope4Collision(other_collider, world_y);
			}

			else if(tile_index == STEEP_SLOPE_1_INDEX)
			{
				
				other_collider = Collider(world_x, 
										world_y + 2, 
										TILE_WIDTH, 
										TILE_HEIGHT / 2);

				resolveSteepSlope1Collision(other_collider, world_y);
			}

			else if(tile_index == STEEP_SLOPE_2_INDEX)
			{
				other_collider = Collider(world_x, 
										world_y, 
										TILE_WIDTH, 
										TILE_HEIGHT);

				resolveSteepSlope2Collision(other_collider, world_y);
			}
			*/
			
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

void Player::resolveYAxisCollision(const Collider& other_collider)
{
	col_y_offset = collider_y_axis.getCollisionYOffset(other_collider, rigidbody.normalized_dir.y());
	collider_y_axis.setY(collider_y_axis.y() + col_y_offset);
	setY(this->y() + col_y_offset);

	if(col_y_offset > 0 && state != PLAYER_CLIMB) 
	{
		rigidbody.removeYForces(); 

		// SFX
		bn::sound_items::player_bonk.play();
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

		// Resolve Corner Collision //
		if(col_y_offset == 0 && col_x_offset == 0)
		{resolveCornerCollision(other_collider);}
    }
}

void Player::resolveClimbableCollision(const Collider& other_collider)
{
	if(collider.isCollision(other_collider))
    {
		// Set bool
		climbable_detected = true;

		if(state != PLAYER_CLIMB && climb_regrab_cooldown <= 0)
		{	
			// Up climb
			if(bn::keypad::up_held())
			{
				// Remove forces
				rigidbody.removeForces();

				// Set state
				setState(PLAYER_CLIMB);
			}

			// R climb
			else if(bn::keypad::r_held())
			{
				r_climb = true;

				// Remove forces
				rigidbody.removeForces();

				// Set state
				setState(PLAYER_CLIMB);
			}
		}
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
	}

    updateTestColliders();

	if(test_collider.isCollision(other_collider) &&
	   rigidbody.normalized_dir.y() >= 0         &&
       test_collider.p4.y() <= other_collider.p1.y() + rigidbody.final_dir.y())
	{
		grounded_detected = true;
		rigidbody.removeYForces();
	}
}