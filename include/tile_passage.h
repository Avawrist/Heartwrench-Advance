#ifndef TILE_PASSAGE_H
#define TILE_PASSAGE_H

// Butano
#include "bn_math.h"

// Base Class
#include "game_object.h"

// Assets
#include "bn_sprite_items_tile_passage.h"

#define TILE_PASSAGE_Z_ORDER 10

#define TILE_PASSAGE_COLLIDER_WIDTH  32
#define TILE_PASSAGE_COLLIDER_HEIGHT 16

#define TILE_PASSAGE_30_FPS true

struct TilePassage : GameObject {

	////////////////////////
	// Struct TilePassage //
	////////////////////////

    TilePassage();
	TilePassage(const TilePassage& other);
	~TilePassage();

	TilePassage& operator =(const TilePassage& other);

	//////////////////////////
	// GameObject Overrides //
	//////////////////////////

	// None..

	//////////////////////////////
	// State Function Overrides //
	//////////////////////////////

    void setState(ObjectState new_state) override;

	/////////////////////////
	// Collision Overrides //
	/////////////////////////

	void resolveCollision(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
						  const bn::regular_bg_ptr&                      bg_ptr, 
						  const bn::span<const bn::regular_bg_map_cell>& cells,
						  const bn::regular_bg_item&                     bg_item) override;

};

#endif