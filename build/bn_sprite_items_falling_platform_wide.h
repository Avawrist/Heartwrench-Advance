#ifndef BN_SPRITE_ITEMS_FALLING_PLATFORM_WIDE_H
#define BN_SPRITE_ITEMS_FALLING_PLATFORM_WIDE_H

#include "bn_sprite_item.h"

//{{BLOCK(falling_platform_wide_bn_gfx)

//======================================================================
//
//	falling_platform_wide_bn_gfx, 64x512@4, 
//	+ palette 16 entries, not compressed
//	+ 512 tiles not compressed
//	Total size: 32 + 16384 = 16416
//
//	Time-stamp: 2025-06-13, 23:34:27
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_FALLING_PLATFORM_WIDE_BN_GFX_H
#define GRIT_FALLING_PLATFORM_WIDE_BN_GFX_H

#define falling_platform_wide_bn_gfxTilesLen 16384
extern const bn::tile falling_platform_wide_bn_gfxTiles[512];

#define falling_platform_wide_bn_gfxPalLen 32
extern const bn::color falling_platform_wide_bn_gfxPal[16];

#endif // GRIT_FALLING_PLATFORM_WIDE_BN_GFX_H

//}}BLOCK(falling_platform_wide_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item falling_platform_wide(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(falling_platform_wide_bn_gfxTiles, 512), bpp_mode::BPP_4, compression_type::NONE, 8), 
            sprite_palette_item(span<const color>(falling_platform_wide_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

