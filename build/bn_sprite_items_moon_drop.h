#ifndef BN_SPRITE_ITEMS_MOON_DROP_H
#define BN_SPRITE_ITEMS_MOON_DROP_H

#include "bn_sprite_item.h"

//{{BLOCK(moon_drop_bn_gfx)

//======================================================================
//
//	moon_drop_bn_gfx, 32x32@4, 
//	+ palette 16 entries, not compressed
//	+ 16 tiles not compressed
//	Total size: 32 + 512 = 544
//
//	Time-stamp: 2025-07-15, 15:55:03
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_MOON_DROP_BN_GFX_H
#define GRIT_MOON_DROP_BN_GFX_H

#define moon_drop_bn_gfxTilesLen 512
extern const bn::tile moon_drop_bn_gfxTiles[16];

#define moon_drop_bn_gfxPalLen 32
extern const bn::color moon_drop_bn_gfxPal[16];

#endif // GRIT_MOON_DROP_BN_GFX_H

//}}BLOCK(moon_drop_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item moon_drop(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(moon_drop_bn_gfxTiles, 16), bpp_mode::BPP_4, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(moon_drop_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

