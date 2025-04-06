#ifndef BN_SPRITE_ITEMS_ANGEL_PLATFORM_H
#define BN_SPRITE_ITEMS_ANGEL_PLATFORM_H

#include "bn_sprite_item.h"

//{{BLOCK(angel_platform_bn_gfx)

//======================================================================
//
//	angel_platform_bn_gfx, 32x32@8, 
//	+ palette 64 entries, not compressed
//	+ 16 tiles not compressed
//	Total size: 128 + 1024 = 1152
//
//	Time-stamp: 2025-04-06, 15:27:43
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_ANGEL_PLATFORM_BN_GFX_H
#define GRIT_ANGEL_PLATFORM_BN_GFX_H

#define angel_platform_bn_gfxTilesLen 1024
extern const bn::tile angel_platform_bn_gfxTiles[32];

#define angel_platform_bn_gfxPalLen 128
extern const bn::color angel_platform_bn_gfxPal[64];

#endif // GRIT_ANGEL_PLATFORM_BN_GFX_H

//}}BLOCK(angel_platform_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item angel_platform(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(angel_platform_bn_gfxTiles, 32), bpp_mode::BPP_8, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(angel_platform_bn_gfxPal, 64), bpp_mode::BPP_8, compression_type::NONE));
}

#endif

