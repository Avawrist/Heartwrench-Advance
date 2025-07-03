#ifndef BN_SPRITE_ITEMS_LARGE_VASE_ORANGE_H
#define BN_SPRITE_ITEMS_LARGE_VASE_ORANGE_H

#include "bn_sprite_item.h"

//{{BLOCK(large_vase_orange_bn_gfx)

//======================================================================
//
//	large_vase_orange_bn_gfx, 64x64@4, 
//	+ palette 16 entries, not compressed
//	+ 64 tiles not compressed
//	Total size: 32 + 2048 = 2080
//
//	Time-stamp: 2025-07-03, 11:30:55
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_LARGE_VASE_ORANGE_BN_GFX_H
#define GRIT_LARGE_VASE_ORANGE_BN_GFX_H

#define large_vase_orange_bn_gfxTilesLen 2048
extern const bn::tile large_vase_orange_bn_gfxTiles[64];

#define large_vase_orange_bn_gfxPalLen 32
extern const bn::color large_vase_orange_bn_gfxPal[16];

#endif // GRIT_LARGE_VASE_ORANGE_BN_GFX_H

//}}BLOCK(large_vase_orange_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item large_vase_orange(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(large_vase_orange_bn_gfxTiles, 64), bpp_mode::BPP_4, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(large_vase_orange_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

