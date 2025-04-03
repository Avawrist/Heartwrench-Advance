#ifndef BN_SPRITE_ITEMS_DEVIL_PLATFORM_H
#define BN_SPRITE_ITEMS_DEVIL_PLATFORM_H

#include "bn_sprite_item.h"

//{{BLOCK(devil_platform_bn_gfx)

//======================================================================
//
//	devil_platform_bn_gfx, 32x32@8, 
//	+ palette 64 entries, not compressed
//	+ 16 tiles not compressed
//	Total size: 128 + 1024 = 1152
//
//	Time-stamp: 2025-04-02, 17:10:12
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_DEVIL_PLATFORM_BN_GFX_H
#define GRIT_DEVIL_PLATFORM_BN_GFX_H

#define devil_platform_bn_gfxTilesLen 1024
extern const bn::tile devil_platform_bn_gfxTiles[32];

#define devil_platform_bn_gfxPalLen 128
extern const bn::color devil_platform_bn_gfxPal[64];

#endif // GRIT_DEVIL_PLATFORM_BN_GFX_H

//}}BLOCK(devil_platform_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item devil_platform(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(devil_platform_bn_gfxTiles, 32), bpp_mode::BPP_8, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(devil_platform_bn_gfxPal, 64), bpp_mode::BPP_8, compression_type::NONE));
}

#endif

