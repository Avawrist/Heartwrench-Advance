#ifndef BN_SPRITE_ITEMS_SCYTHE_PLATFORM_H
#define BN_SPRITE_ITEMS_SCYTHE_PLATFORM_H

#include "bn_sprite_item.h"

//{{BLOCK(scythe_platform_bn_gfx)

//======================================================================
//
//	scythe_platform_bn_gfx, 32x32@8, 
//	+ palette 16 entries, not compressed
//	+ 16 tiles not compressed
//	Total size: 32 + 1024 = 1056
//
//	Time-stamp: 2025-03-18, 14:55:53
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_SCYTHE_PLATFORM_BN_GFX_H
#define GRIT_SCYTHE_PLATFORM_BN_GFX_H

#define scythe_platform_bn_gfxTilesLen 1024
extern const bn::tile scythe_platform_bn_gfxTiles[32];

#define scythe_platform_bn_gfxPalLen 32
extern const bn::color scythe_platform_bn_gfxPal[16];

#endif // GRIT_SCYTHE_PLATFORM_BN_GFX_H

//}}BLOCK(scythe_platform_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item scythe_platform(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(scythe_platform_bn_gfxTiles, 32), bpp_mode::BPP_8, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(scythe_platform_bn_gfxPal, 16), bpp_mode::BPP_8, compression_type::NONE));
}

#endif

