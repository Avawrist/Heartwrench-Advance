#ifndef ENEMY_H
#define ENEMY_H

#include "game_object.h"
#include "hitbox.h"

#define ENEMY_Z_ORDER -1

#define ENEMY_DAMAGE 1

struct Enemy : GameObject
{
    //////////////////
    // Struct Enemy //
    //////////////////
    
    Enemy();
	Enemy(const Enemy& other);
	virtual ~Enemy();

    void wallSplatCheck();

    //////////////////////////
    // GameObject Overrides //
    //////////////////////////

    // None..

    //////////////////////////////
    // State Function Overrides //
    //////////////////////////////

    // Get State from Level Objects
	void getStateFromTilePassage(GameObject& object)         override;
	void getStateFromFallingPlatformWide(GameObject& object) override;
	void getStateFromFallingPlatformThin(GameObject& object) override;
	void getStateFromPushBlock(GameObject& object)           override;
	void getStateFromPushBlockMini(GameObject& object)       override;
	void getStateFromAutoPlatform(GameObject& object)        override;

    // Get State From Tiles
    void getStateFromHardBlock(int32 world_x, int32 world_y)          override;
	void getStateFromLeftShallowSlope1(int32 world_x, int32 world_y)  override;
	void getStateFromLeftShallowSlope2(int32 world_x, int32 world_y)  override;
	void getStateFromLeftShallowSlope3(int32 world_x, int32 world_y)  override;
	void getStateFromLeftShallowSlope4(int32 world_x, int32 world_y)  override;
	void getStateFromLeftSteepSlope1(int32 world_x, int32 world_y)    override;
	void getStateFromLeftSteepSlope2(int32 world_x, int32 world_y)    override;
	void getStateFromRightShallowSlope1(int32 world_x, int32 world_y) override;
	void getStateFromRightShallowSlope2(int32 world_x, int32 world_y) override;
	void getStateFromRightShallowSlope3(int32 world_x, int32 world_y) override;
	void getStateFromRightShallowSlope4(int32 world_x, int32 world_y) override;
	void getStateFromRightSteepSlope1(int32 world_x, int32 world_y)   override;
	void getStateFromRightSteepSlope2(int32 world_x, int32 world_y)   override;
	void getStateFromOneWayBlock(int32 world_x, int32 world_y)        override;

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

    // Level Enemies
    void resolveThornColumnCollision(GameObject& object) override;
    void resolveThornBarCollision(GameObject& object)    override;

    // Tiles
    void resolveXAxisCollision(const Collider& other_collider) override;
};

#endif