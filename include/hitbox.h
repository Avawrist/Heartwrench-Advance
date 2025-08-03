#ifndef HITBOX_H
#define HITBOX_H

// Butano
#include "bn_math.h"

// Base Class
#include "game_object.h"

// My libs
#include "utility.h"

///////////////////
// Struct Hitbox //
///////////////////

#define HITBOX_30_FPS true

#define HITBOX_KNOCKBACK_FORCE Force(bn::fixed_point_t<12>(x_knockback * x_dir, y_knockback * y_dir), knockback_decay)

struct Hitbox : GameObject
{
    ///////////////////
    // Struct Hitbox //
    ///////////////////

    int32 hitstop_frames;
    int32 hitstun_frames;
    int32 screenshake_frames;
    int32 lifespan_frames;
    int32 current_lifespan_frame;
    int32 x_knockback;
    int32 y_knockback;
    bn::fixed knockback_decay;
    int32 width;
    int32 height;
    int32 damage;

    ScreenShakeSeverity screenshake_severity;

    Hitbox(bn::point  pos,
           int32      _hitstop_frames,
           int32      _hitstun_frames,
           int32      _screenshake_frames,
           int32      _lifespan_frames,
           int32      _x_knockback,
           int32      _y_knockback,
           bn::fixed  _knockback_decay,
           int32      _width,
           int32      _height,
           int32      _damage,
           XDirection _x_dir,
           YDirection _y_dir,
           ObjectType _type,
           ScreenShakeSeverity _screenshake_severity);
    Hitbox(const Hitbox& other);
    ~Hitbox();

    Hitbox& operator =(const Hitbox& other);

    void applyHBHit(GameObject& object);
    void applyWallHit(GameObject& object);

    //////////////////////////
    // GameObject Overrides //
    //////////////////////////

    // None..

    //////////////////////////////
    // State Function Overrides //
    //////////////////////////////

    void updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&        game_objects,
                            const bn::regular_bg_ptr&                         bg_ptr, 
                            const bn::span<const bn::regular_bg_map_cell>&    cells,
                            const bn::regular_bg_item&                        bg_item,
                            const bn::camera_ptr&                             camera) override;

    /////////////////////////
    // Collision Overrides //
    /////////////////////////

    // Level Objects
    void resolvePushBlockCollision(GameObject& object)     override;
    void resolvePushBlockMiniCollision(GameObject& object) override;

    // Level Enemies
    void resolveThornColumnCollision(GameObject& object)     override;
    void resolveThornBarCollision(GameObject& object)        override;
    void resolveGroundGhoulCollision(GameObject& object)     override;
    void resolveSmashBlockLargeCollision(GameObject& object) override;
    void resolveSmashBlockMiniCollision(GameObject& object)  override;
    void resolveLargeVaseCollision(GameObject& object)       override;
    void resolveSmallVaseCollision(GameObject& object)       override;
    void resolveHPTotemCollision(GameObject& object)         override;
    void resolveHPDropCollision(GameObject& object)          override;

    // Tiles
    void resolveTileCollision(const bn::regular_bg_ptr&                      bg_ptr, 
                              const bn::span<const bn::regular_bg_map_cell>& cells,
                              const bn::regular_bg_item&                     bg_item) override;

};

#endif