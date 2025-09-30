#ifndef BN_SPRITE_ITEMS_ALPHABET_H
#define BN_SPRITE_ITEMS_ALPHABET_H

#include "bn_sprite_item.h"

//{{BLOCK(alphabet_bn_gfx)

//======================================================================
//
//	alphabet_bn_gfx, 16x448@4, 
//	+ palette 16 entries, not compressed
//	+ 112 tiles not compressed
//	Total size: 32 + 3584 = 3616
//
//	Time-stamp: 2025-09-30, 13:20:24
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_ALPHABET_BN_GFX_H
#define GRIT_ALPHABET_BN_GFX_H

#define alphabet_bn_gfxTilesLen 3584
extern const bn::tile alphabet_bn_gfxTiles[112];

#define alphabet_bn_gfxPalLen 32
extern const bn::color alphabet_bn_gfxPal[16];

#endif // GRIT_ALPHABET_BN_GFX_H

//}}BLOCK(alphabet_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item alphabet(sprite_shape_size(sprite_shape::SQUARE, sprite_size::NORMAL), 
            sprite_tiles_item(span<const tile>(alphabet_bn_gfxTiles, 112), bpp_mode::BPP_4, compression_type::NONE, 28), 
            sprite_palette_item(span<const color>(alphabet_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

