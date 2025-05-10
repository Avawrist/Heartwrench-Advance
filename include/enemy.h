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
    void resolveXAxisCollision(const Collider& other_collider) override;
};

#endif