#ifndef BN_SPRITE_ITEMS_AIR_JUMP_EFFECT_H
#define BN_SPRITE_ITEMS_AIR_JUMP_EFFECT_H

#include "bn_sprite_item.h"

//{{BLOCK(air_jump_effect_bn_gfx)

//======================================================================
//
//	air_jump_effect_bn_gfx, 64x576@4, 
//	+ palette 16 entries, not compressed
//	+ 576 tiles not compressed
//	Total size: 32 + 18432 = 18464
//
//	Time-stamp: 2025-06-30, 15:37:53
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_AIR_JUMP_EFFECT_BN_GFX_H
#define GRIT_AIR_JUMP_EFFECT_BN_GFX_H

#define air_jump_effect_bn_gfxTilesLen 18432
extern const bn::tile air_jump_effect_bn_gfxTiles[576];

#define air_jump_effect_bn_gfxPalLen 32
extern const bn::color air_jump_effect_bn_gfxPal[16];

#endif // GRIT_AIR_JUMP_EFFECT_BN_GFX_H

//}}BLOCK(air_jump_effect_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item air_jump_effect(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(air_jump_effect_bn_gfxTiles, 576), bpp_mode::BPP_4, compression_type::NONE, 9), 
            sprite_palette_item(span<const color>(air_jump_effect_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

