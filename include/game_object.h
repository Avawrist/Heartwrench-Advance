#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

// Butano
#include "bn_optional.h"
#include "bn_camera_ptr.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_profiler.h"

#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_sprite_palette_actions.h"

#include "bn_fixed.h"
#include "bn_fixed_point.h"

#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_map_cell.h"
#include "bn_regular_bg_map_cell_info.h"

// My Libs
#include "room_bounds.h"
#include "utility.h"
#include "physics.h"
#include "collider.h"

// Flash Palette
#include "bn_sprite_palette_items_sprite_flash_palette.h"

// Effect Assets
#include "bn_sprite_items_jump_effect.h"
#include "bn_sprite_items_air_jump_effect.h"
#include "bn_sprite_items_land_effect.h"
#include "bn_sprite_items_hit_effect.h"
#include "bn_sprite_items_wall_splat_effect.h"

// Level Object Assets
#include "bn_sprite_items_tile_passage.h"
#include "bn_sprite_items_phase_orb_up.h"
#include "bn_sprite_items_phase_orb_down.h"
#include "bn_sprite_items_phase_orb_left.h"
#include "bn_sprite_items_phase_orb_right.h"
#include "bn_sprite_items_thorn_column.h"
#include "bn_sprite_items_thorn_bar.h"

// Enemy Object Assets
#include "bn_sprite_items_ceiling_ghoul.h"
#include "bn_sprite_items_ground_ghoul.h"
#include "bn_sprite_items_wall_left_ghoul.h"
#include "bn_sprite_items_wall_right_ghoul.h"
#include "bn_sprite_items_test_enemy.h"

// Special Object Assets
#include "bn_sprite_items_devil_platform.h"
#include "bn_sprite_items_angel_platform.h"
#include "bn_sprite_items_scythe_platform.h"
#include "bn_sprite_items_hitbox.h"
#include "bn_sprite_items_player.h"
#include "bn_sprite_items_phase_marker.h"

///////////////////////
// Struct GameObject //
///////////////////////

#define GAME_OBJECT_MAX_ANIM_FRAMES 40

#define GAME_OBJECT_Z_ORDER   0
#define HIT_EFFECT_Z_ORDER   -1
#define SPLAT_EFFECT_Z_ORDER -2

#define GAME_OBJECT_COLLIDER_WIDTH  8
#define GAME_OBJECT_COLLIDER_HEIGHT 8

#define GAME_OBJECT_IGNORE_OWP_FRAMES 3

#define GAME_OBJECT_HIT_INVULNERABILITY_FRAMES 30
#define GAME_OBJECT_MAX_HIT_FLASH_FRAMES       8

#define GAME_OBJECT_SPRITE_OFFSET_INCREMENT 0.1

#define GAME_OBJECT_MIN_STRETCH_V  0.5
#define GAME_OBJECT_MAX_STRETCH_V  1.9
#define GAME_OBJECT_MIN_STRETCH_H  0.5
#define GAME_OBJECT_MAX_STRETCH_H  1.9

#define GAME_OBJECT_REQUIRED_SPLAT_SPEED 3

#define WALL_SPLAT_X_OFFSET           0
#define WALL_SPLAT_Y_OFFSET           0
#define WALL_SPLAT_HB_WIDTH           0
#define WALL_SPLAT_HB_HEIGHT          0
#define WALL_SPLAT_HB_LIFESPAN_FRAMES 0
#define WALL_SPLAT_X_KNOCKBACK        3
#define WALL_SPLAT_Y_KNOCKBACK        0
#define WALL_SPLAT_KNOCKBACK_DECAY    0.05
#define WALL_SPLAT_DAMAGE             1
#define WALL_SPLAT_HITSTOP_FRAMES     8
#define WALL_SPLAT_HITSTUN_FRAMES     30
#define WALL_SPLAT_SCREENSHAKE_FRAMES 8
#define WALL_SPLAT_SCREENSHAKE_SEVERITY STRONG_SHAKE

// The index order here needs to align with the object tile index order in 
// Aseprite.

enum ObjectType 
{
	NO_TYPE = 0,

	// Level Objects
	TILE_PASSAGE,
	PHASE_ORB_UP,
	PHASE_ORB_DOWN,
	PHASE_ORB_LEFT,
	PHASE_ORB_RIGHT,

	// Level Enemies
	THORN_COLUMN,
	THORN_BAR,
	GROUND_GHOUL,
	CEILING_GHOUL,
	WALL_LEFT_GHOUL,
	WALL_RIGHT_GHOUL,
	TEST_ENEMY,

	// Special Objects
	DEVIL_PLATFORM,
	ANGEL_PLATFORM,
	SCYTHE_PLATFORM,
	HITBOX_ATTACK_GROUND_1,
	HITBOX_ATTACK_AIR_1,
	HITBOX_WALL_SPLAT,
	PLAYER,
};

enum ObjectState
{
	NONE,

	// Player
	PLAYER_GROUNDED_NEUTRAL,
	PLAYER_WALK,
	PLAYER_AIR_NEUTRAL,
	PLAYER_WALL_SLIDE_RIGHT,
	PLAYER_WALL_SLIDE_LEFT,
	PLAYER_PHASE_STEP,
	PLAYER_ATTACK_GROUND_1,
	PLAYER_ATTACK_AIR_1,
	PLAYER_ROLL,

