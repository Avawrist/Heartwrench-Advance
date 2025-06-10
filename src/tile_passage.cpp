#include "tile_passage.h"

////////////////////////
// Struct TilePassage //
////////////////////////

TilePassage::TilePassage()
{
    // Init Variables //
	object_type        = TILE_PASSAGE;
    state              = TILE_PASSAGE_SHUT;
    sprite_ptr         = bn::sprite_items::tile_passage.create_sprite(0, 0);
    sprite_ptr->set_z_order(TILE_PASSAGE_Z_ORDER);
    default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  2,
								  								  bn::sprite_items::tile_passage.tiles_item(),
								  								  0, 0);

	collider          = Collider(x(), y(), TILE_PASSAGE_COLLIDER_WIDTH, TILE_PASSAGE_COLLIDER_HEIGHT);
	collider_x_axis   = collider;
	collider_y_axis   = collider;
	collider_offset_x = 0;
	collider_offset_y = 0;

}

TilePassage::TilePassage(const TilePassage& other) : GameObject(other)
{

}

TilePassage::~TilePassage()
{

}

TilePassage& TilePassage::operator =(const TilePassage& other)
{
    return *this;
}

//////////////////////////
// GameObject Overrides //
//////////////////////////

// None..

//////////////////////////////
// State Function Overrides //
//////////////////////////////

void TilePassage::setState(ObjectState new_state)
{
    // Set State
	state = new_state;

	// Set animations & other state specific variables
    switch(new_state)
	{
        case TILE_PASSAGE_OPEN:

            animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                          2,
                                                                          bn::sprite_items::tile_passage.tiles_item(),
                                                                          1, 1);
        break;

        case TILE_PASSAGE_SHUT:

            animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
                                                                          2,
                                                                          bn::sprite_items::tile_passage.tiles_item(),
                                                                          0, 0);       
        break;

        default:

            BN_LOG("Failed to set TilePassage state - State not found.");

        break;

    }

}

/////////////////////////
// Collision Overrides //
/////////////////////////

void TilePassage::resolveCollision(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                                const bn::regular_bg_ptr&                      bg_ptr, 
                                const bn::span<const bn::regular_bg_map_cell>& cells,
                                const bn::regular_bg_item&                     bg_item)
{

}