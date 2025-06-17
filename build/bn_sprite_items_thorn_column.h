#ifndef BN_SPRITE_ITEMS_THORN_COLUMN_H
#define BN_SPRITE_ITEMS_THORN_COLUMN_H

#include "bn_sprite_item.h"

//{{BLOCK(thorn_column_bn_gfx)

//======================================================================
//
//	thorn_column_bn_gfx, 64x320@4, 
//	+ palette 16 entries, not compressed
//	+ 320 tiles not compressed
//	Total size: 32 + 10240 = 10272
//
//	Time-stamp: 2025-06-17, 14:30:04
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_THORN_COLUMN_BN_GFX_H
#define GRIT_THORN_COLUMN_BN_GFX_H

#define thorn_column_bn_gfxTilesLen 10240
extern const bn::tile thorn_column_bn_gfxTiles[320];

#define thorn_column_bn_gfxPalLen 32
extern const bn::color thorn_column_bn_gfxPal[16];

#endif // GRIT_THORN_COLUMN_BN_GFX_H

//}}BLOCK(thorn_column_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item thorn_column(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(thorn_column_bn_gfxTiles, 320), bpp_mode::BPP_4, compression_type::NONE, 5), 
            sprite_palette_item(span<const color>(thorn_column_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

