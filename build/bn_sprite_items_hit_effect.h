#ifndef BN_SPRITE_ITEMS_HIT_EFFECT_H
#define BN_SPRITE_ITEMS_HIT_EFFECT_H

#include "bn_sprite_item.h"

//{{BLOCK(hit_effect_bn_gfx)

//======================================================================
//
//	hit_effect_bn_gfx, 64x512@4, 
//	+ palette 16 entries, not compressed
//	+ 512 tiles not compressed
//	Total size: 32 + 16384 = 16416
//
//	Time-stamp: 2025-07-07, 15:29:58
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_HIT_EFFECT_BN_GFX_H
#define GRIT_HIT_EFFECT_BN_GFX_H

#define hit_effect_bn_gfxTilesLen 16384
extern const bn::tile hit_effect_bn_gfxTiles[512];

#define hit_effect_bn_gfxPalLen 32
extern const bn::color hit_effect_bn_gfxPal[16];

#endif // GRIT_HIT_EFFECT_BN_GFX_H

//}}BLOCK(hit_effect_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item hit_effect(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(hit_effect_bn_gfxTiles, 512), bpp_mode::BPP_4, compression_type::NONE, 8), 
            sprite_palette_item(span<const color>(hit_effect_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

