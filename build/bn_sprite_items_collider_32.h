#ifndef BN_SPRITE_ITEMS_COLLIDER_32_H
#define BN_SPRITE_ITEMS_COLLIDER_32_H

#include "bn_sprite_item.h"

//{{BLOCK(collider_32_bn_gfx)

//======================================================================
//
//	collider_32_bn_gfx, 32x32@8, 
//	+ palette 48 entries, not compressed
//	+ 16 tiles not compressed
//	Total size: 96 + 1024 = 1120
//
//	Time-stamp: 2024-11-20, 18:13:55
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_COLLIDER_32_BN_GFX_H
#define GRIT_COLLIDER_32_BN_GFX_H

#define collider_32_bn_gfxTilesLen 1024
extern const bn::tile collider_32_bn_gfxTiles[32];

#define collider_32_bn_gfxPalLen 96
extern const bn::color collider_32_bn_gfxPal[48];

#endif // GRIT_COLLIDER_32_BN_GFX_H

//}}BLOCK(collider_32_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item collider_32(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(collider_32_bn_gfxTiles, 32), bpp_mode::BPP_8, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(collider_32_bn_gfxPal, 48), bpp_mode::BPP_8, compression_type::NONE));
}

#endif

