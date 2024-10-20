#ifndef BN_SPRITE_ITEMS_COLLIDER_16_H
#define BN_SPRITE_ITEMS_COLLIDER_16_H

#include "bn_sprite_item.h"

//{{BLOCK(collider_16_bn_gfx)

//======================================================================
//
//	collider_16_bn_gfx, 16x16@8, 
//	+ palette 48 entries, not compressed
//	+ 4 tiles not compressed
//	Total size: 96 + 256 = 352
//
//	Time-stamp: 2024-10-18, 20:33:10
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_COLLIDER_16_BN_GFX_H
#define GRIT_COLLIDER_16_BN_GFX_H

#define collider_16_bn_gfxTilesLen 256
extern const bn::tile collider_16_bn_gfxTiles[8];

#define collider_16_bn_gfxPalLen 96
extern const bn::color collider_16_bn_gfxPal[48];

#endif // GRIT_COLLIDER_16_BN_GFX_H

//}}BLOCK(collider_16_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item collider_16(sprite_shape_size(sprite_shape::SQUARE, sprite_size::NORMAL), 
            sprite_tiles_item(span<const tile>(collider_16_bn_gfxTiles, 8), bpp_mode::BPP_8, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(collider_16_bn_gfxPal, 48), bpp_mode::BPP_8, compression_type::NONE));
}

#endif

