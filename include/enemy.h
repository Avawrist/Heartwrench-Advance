#ifndef ENEMY_H
#define ENEMY_H

#include "game_object.h"
#include "hitbox.h"

//////////////////
// Struct Enemy //
//////////////////

struct Enemy : GameObject
{
    Enemy();
	Enemy(const Enemy& other);
	virtual ~Enemy();

    void wallSplatCheck();

    /////////////////////////
    // Collision Overrides //
    /////////////////////////
    void resolveTilePassageCollision(GameObject& object) override;

    void resolveThornColumnCollision(GameObject& object) override;
    void resolveThornBarCollision(GameObject& object)    override;

    void resolveXAxisCollision(const Collider& other_collider) override;
};

#endif