#ifndef BN_SPRITE_ITEMS_FALLING_PLATFORM_THIN_H
#define BN_SPRITE_ITEMS_FALLING_PLATFORM_THIN_H

#include "bn_sprite_item.h"

//{{BLOCK(falling_platform_thin_bn_gfx)

//======================================================================
//
//	falling_platform_thin_bn_gfx, 32x256@4, 
//	+ palette 16 entries, not compressed
//	+ 128 tiles not compressed
//	Total size: 32 + 4096 = 4128
//
//	Time-stamp: 2025-06-25, 19:44:10
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_FALLING_PLATFORM_THIN_BN_GFX_H
#define GRIT_FALLING_PLATFORM_THIN_BN_GFX_H

#define falling_platform_thin_bn_gfxTilesLen 4096
extern const bn::tile falling_platform_thin_bn_gfxTiles[128];

#define falling_platform_thin_bn_gfxPalLen 32
extern const bn::color falling_platform_thin_bn_gfxPal[16];

#endif // GRIT_FALLING_PLATFORM_THIN_BN_GFX_H

//}}BLOCK(falling_platform_thin_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item falling_platform_thin(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(falling_platform_thin_bn_gfxTiles, 128), bpp_mode::BPP_4, compression_type::NONE, 8), 
            sprite_palette_item(span<const color>(falling_platform_thin_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