	// Ceiling Ghoul
	CEILING_GHOUL_IDLE,
	CEILING_GHOUL_CRAWL,

	// Ground Ghoul
	GROUND_GHOUL_IDLE,
	GROUND_GHOUL_CRAWL,
	GROUND_GHOUL_AIR,

	// Wall Left Ghoul
	WALL_LEFT_GHOUL_IDLE,
	WALL_LEFT_GHOUL_CRAWL,

	// Wall Right Ghoul
	WALL_RIGHT_GHOUL_IDLE,
	WALL_RIGHT_GHOUL_CRAWL,

	// Scythe Platform
	SCYTHE_PLATFORM_THROWN,

	// Tile Passage
	TILE_PASSAGE_SHUT,
    TILE_PASSAGE_OPEN,

	// Generic
	OBJECT_HITSTUN,
	OBJECT_DEATH,
};

struct GameObject 
{

	bn::optional<bn::sprite_ptr>                                         sprite_ptr;

	bn::optional<bn::sprite_animate_action<GAME_OBJECT_MAX_ANIM_FRAMES>> animate_action_ptr;
	bn::optional<bn::sprite_palette_ptr>                                 default_palette_ptr;

	bn::optional<bn::sprite_ptr>                                         hit_effect_sprite_ptr;
	bn::optional<bn::sprite_animate_action<GAME_OBJECT_MAX_ANIM_FRAMES>> hit_effect_animate_action_ptr;

	bn::optional<bn::sprite_ptr>                                         splat_effect_sprite_ptr;
	bn::optional<bn::sprite_animate_action<GAME_OBJECT_MAX_ANIM_FRAMES>> splat_effect_animate_action_ptr;

	ObjectType  object_type;
	ObjectState state;

	RigidBody rigidbody;

	XDirection x_dir;
	YDirection y_dir;

	Collider collider;
	Collider collider_x_axis;
	Collider collider_y_axis;

	bn::fixed col_x_offset;
	bn::fixed col_y_offset;

	int32 object_id;
	int32 collider_offset_x      = 0;
	int32 collider_offset_y      = 0;
	int32 hit_flash_frames       = 0;
	int32 invulnerability_frames = 0;
	int32 hitstun_frames         = 0;
	int32 hitpoints              = 1;

	bool is_inactive             = false;
	bool is_dead                 = false;
	bool is_persistent           = false;
	bool received_platform_force = false;

	void applyForces();

	GameObject();
	GameObject(const GameObject& other);
	virtual ~GameObject();

	GameObject& operator =(const GameObject& other);

	virtual void update(const RoomBounds& 							   room_bounds,
	                    bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
						const bn::regular_bg_ptr&                      bg_ptr, 
                        const bn::span<const bn::regular_bg_map_cell>& cells,
                        const bn::regular_bg_item&                     bg_item,
						const bn::camera_ptr&                          camera);
	
	virtual void draw();
	virtual void setCamera(const bn::camera_ptr& camera);
	virtual void setState(ObjectState new_state);
	bn::fixed x() const;
	bn::fixed y() const;
	bn::fixed_point pos() const;
	void setX(bn::fixed new_x);
	void setY(bn::fixed new_y);
	void setPos(bn::fixed new_x, bn::fixed new_y);
	void setPos(bn::fixed_point new_pos);
	void setHitStretch();
	void setVerticalStretch();
	void setHorizontalStretch();
	void updateSpriteDirection();
	void updateSpriteOffsets();
	void updateInactiveState(const bn::camera_ptr& camera);
	virtual void updateHitstunState();
	virtual void udpateDeathState();
	void clampPosition(const bn::regular_bg_ptr& bg_ptr);
	void setHitFlash();
	void setHitFlash(int32 frames);
	void updateHitFlash();
	void applyDamage(int32 damage);
	void applyHitEffect(int32 x, int32 y);
	void applySplatEffect(int32 x, int32 y);

	virtual void resolveCollision(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
						          const bn::regular_bg_ptr&                      bg_ptr, 
                                  const bn::span<const bn::regular_bg_map_cell>& cells,
                                  const bn::regular_bg_item&                     bg_item);
	virtual void resolveXAxisCollision(const Collider& other_collider);
    virtual void resolveYAxisCollision(const Collider& other_collider);
    virtual void resolveCornerCollision(const Collider& other_collider);

};

///////////////////////////
// Struct UnloadedObject //
///////////////////////////

#define UNLOADED_OBJECT_STATE_UNLOADED -1
#define UNLOADED_OBJECT_STATE_DEAD     -2

struct UnloadedObject
{
    bn::point  room_pos;
    ObjectType object_type;
	int32      loaded_instance_id;
	bool       is_persistent = false;

    UnloadedObject();
    UnloadedObject(bn::point _room_pos, ObjectType _object_type);
    UnloadedObject(const UnloadedObject& other);
    ~UnloadedObject();

    void operator =(const UnloadedObject& other);
};

# endif
