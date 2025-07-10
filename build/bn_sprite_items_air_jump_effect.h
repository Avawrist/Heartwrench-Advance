#ifndef BN_SPRITE_ITEMS_AIR_JUMP_EFFECT_H
#define BN_SPRITE_ITEMS_AIR_JUMP_EFFECT_H

#include "bn_sprite_item.h"

//{{BLOCK(air_jump_effect_bn_gfx)

//======================================================================
//
//	air_jump_effect_bn_gfx, 32x224@4, 
//	+ palette 16 entries, not compressed
//	+ 112 tiles not compressed
//	Total size: 32 + 3584 = 3616
//
//	Time-stamp: 2025-07-09, 19:50:14
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_AIR_JUMP_EFFECT_BN_GFX_H
#define GRIT_AIR_JUMP_EFFECT_BN_GFX_H

#define air_jump_effect_bn_gfxTilesLen 3584
extern const bn::tile air_jump_effect_bn_gfxTiles[112];

#define air_jump_effect_bn_gfxPalLen 32
extern const bn::color air_jump_effect_bn_gfxPal[16];

#endif // GRIT_AIR_JUMP_EFFECT_BN_GFX_H

//}}BLOCK(air_jump_effect_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item air_jump_effect(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(air_jump_effect_bn_gfxTiles, 112), bpp_mode::BPP_4, compression_type::NONE, 7), 
            sprite_palette_item(span<const color>(air_jump_effect_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

