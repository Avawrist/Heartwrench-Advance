#ifndef BN_SPRITE_ITEMS_SCREW_H
#define BN_SPRITE_ITEMS_SCREW_H

#include "bn_sprite_item.h"

//{{BLOCK(screw_bn_gfx)

//======================================================================
//
//	screw_bn_gfx, 32x416@4, 
//	+ palette 16 entries, not compressed
//	+ 208 tiles not compressed
//	Total size: 32 + 6656 = 6688
//
//	Time-stamp: 2025-10-01, 00:27:09
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_SCREW_BN_GFX_H
#define GRIT_SCREW_BN_GFX_H

#define screw_bn_gfxTilesLen 6656
extern const bn::tile screw_bn_gfxTiles[208];

#define screw_bn_gfxPalLen 32
extern const bn::color screw_bn_gfxPal[16];

#endif // GRIT_SCREW_BN_GFX_H

//}}BLOCK(screw_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item screw(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(screw_bn_gfxTiles, 208), bpp_mode::BPP_4, compression_type::NONE, 13), 
            sprite_palette_item(span<const color>(screw_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

