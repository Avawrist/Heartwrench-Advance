#ifndef BN_SPRITE_ITEMS_LARGE_VASE_H
#define BN_SPRITE_ITEMS_LARGE_VASE_H

#include "bn_sprite_item.h"

//{{BLOCK(large_vase_bn_gfx)

//======================================================================
//
//	large_vase_bn_gfx, 64x320@4, 
//	+ palette 16 entries, not compressed
//	+ 320 tiles not compressed
//	Total size: 32 + 10240 = 10272
//
//	Time-stamp: 2025-07-13, 14:46:06
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_LARGE_VASE_BN_GFX_H
#define GRIT_LARGE_VASE_BN_GFX_H

#define large_vase_bn_gfxTilesLen 10240
extern const bn::tile large_vase_bn_gfxTiles[320];

#define large_vase_bn_gfxPalLen 32
extern const bn::color large_vase_bn_gfxPal[16];

#endif // GRIT_LARGE_VASE_BN_GFX_H

//}}BLOCK(large_vase_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item large_vase(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(large_vase_bn_gfxTiles, 320), bpp_mode::BPP_4, compression_type::NONE, 5), 
            sprite_palette_item(span<const color>(large_vase_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

