#ifndef BN_SPRITE_ITEMS_WALL_JUMP_EFFECT_H
#define BN_SPRITE_ITEMS_WALL_JUMP_EFFECT_H

#include "bn_sprite_item.h"

//{{BLOCK(wall_jump_effect_bn_gfx)

//======================================================================
//
//	wall_jump_effect_bn_gfx, 32x160@4, 
//	+ palette 16 entries, not compressed
//	+ 80 tiles not compressed
//	Total size: 32 + 2560 = 2592
//
//	Time-stamp: 2025-08-01, 20:37:05
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_WALL_JUMP_EFFECT_BN_GFX_H
#define GRIT_WALL_JUMP_EFFECT_BN_GFX_H

#define wall_jump_effect_bn_gfxTilesLen 2560
extern const bn::tile wall_jump_effect_bn_gfxTiles[80];

#define wall_jump_effect_bn_gfxPalLen 32
extern const bn::color wall_jump_effect_bn_gfxPal[16];

#endif // GRIT_WALL_JUMP_EFFECT_BN_GFX_H

//}}BLOCK(wall_jump_effect_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item wall_jump_effect(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(wall_jump_effect_bn_gfxTiles, 80), bpp_mode::BPP_4, compression_type::NONE, 5), 
            sprite_palette_item(span<const color>(wall_jump_effect_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

