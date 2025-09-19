#ifndef BN_SPRITE_ITEMS_PUSH_BLOCK_MINI_H
#define BN_SPRITE_ITEMS_PUSH_BLOCK_MINI_H

#include "bn_sprite_item.h"

//{{BLOCK(push_block_mini_bn_gfx)

//======================================================================
//
//	push_block_mini_bn_gfx, 32x32@4, 
//	+ palette 16 entries, not compressed
//	+ 16 tiles not compressed
//	Total size: 32 + 512 = 544
//
//	Time-stamp: 2025-09-18, 16:16:32
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PUSH_BLOCK_MINI_BN_GFX_H
#define GRIT_PUSH_BLOCK_MINI_BN_GFX_H

#define push_block_mini_bn_gfxTilesLen 512
extern const bn::tile push_block_mini_bn_gfxTiles[16];

#define push_block_mini_bn_gfxPalLen 32
extern const bn::color push_block_mini_bn_gfxPal[16];

#endif // GRIT_PUSH_BLOCK_MINI_BN_GFX_H

//}}BLOCK(push_block_mini_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item push_block_mini(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(push_block_mini_bn_gfxTiles, 16), bpp_mode::BPP_4, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(push_block_mini_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

