#ifndef BN_SPRITE_ITEMS_JUMP_EFFECT_H
#define BN_SPRITE_ITEMS_JUMP_EFFECT_H

#include "bn_sprite_item.h"

//{{BLOCK(jump_effect_bn_gfx)

//======================================================================
//
//	jump_effect_bn_gfx, 64x640@4, 
//	+ palette 16 entries, not compressed
//	+ 640 tiles not compressed
//	Total size: 32 + 20480 = 20512
//
//	Time-stamp: 2025-05-29, 16:47:15
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_JUMP_EFFECT_BN_GFX_H
#define GRIT_JUMP_EFFECT_BN_GFX_H

#define jump_effect_bn_gfxTilesLen 20480
extern const bn::tile jump_effect_bn_gfxTiles[640];

#define jump_effect_bn_gfxPalLen 32
extern const bn::color jump_effect_bn_gfxPal[16];

#endif // GRIT_JUMP_EFFECT_BN_GFX_H

//}}BLOCK(jump_effect_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item jump_effect(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(jump_effect_bn_gfxTiles, 640), bpp_mode::BPP_4, compression_type::NONE, 10), 
            sprite_palette_item(span<const color>(jump_effect_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

