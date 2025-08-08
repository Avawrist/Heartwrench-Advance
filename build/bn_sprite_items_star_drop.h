#ifndef BN_SPRITE_ITEMS_STAR_DROP_H
#define BN_SPRITE_ITEMS_STAR_DROP_H

#include "bn_sprite_item.h"

//{{BLOCK(star_drop_bn_gfx)

//======================================================================
//
//	star_drop_bn_gfx, 32x320@4, 
//	+ palette 16 entries, not compressed
//	+ 160 tiles not compressed
//	Total size: 32 + 5120 = 5152
//
//	Time-stamp: 2025-08-07, 18:47:48
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_STAR_DROP_BN_GFX_H
#define GRIT_STAR_DROP_BN_GFX_H

#define star_drop_bn_gfxTilesLen 5120
extern const bn::tile star_drop_bn_gfxTiles[160];

#define star_drop_bn_gfxPalLen 32
extern const bn::color star_drop_bn_gfxPal[16];

#endif // GRIT_STAR_DROP_BN_GFX_H

//}}BLOCK(star_drop_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item star_drop(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(star_drop_bn_gfxTiles, 160), bpp_mode::BPP_4, compression_type::NONE, 10), 
            sprite_palette_item(span<const color>(star_drop_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

