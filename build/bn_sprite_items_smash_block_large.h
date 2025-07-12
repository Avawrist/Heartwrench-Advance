#ifndef BN_SPRITE_ITEMS_SMASH_BLOCK_LARGE_H
#define BN_SPRITE_ITEMS_SMASH_BLOCK_LARGE_H

#include "bn_sprite_item.h"

//{{BLOCK(smash_block_large_bn_gfx)

//======================================================================
//
//	smash_block_large_bn_gfx, 64x384@4, 
//	+ palette 16 entries, not compressed
//	+ 384 tiles not compressed
//	Total size: 32 + 12288 = 12320
//
//	Time-stamp: 2025-07-11, 19:00:00
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_SMASH_BLOCK_LARGE_BN_GFX_H
#define GRIT_SMASH_BLOCK_LARGE_BN_GFX_H

#define smash_block_large_bn_gfxTilesLen 12288
extern const bn::tile smash_block_large_bn_gfxTiles[384];

#define smash_block_large_bn_gfxPalLen 32
extern const bn::color smash_block_large_bn_gfxPal[16];

#endif // GRIT_SMASH_BLOCK_LARGE_BN_GFX_H

//}}BLOCK(smash_block_large_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item smash_block_large(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(smash_block_large_bn_gfxTiles, 384), bpp_mode::BPP_4, compression_type::NONE, 6), 
            sprite_palette_item(span<const color>(smash_block_large_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

