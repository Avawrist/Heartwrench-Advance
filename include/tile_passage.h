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

	void update(const RoomBounds&                              room_bounds,
				bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
				const bn::regular_bg_ptr&                      bg_ptr, 
                const bn::span<const bn::regular_bg_map_cell>& cells,
                const bn::regular_bg_item&                     bg_item,
				const bn::camera_ptr&                          camera) override;
    void setState(ObjectState new_state) override;

};

#endif