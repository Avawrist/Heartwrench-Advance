#ifndef BN_SPRITE_ITEMS_SMALL_VASE_H
#define BN_SPRITE_ITEMS_SMALL_VASE_H

#include "bn_sprite_item.h"

//{{BLOCK(small_vase_bn_gfx)

//======================================================================
//
//	small_vase_bn_gfx, 64x448@4, 
//	+ palette 16 entries, not compressed
//	+ 448 tiles not compressed
//	Total size: 32 + 14336 = 14368
//
//	Time-stamp: 2025-07-13, 14:46:06
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_SMALL_VASE_BN_GFX_H
#define GRIT_SMALL_VASE_BN_GFX_H

#define small_vase_bn_gfxTilesLen 14336
extern const bn::tile small_vase_bn_gfxTiles[448];

#define small_vase_bn_gfxPalLen 32
extern const bn::color small_vase_bn_gfxPal[16];

#endif // GRIT_SMALL_VASE_BN_GFX_H

//}}BLOCK(small_vase_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item small_vase(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(small_vase_bn_gfxTiles, 448), bpp_mode::BPP_4, compression_type::NONE, 7), 
            sprite_palette_item(span<const color>(small_vase_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

