#ifndef BN_SPRITE_ITEMS_PUSH_BLOCK_H
#define BN_SPRITE_ITEMS_PUSH_BLOCK_H

#include "bn_sprite_item.h"

//{{BLOCK(push_block_bn_gfx)

//======================================================================
//
//	push_block_bn_gfx, 64x128@4, 
//	+ palette 16 entries, not compressed
//	+ 128 tiles not compressed
//	Total size: 32 + 4096 = 4128
//
//	Time-stamp: 2025-06-25, 18:39:57
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PUSH_BLOCK_BN_GFX_H
#define GRIT_PUSH_BLOCK_BN_GFX_H

#define push_block_bn_gfxTilesLen 4096
extern const bn::tile push_block_bn_gfxTiles[128];

#define push_block_bn_gfxPalLen 32
extern const bn::color push_block_bn_gfxPal[16];

#endif // GRIT_PUSH_BLOCK_BN_GFX_H

//}}BLOCK(push_block_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item push_block(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(push_block_bn_gfxTiles, 128), bpp_mode::BPP_4, compression_type::NONE, 2), 
            sprite_palette_item(span<const color>(push_block_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

