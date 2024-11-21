#ifndef BN_SPRITE_ITEMS_COLLIDER_8_H
#define BN_SPRITE_ITEMS_COLLIDER_8_H

#include "bn_sprite_item.h"

//{{BLOCK(collider_8_bn_gfx)

//======================================================================
//
//	collider_8_bn_gfx, 8x8@8, 
//	+ palette 48 entries, not compressed
//	+ 1 tiles not compressed
//	Total size: 96 + 64 = 160
//
//	Time-stamp: 2024-11-20, 18:13:55
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_COLLIDER_8_BN_GFX_H
#define GRIT_COLLIDER_8_BN_GFX_H

#define collider_8_bn_gfxTilesLen 64
extern const bn::tile collider_8_bn_gfxTiles[2];

#define collider_8_bn_gfxPalLen 96
extern const bn::color collider_8_bn_gfxPal[48];

#endif // GRIT_COLLIDER_8_BN_GFX_H

//}}BLOCK(collider_8_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item collider_8(sprite_shape_size(sprite_shape::SQUARE, sprite_size::SMALL), 
            sprite_tiles_item(span<const tile>(collider_8_bn_gfxTiles, 2), bpp_mode::BPP_8, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(collider_8_bn_gfxPal, 48), bpp_mode::BPP_8, compression_type::NONE));
}

#endif

