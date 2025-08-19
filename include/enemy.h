#ifndef ENEMY_H
#define ENEMY_H

#include "game_object.h"
#include "hitbox.h"

#define ENEMY_DAMAGE 1

#define ENEMY_MAX_HP 5

#define ENEMY_HP_BAR_X_OFFSET  12
#define ENEMY_HP_BAR_Y_OFFSET -8

struct Enemy : GameObject
{
    //////////////////
    // Struct Enemy //
    //////////////////
    
    Enemy();
	Enemy(const Enemy& other);
	virtual ~Enemy();

    Enemy& operator =(const Enemy& other);

    void wallSplatCheck();

    //////////////////////////
    // GameObject Overrides //
    //////////////////////////

	void updateHitFlash()          override;
    void updateHPBar()             override;
    void setHitFlash()             override;
	void setHitFlash(int32 frames) override;
    void updateDeathState()        override;

    //////////////////////////////
    // State Function Overrides //
    //////////////////////////////

    // None..

    /////////////////////////
    // Collision Overrides //
    /////////////////////////

    // Level Objects
    void resolveTilePassageCollision(GameObject& object)         override;
    void resolveFallingPlatformWideCollision(GameObject& object) override;
    void resolveFallingPlatformThinCollision(GameObject& object) override;
    void resolvePushBlockCollision(GameObject& object)           override;
    void resolvePushBlockMiniCollision(GameObject& object)       override;
    void resolveAutoPlatformCollision(GameObject& object)        override;
    void resolveSmashBlockLargeCollision(GameObject& object)     override;
    void resolveSmashBlockMiniCollision(GameObject& object)      override;
    void resolveSmashBlockZigguratLCollision(GameObject& object) override;
    void resolveSmashBlockZigguratCCollision(GameObject& object) override;
    void resolveSmashBlockZigguratRCollision(GameObject& object) override;
    void resolveLargeVaseCollision(GameObject& object)           override;
    void resolveSmallVaseCollision(GameObject& object)           override;
    void resolveHPTotemCollision(GameObject& object)             override;
    void resolveFinishSealCollision(GameObject& object)          override;

    // Level Enemies
    void resolveThornColumnCollision(GameObject& object) override;
    void resolveThornBarCollision(GameObject& object)    override;
	void resolveGroundGhoulCollision(GameObject& object) override;

    // Tiles
    void resolveXAxisCollision(const Collider& other_collider) override;
};

#endif