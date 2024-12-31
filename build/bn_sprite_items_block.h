#ifndef BN_SPRITE_ITEMS_BLOCK_H
#define BN_SPRITE_ITEMS_BLOCK_H

#include "bn_sprite_item.h"

//{{BLOCK(block_bn_gfx)

//======================================================================
//
//	block_bn_gfx, 8x8@8, 
//	+ palette 32 entries, not compressed
//	+ 1 tiles not compressed
//	Total size: 64 + 64 = 128
//
//	Time-stamp: 2024-12-30, 23:07:11
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BLOCK_BN_GFX_H
#define GRIT_BLOCK_BN_GFX_H

#define block_bn_gfxTilesLen 64
extern const bn::tile block_bn_gfxTiles[2];

#define block_bn_gfxPalLen 64
extern const bn::color block_bn_gfxPal[32];

#endif // GRIT_BLOCK_BN_GFX_H

//}}BLOCK(block_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item block(sprite_shape_size(sprite_shape::SQUARE, sprite_size::SMALL), 
            sprite_tiles_item(span<const tile>(block_bn_gfxTiles, 2), bpp_mode::BPP_8, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(block_bn_gfxPal, 32), bpp_mode::BPP_8, compression_type::NONE));
}

#endif

