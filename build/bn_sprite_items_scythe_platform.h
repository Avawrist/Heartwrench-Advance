#ifndef BN_SPRITE_ITEMS_SCYTHE_PLATFORM_H
#define BN_SPRITE_ITEMS_SCYTHE_PLATFORM_H

#include "bn_sprite_item.h"

//{{BLOCK(scythe_platform_bn_gfx)

//======================================================================
//
//	scythe_platform_bn_gfx, 32x32@8, 
//	+ palette 64 entries, not compressed
//	+ 16 tiles not compressed
//	Total size: 128 + 1024 = 1152
//
//	Time-stamp: 2024-11-22, 16:06:11
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_SCYTHE_PLATFORM_BN_GFX_H
#define GRIT_SCYTHE_PLATFORM_BN_GFX_H

#define scythe_platform_bn_gfxTilesLen 1024
extern const bn::tile scythe_platform_bn_gfxTiles[32];

#define scythe_platform_bn_gfxPalLen 128
extern const bn::color scythe_platform_bn_gfxPal[64];

#endif // GRIT_SCYTHE_PLATFORM_BN_GFX_H

//}}BLOCK(scythe_platform_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item scythe_platform(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(scythe_platform_bn_gfxTiles, 32), bpp_mode::BPP_8, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(scythe_platform_bn_gfxPal, 64), bpp_mode::BPP_8, compression_type::NONE));
}

#endif

