#ifndef BN_SPRITE_ITEMS_DEVIL_PLATFORM_H
#define BN_SPRITE_ITEMS_DEVIL_PLATFORM_H

#include "bn_sprite_item.h"

//{{BLOCK(devil_platform_bn_gfx)

//======================================================================
//
//	devil_platform_bn_gfx, 32x32@4, 
//	+ palette 16 entries, not compressed
//	+ 16 tiles not compressed
//	Total size: 32 + 512 = 544
//
//	Time-stamp: 2025-05-01, 13:32:15
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_DEVIL_PLATFORM_BN_GFX_H
#define GRIT_DEVIL_PLATFORM_BN_GFX_H

#define devil_platform_bn_gfxTilesLen 512
extern const bn::tile devil_platform_bn_gfxTiles[16];

#define devil_platform_bn_gfxPalLen 32
extern const bn::color devil_platform_bn_gfxPal[16];

#endif // GRIT_DEVIL_PLATFORM_BN_GFX_H

//}}BLOCK(devil_platform_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item devil_platform(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(devil_platform_bn_gfxTiles, 16), bpp_mode::BPP_4, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(devil_platform_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

