#ifndef BN_SPRITE_ITEMS_ITEM_GET_H
#define BN_SPRITE_ITEMS_ITEM_GET_H

#include "bn_sprite_item.h"

//{{BLOCK(item_get_bn_gfx)

//======================================================================
//
//	item_get_bn_gfx, 32x192@4, 
//	+ palette 16 entries, not compressed
//	+ 96 tiles not compressed
//	Total size: 32 + 3072 = 3104
//
//	Time-stamp: 2025-07-19, 13:42:34
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_ITEM_GET_BN_GFX_H
#define GRIT_ITEM_GET_BN_GFX_H

#define item_get_bn_gfxTilesLen 3072
extern const bn::tile item_get_bn_gfxTiles[96];

#define item_get_bn_gfxPalLen 32
extern const bn::color item_get_bn_gfxPal[16];

#endif // GRIT_ITEM_GET_BN_GFX_H

//}}BLOCK(item_get_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item item_get(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(item_get_bn_gfxTiles, 96), bpp_mode::BPP_4, compression_type::NONE, 6), 
            sprite_palette_item(span<const color>(item_get_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

