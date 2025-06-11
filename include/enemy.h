#ifndef ENEMY_H
#define ENEMY_H

#include "game_object.h"
#include "hitbox.h"

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

    // None..

    /////////////////////////
    // Collision Overrides //
    /////////////////////////

    // Level Objects
    void resolveTilePassageCollision(GameObject& object) override;
    void resolveFallingPlatformWideCollision(GameObject& object) override;
    void resolveFallingPlatformThinCollision(GameObject& object) override;

    // Level Enemies
    void resolveThornColumnCollision(GameObject& object) override;
    void resolveThornBarCollision(GameObject& object)    override;

    // Tiles
    void resolveXAxisCollision(const Collider& other_collider) override;
};

#endif