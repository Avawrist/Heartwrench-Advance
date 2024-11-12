#ifndef MOVING_PLATFORM_H
#define MOVING_PLATFORM_H

// Butano
#include "bn_optional.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_math.h"

// My Libs
#include "utility.h"
#include "physics.h"
#include "collider.h"

// Assets
#include "bn_sprite_items_moving_platform.h"

// Base Class
#include "game_object.h"
#include "player.h"

///////////////////////////
// Struct MovingPlatform //
///////////////////////////

#define MOVING_PLATFORM_COLLIDER_WIDTH  32
#define MOVING_PLATFORM_COLLIDER_HEIGHT 8

#define MOVING_PLATFORM_SPEED 1
#define MOVING_PLATFORM_DECAY 1

struct MovingPlatform : GameObject {

    bn::optional<bn::sprite_ptr> sprite_ptr;
	bn::optional<bn::sprite_animate_action<MAX_ANIM_FRAMES>> animate_action_ptr;

    RigidBody* rigidbody_ptr = NULL;
    bn::fixed  speed;
    bn::point  target;
    bn::point  next_target;

    MovingPlatform(bn::point p1, bn::point p2);
    ~MovingPlatform();

    void update(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects) override;
	void draw() override;
	void setCamera(const bn::camera_ptr& camera) override;
	
	bn::fixed x() const override;
	bn::fixed y() const override;
	bn::fixed_point pos() const override;
	void setX(bn::fixed new_x) override;
	void setY(bn::fixed new_y) override;
	void setPos(bn::fixed new_x, bn::fixed new_y) override;
	void setPos(bn::fixed_point new_pos) override;

};

#endif