#ifndef BN_SPRITE_ITEMS_TILE_PASSAGE_SHUT_H
#define BN_SPRITE_ITEMS_TILE_PASSAGE_SHUT_H

#include "bn_sprite_item.h"

//{{BLOCK(tile_passage_shut_bn_gfx)

//======================================================================
//
//	tile_passage_shut_bn_gfx, 32x32@8, 
//	+ palette 80 entries, not compressed
//	+ 16 tiles not compressed
//	Total size: 160 + 1024 = 1184
//
//	Time-stamp: 2025-04-08, 14:59:02
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TILE_PASSAGE_SHUT_BN_GFX_H
#define GRIT_TILE_PASSAGE_SHUT_BN_GFX_H

#define tile_passage_shut_bn_gfxTilesLen 1024
extern const bn::tile tile_passage_shut_bn_gfxTiles[32];

#define tile_passage_shut_bn_gfxPalLen 160
extern const bn::color tile_passage_shut_bn_gfxPal[80];

#endif // GRIT_TILE_PASSAGE_SHUT_BN_GFX_H

//}}BLOCK(tile_passage_shut_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item tile_passage_shut(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(tile_passage_shut_bn_gfxTiles, 32), bpp_mode::BPP_8, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(tile_passage_shut_bn_gfxPal, 80), bpp_mode::BPP_8, compression_type::NONE));
}

#endif

