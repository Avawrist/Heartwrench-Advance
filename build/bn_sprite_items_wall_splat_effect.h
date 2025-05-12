#ifndef BN_SPRITE_ITEMS_WALL_SPLAT_EFFECT_H
#define BN_SPRITE_ITEMS_WALL_SPLAT_EFFECT_H

#include "bn_sprite_item.h"

//{{BLOCK(wall_splat_effect_bn_gfx)

//======================================================================
//
//	wall_splat_effect_bn_gfx, 32x352@8, 
//	+ palette 16 entries, not compressed
//	+ 176 tiles not compressed
//	Total size: 32 + 11264 = 11296
//
//	Time-stamp: 2025-05-12, 19:24:45
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_WALL_SPLAT_EFFECT_BN_GFX_H
#define GRIT_WALL_SPLAT_EFFECT_BN_GFX_H

#define wall_splat_effect_bn_gfxTilesLen 11264
extern const bn::tile wall_splat_effect_bn_gfxTiles[352];

#define wall_splat_effect_bn_gfxPalLen 32
extern const bn::color wall_splat_effect_bn_gfxPal[16];

#endif // GRIT_WALL_SPLAT_EFFECT_BN_GFX_H

//}}BLOCK(wall_splat_effect_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item wall_splat_effect(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(wall_splat_effect_bn_gfxTiles, 352), bpp_mode::BPP_8, compression_type::NONE, 11), 
            sprite_palette_item(span<const color>(wall_splat_effect_bn_gfxPal, 16), bpp_mode::BPP_8, compression_type::NONE));
}

#endif

