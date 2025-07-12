#ifndef BN_SPRITE_ITEMS_LAND_EFFECT_H
#define BN_SPRITE_ITEMS_LAND_EFFECT_H

#include "bn_sprite_item.h"

//{{BLOCK(land_effect_bn_gfx)

//======================================================================
//
//	land_effect_bn_gfx, 64x448@4, 
//	+ palette 16 entries, not compressed
//	+ 448 tiles not compressed
//	Total size: 32 + 14336 = 14368
//
//	Time-stamp: 2025-07-11, 19:00:00
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_LAND_EFFECT_BN_GFX_H
#define GRIT_LAND_EFFECT_BN_GFX_H

#define land_effect_bn_gfxTilesLen 14336
extern const bn::tile land_effect_bn_gfxTiles[448];

#define land_effect_bn_gfxPalLen 32
extern const bn::color land_effect_bn_gfxPal[16];

#endif // GRIT_LAND_EFFECT_BN_GFX_H

//}}BLOCK(land_effect_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item land_effect(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(land_effect_bn_gfxTiles, 448), bpp_mode::BPP_4, compression_type::NONE, 7), 
            sprite_palette_item(span<const color>(land_effect_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

