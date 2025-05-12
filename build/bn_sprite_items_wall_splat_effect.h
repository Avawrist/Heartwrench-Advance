#ifndef BN_SPRITE_ITEMS_WALL_SPLAT_EFFECT_H
#define BN_SPRITE_ITEMS_WALL_SPLAT_EFFECT_H

#include "bn_sprite_item.h"

//{{BLOCK(wall_splat_effect_bn_gfx)

//======================================================================
//
//	wall_splat_effect_bn_gfx, 64x960@8, 
//	+ palette 32 entries, not compressed
//	+ 960 tiles not compressed
//	Total size: 64 + 61440 = 61504
//
//	Time-stamp: 2025-05-13, 00:55:23
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_WALL_SPLAT_EFFECT_BN_GFX_H
#define GRIT_WALL_SPLAT_EFFECT_BN_GFX_H

#define wall_splat_effect_bn_gfxTilesLen 61440
extern const bn::tile wall_splat_effect_bn_gfxTiles[1920];

#define wall_splat_effect_bn_gfxPalLen 64
extern const bn::color wall_splat_effect_bn_gfxPal[32];

#endif // GRIT_WALL_SPLAT_EFFECT_BN_GFX_H

//}}BLOCK(wall_splat_effect_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item wall_splat_effect(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(wall_splat_effect_bn_gfxTiles, 1920), bpp_mode::BPP_8, compression_type::NONE, 15), 
            sprite_palette_item(span<const color>(wall_splat_effect_bn_gfxPal, 32), bpp_mode::BPP_8, compression_type::NONE));
}

#endif

