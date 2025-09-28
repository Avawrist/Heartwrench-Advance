#ifndef BN_SPRITE_ITEMS_CURRENCY_NUMBER_H
#define BN_SPRITE_ITEMS_CURRENCY_NUMBER_H

#include "bn_sprite_item.h"

//{{BLOCK(currency_number_bn_gfx)

//======================================================================
//
//	currency_number_bn_gfx, 16x176@4, 
//	+ palette 16 entries, not compressed
//	+ 44 tiles not compressed
//	Total size: 32 + 1408 = 1440
//
//	Time-stamp: 2025-09-28, 14:35:41
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_CURRENCY_NUMBER_BN_GFX_H
#define GRIT_CURRENCY_NUMBER_BN_GFX_H

#define currency_number_bn_gfxTilesLen 1408
extern const bn::tile currency_number_bn_gfxTiles[44];

#define currency_number_bn_gfxPalLen 32
extern const bn::color currency_number_bn_gfxPal[16];

#endif // GRIT_CURRENCY_NUMBER_BN_GFX_H

//}}BLOCK(currency_number_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item currency_number(sprite_shape_size(sprite_shape::SQUARE, sprite_size::NORMAL), 
            sprite_tiles_item(span<const tile>(currency_number_bn_gfxTiles, 44), bpp_mode::BPP_4, compression_type::NONE, 11), 
            sprite_palette_item(span<const color>(currency_number_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

