#include "enemy.h"

//////////////////
// Struct Enemy //
//////////////////

Enemy::Enemy()
{

}

Enemy::Enemy(const Enemy& other) : GameObject(other)
{

}

Enemy::~Enemy()
{

}

void Enemy::wallSplatCheck()
{
	if(col_x_offset != 0 &&
	   state == OBJECT_HITSTUN &&
	   abs(rigidbody.final_dir.x().integer()) >= GAME_OBJECT_REQUIRED_SPLAT_SPEED)
	{

		XDirection splat_x_dir;
		if(col_x_offset < 0) {splat_x_dir = LEFT;}
		else                 {splat_x_dir = RIGHT;}

		Hitbox temp_hitbox(bn::point(0, 0),
							WALL_SPLAT_HITSTOP_FRAMES,
							WALL_SPLAT_HITSTUN_FRAMES,
							WALL_SPLAT_SCREENSHAKE_FRAMES,
							WALL_SPLAT_HB_LIFESPAN_FRAMES,
							WALL_SPLAT_X_KNOCKBACK,
							WALL_SPLAT_Y_KNOCKBACK,	
							WALL_SPLAT_KNOCKBACK_DECAY,
							WALL_SPLAT_HB_WIDTH,
							WALL_SPLAT_HB_HEIGHT,
							WALL_SPLAT_DAMAGE,
							splat_x_dir,
							y_dir,
							HITBOX_WALL_SPLAT,
							WALL_SPLAT_SCREENSHAKE_SEVERITY);
		temp_hitbox.applyWallHit(*this);
	}
}

/////////////////////////
// Collision Overrides //
/////////////////////////

void Enemy::resolveThornColumnCollision(GameObject& object)
{
	int32 thorn_collision_x_offset = collider.getCollisionXOffset(object.collider, rigidbody.normalized_dir.x()).integer();
	
	if(thorn_collision_x_offset != 0 &&
	   hitpoints > 0)
	{
		int32 knockback_x_dir = abs(thorn_collision_x_offset) / thorn_collision_x_offset;

		rigidbody.removeForces();
		rigidbody.addForce(Force(bn::fixed_point_t<12>(OBJECT_DEATH_X_FORCE * knockback_x_dir, 
													   OBJECT_DEATH_Y_FORCE * 0),
													   OBJECT_DEATH_DECAY));
		hitpoints = 0;
	}
}

void Enemy::resolveThornBarCollision(GameObject& object)
{
	int32 thorn_collision_y_offset = collider.getCollisionYOffset(object.collider, rigidbody.normalized_dir.y()).integer();
	
	if(thorn_collision_y_offset != 0 && 
	   hitpoints > 0)
	{
		int32 knockback_y_dir = abs(thorn_collision_y_offset) / thorn_collision_y_offset;

		rigidbody.removeForces();
		rigidbody.addForce(Force(bn::fixed_point_t<12>(OBJECT_DEATH_X_FORCE * 0, 
														OBJECT_DEATH_Y_FORCE * knockback_y_dir),
														OBJECT_DEATH_DECAY));
		hitpoints = 0;
	}
}

void Enemy::resolveTilePassageCollision(GameObject& object)
{
	if(object.state == TILE_PASSAGE_SHUT &&
	   collider.isCollision(object.collider))
	{
		// Resolve X Axis Collision //
		BN_LOG(collider.getCollisionXOffset(object.collider, rigidbody.normalized_dir.x()));
		resolveXAxisCollision(object.collider);

		// Resolve Y Axis Collision //
		resolveYAxisCollision(object.collider);

		// If there is still collision somehow, must be corner case //
		resolveCornerCollision(object.collider);
	}
}

void Enemy::resolveXAxisCollision(const Collider& other_collider)
{
	GameObject::resolveXAxisCollision(other_collider);
	
	// Wall splat check
	wallSplatCheck();
}
