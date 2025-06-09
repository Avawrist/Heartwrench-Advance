#ifndef TILE_PASSAGE_H
#define TILE_PASSAGE_H

// Butano
#include "bn_math.h"

// Base Class
#include "game_object.h"

////////////////////////
// Struct TilePassage //
////////////////////////

#define TILE_PASSAGE_Z_ORDER 10

#define TILE_PASSAGE_COLLIDER_WIDTH  32
#define TILE_PASSAGE_COLLIDER_HEIGHT 16

struct TilePassage : GameObject {

    TilePassage();
	TilePassage(const TilePassage& other);
	~TilePassage();

	TilePassage& operator =(const TilePassage& other);

    void setState(ObjectState new_state) override;

};

#endif