#ifndef BN_REGULAR_BG_TILES_ITEMS_BLOCK_H
#define BN_REGULAR_BG_TILES_ITEMS_BLOCK_H

#include "bn_regular_bg_tiles_item.h"

//{{BLOCK(block_bn_gfx)

//======================================================================
//
//	block_bn_gfx, 16x16@8, 
//	+ 4 tiles not compressed
//	Total size: 256 = 256
//
//	Time-stamp: 2024-11-04, 15:08:05
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BLOCK_BN_GFX_H
#define GRIT_BLOCK_BN_GFX_H

#define block_bn_gfxTilesLen 256
extern const bn::tile block_bn_gfxTiles[8];

#endif // GRIT_BLOCK_BN_GFX_H

//}}BLOCK(block_bn_gfx)

namespace bn::regular_bg_tiles_items
{
    constexpr inline regular_bg_tiles_item block(
            span<const tile>(block_bn_gfxTiles, 8), bpp_mode::BPP_8, compression_type::NONE);
}

#endif

