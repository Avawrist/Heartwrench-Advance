#ifndef BN_SPRITE_ITEMS_SMALL_VASE_H
#define BN_SPRITE_ITEMS_SMALL_VASE_H

#include "bn_sprite_item.h"

//{{BLOCK(small_vase_bn_gfx)

//======================================================================
//
//	small_vase_bn_gfx, 64x384@4, 
//	+ palette 16 entries, not compressed
//	+ 384 tiles not compressed
//	Total size: 32 + 12288 = 12320
//
//	Time-stamp: 2025-08-01, 20:37:05
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_SMALL_VASE_BN_GFX_H
#define GRIT_SMALL_VASE_BN_GFX_H

#define small_vase_bn_gfxTilesLen 12288
extern const bn::tile small_vase_bn_gfxTiles[384];

#define small_vase_bn_gfxPalLen 32
extern const bn::color small_vase_bn_gfxPal[16];

#endif // GRIT_SMALL_VASE_BN_GFX_H

//}}BLOCK(small_vase_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item small_vase(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(small_vase_bn_gfxTiles, 384), bpp_mode::BPP_4, compression_type::NONE, 6), 
            sprite_palette_item(span<const color>(small_vase_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

