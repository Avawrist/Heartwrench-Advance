#ifndef BN_SPRITE_ITEMS_AUTO_PLATFORM_H
#define BN_SPRITE_ITEMS_AUTO_PLATFORM_H

#include "bn_sprite_item.h"

//{{BLOCK(auto_platform_bn_gfx)

//======================================================================
//
//	auto_platform_bn_gfx, 64x64@4, 
//	+ palette 16 entries, not compressed
//	+ 64 tiles not compressed
//	Total size: 32 + 2048 = 2080
//
//	Time-stamp: 2025-06-24, 15:06:42
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_AUTO_PLATFORM_BN_GFX_H
#define GRIT_AUTO_PLATFORM_BN_GFX_H

#define auto_platform_bn_gfxTilesLen 2048
extern const bn::tile auto_platform_bn_gfxTiles[64];

#define auto_platform_bn_gfxPalLen 32
extern const bn::color auto_platform_bn_gfxPal[16];

#endif // GRIT_AUTO_PLATFORM_BN_GFX_H

//}}BLOCK(auto_platform_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item auto_platform(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(auto_platform_bn_gfxTiles, 64), bpp_mode::BPP_4, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(auto_platform_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

