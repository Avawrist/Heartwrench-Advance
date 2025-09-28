#ifndef BN_REGULAR_BG_ITEMS_LEVEL_1_PAINTED_BG_H
#define BN_REGULAR_BG_ITEMS_LEVEL_1_PAINTED_BG_H

#include "bn_regular_bg_item.h"

//{{BLOCK(level_1_painted_bg_bn_gfx)

//======================================================================
//
//	level_1_painted_bg_bn_gfx, 512x1536@4, 
//	+ palette 16 entries, not compressed
//	+ 111 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 64x192 
//	Total size: 32 + 3552 + 24576 = 28160
//
//	Time-stamp: 2025-09-28, 14:35:41
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_LEVEL_1_PAINTED_BG_BN_GFX_H
#define GRIT_LEVEL_1_PAINTED_BG_BN_GFX_H

#define level_1_painted_bg_bn_gfxTilesLen 3552
extern const bn::tile level_1_painted_bg_bn_gfxTiles[111];

#define level_1_painted_bg_bn_gfxMapLen 24576
extern const bn::regular_bg_map_cell level_1_painted_bg_bn_gfxMap[12288];

#define level_1_painted_bg_bn_gfxPalLen 32
extern const bn::color level_1_painted_bg_bn_gfxPal[16];

#endif // GRIT_LEVEL_1_PAINTED_BG_BN_GFX_H

//}}BLOCK(level_1_painted_bg_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item level_1_painted_bg(
            regular_bg_tiles_item(span<const tile>(level_1_painted_bg_bn_gfxTiles, 111), bpp_mode::BPP_4, compression_type::NONE), 
            bg_palette_item(span<const color>(level_1_painted_bg_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE),
            regular_bg_map_item(level_1_painted_bg_bn_gfxMap[0], size(64, 64), compression_type::NONE, 3, false));
}

#endif

