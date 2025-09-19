#ifndef BN_SPRITE_ITEMS_OLD_MOON_DROP_H
#define BN_SPRITE_ITEMS_OLD_MOON_DROP_H

#include "bn_sprite_item.h"

//{{BLOCK(old_moon_drop_bn_gfx)

//======================================================================
//
//	old_moon_drop_bn_gfx, 32x416@4, 
//	+ palette 16 entries, not compressed
//	+ 208 tiles not compressed
//	Total size: 32 + 6656 = 6688
//
//	Time-stamp: 2025-09-18, 16:16:32
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_OLD_MOON_DROP_BN_GFX_H
#define GRIT_OLD_MOON_DROP_BN_GFX_H

#define old_moon_drop_bn_gfxTilesLen 6656
extern const bn::tile old_moon_drop_bn_gfxTiles[208];

#define old_moon_drop_bn_gfxPalLen 32
extern const bn::color old_moon_drop_bn_gfxPal[16];

#endif // GRIT_OLD_MOON_DROP_BN_GFX_H

//}}BLOCK(old_moon_drop_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item old_moon_drop(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(old_moon_drop_bn_gfxTiles, 208), bpp_mode::BPP_4, compression_type::NONE, 13), 
            sprite_palette_item(span<const color>(old_moon_drop_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

