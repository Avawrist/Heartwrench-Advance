#ifndef BN_SPRITE_ITEMS_TILE_PASSAGE_H
#define BN_SPRITE_ITEMS_TILE_PASSAGE_H

#include "bn_sprite_item.h"

//{{BLOCK(tile_passage_bn_gfx)

//======================================================================
//
//	tile_passage_bn_gfx, 32x64@4, 
//	+ palette 16 entries, not compressed
//	+ 32 tiles not compressed
//	Total size: 32 + 1024 = 1056
//
//	Time-stamp: 2025-05-04, 23:13:05
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TILE_PASSAGE_BN_GFX_H
#define GRIT_TILE_PASSAGE_BN_GFX_H

#define tile_passage_bn_gfxTilesLen 1024
extern const bn::tile tile_passage_bn_gfxTiles[32];

#define tile_passage_bn_gfxPalLen 32
extern const bn::color tile_passage_bn_gfxPal[16];

#endif // GRIT_TILE_PASSAGE_BN_GFX_H

//}}BLOCK(tile_passage_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item tile_passage(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(tile_passage_bn_gfxTiles, 32), bpp_mode::BPP_4, compression_type::NONE, 2), 
            sprite_palette_item(span<const color>(tile_passage_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

