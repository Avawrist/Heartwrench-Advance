#ifndef BN_SPRITE_ITEMS_WALL_SPLAT_EFFECT_H
#define BN_SPRITE_ITEMS_WALL_SPLAT_EFFECT_H

#include "bn_sprite_item.h"

//{{BLOCK(wall_splat_effect_bn_gfx)

//======================================================================
//
//	wall_splat_effect_bn_gfx, 64x768@4, 
//	+ palette 16 entries, not compressed
//	+ 768 tiles not compressed
//	Total size: 32 + 24576 = 24608
//
//	Time-stamp: 2025-07-07, 15:29:58
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_WALL_SPLAT_EFFECT_BN_GFX_H
#define GRIT_WALL_SPLAT_EFFECT_BN_GFX_H

#define wall_splat_effect_bn_gfxTilesLen 24576
extern const bn::tile wall_splat_effect_bn_gfxTiles[768];

#define wall_splat_effect_bn_gfxPalLen 32
extern const bn::color wall_splat_effect_bn_gfxPal[16];

#endif // GRIT_WALL_SPLAT_EFFECT_BN_GFX_H

//}}BLOCK(wall_splat_effect_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item wall_splat_effect(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(wall_splat_effect_bn_gfxTiles, 768), bpp_mode::BPP_4, compression_type::NONE, 12), 
            sprite_palette_item(span<const color>(wall_splat_effect_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

