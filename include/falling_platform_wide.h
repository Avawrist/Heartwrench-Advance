#ifndef FALLING_PLATFORM_WIDE_H
#define FALLING_PLATFORM_WIDE_H

// Butano
#include "bn_optional.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_math.h"

// My Libs
#include "utility.h"
#include "physics.h"
#include "collider.h"

// Base Class
#include "game_object.h"

////////////////////////////////
// Struct FallingPlatformWideWide //
////////////////////////////////

#define FALLING_PLATFORM_WIDE_COLLIDER_WIDTH  32
#define FALLING_PLATFORM_WIDE_COLLIDER_HEIGHT 1

#define FALLING_PLATFORM_WIDE_FALL_SPEED 2
#define FALLING_PLATFORM_WIDE_FALL_DECAY 1

#define FALLING_PLATFORM_WIDE_FALL_TIMER 60

struct FallingPlatformWide : GameObject {

    ////////////////////////////////
    // Struct FallingPlatformWide //
    ////////////////////////////////

    bn::fixed fall_speed;
    int32     fall_timer;

    FallingPlatformWide();
    FallingPlatformWide(const FallingPlatformWide& other);
    ~FallingPlatformWide();

    FallingPlatformWide& operator =(const FallingPlatformWide& other);

    //////////////////////////////
    // State Function Overrides //
    //////////////////////////////

    void setState(ObjectState new_state) override;

    /////////////////////////
    // Collision Overrides //
    /////////////////////////

};

#endif