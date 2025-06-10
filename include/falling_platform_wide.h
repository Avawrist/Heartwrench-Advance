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

#define FALLING_PLATFORM_WIDE_COLLIDER_WIDTH  64
#define FALLING_PLATFORM_WIDE_COLLIDER_HEIGHT 1

#define FALLING_PLATFORM_WIDE_COLLIDER_OFFSET_X 0
#define FALLING_PLATFORM_WIDE_COLLIDER_OFFSET_Y 0

#define FALLING_PLATFORM_WIDE_HITPOINTS 1

#define FALLING_PLATFORM_WIDE_FALL_TIMER 60

#define FALLING_PLATFORM_WIDE_FALL_SPEED 1
#define FALLING_PLATFORM_WIDE_FALL_DECAY 1

#define FALLING_PLATFORM_WIDE_GRAVITY_FORCE Force(bn::fixed_point_t<12>(0, FALLING_PLATFORM_WIDE_FALL_SPEED), FALLING_PLATFORM_WIDE_FALL_DECAY)

struct FallingPlatformWide : GameObject {

    ////////////////////////////////
    // Struct FallingPlatformWide //
    ////////////////////////////////

    int32 fall_timer;

    FallingPlatformWide();
    FallingPlatformWide(const FallingPlatformWide& other);
    ~FallingPlatformWide();

    FallingPlatformWide& operator =(const FallingPlatformWide& other);

    ///////////////////////////
    // GameObjects Overrides //
    ///////////////////////////

    // None..

    //////////////////////////////
    // State Function Overrides //
    //////////////////////////////

    void updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                            const bn::regular_bg_ptr&                      bg_ptr, 
                            const bn::span<const bn::regular_bg_map_cell>& cells,
                            const bn::regular_bg_item&                     bg_item,
                            const bn::camera_ptr&                          camera) override;

    void setState(ObjectState new_state) override;

    /////////////////////////
    // Collision Overrides //
    /////////////////////////

};

#endif