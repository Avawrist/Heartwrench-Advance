#ifndef BN_SPRITE_ITEMS_SKULL_DROP_H
#define BN_SPRITE_ITEMS_SKULL_DROP_H

#include "bn_sprite_item.h"

//{{BLOCK(skull_drop_bn_gfx)

//======================================================================
//
//	skull_drop_bn_gfx, 32x288@4, 
//	+ palette 16 entries, not compressed
//	+ 144 tiles not compressed
//	Total size: 32 + 4608 = 4640
//
//	Time-stamp: 2025-08-12, 11:11:37
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_SKULL_DROP_BN_GFX_H
#define GRIT_SKULL_DROP_BN_GFX_H

#define skull_drop_bn_gfxTilesLen 4608
extern const bn::tile skull_drop_bn_gfxTiles[144];

#define skull_drop_bn_gfxPalLen 32
extern const bn::color skull_drop_bn_gfxPal[16];

#endif // GRIT_SKULL_DROP_BN_GFX_H

//}}BLOCK(skull_drop_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item skull_drop(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(skull_drop_bn_gfxTiles, 144), bpp_mode::BPP_4, compression_type::NONE, 9), 
            sprite_palette_item(span<const color>(skull_drop_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

