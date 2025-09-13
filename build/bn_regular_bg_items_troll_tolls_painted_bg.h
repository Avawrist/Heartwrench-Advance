#ifndef BN_REGULAR_BG_ITEMS_TROLL_TOLLS_PAINTED_BG_H
#define BN_REGULAR_BG_ITEMS_TROLL_TOLLS_PAINTED_BG_H

#include "bn_regular_bg_item.h"

//{{BLOCK(troll_tolls_painted_bg_bn_gfx)

//======================================================================
//
//	troll_tolls_painted_bg_bn_gfx, 512x1536@4, 
//	+ palette 16 entries, not compressed
//	+ 92 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 64x192 
//	Total size: 32 + 2944 + 24576 = 27552
//
//	Time-stamp: 2025-09-13, 02:28:23
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TROLL_TOLLS_PAINTED_BG_BN_GFX_H
#define GRIT_TROLL_TOLLS_PAINTED_BG_BN_GFX_H

#define troll_tolls_painted_bg_bn_gfxTilesLen 2944
extern const bn::tile troll_tolls_painted_bg_bn_gfxTiles[92];

#define troll_tolls_painted_bg_bn_gfxMapLen 24576
extern const bn::regular_bg_map_cell troll_tolls_painted_bg_bn_gfxMap[12288];

#define troll_tolls_painted_bg_bn_gfxPalLen 32
extern const bn::color troll_tolls_painted_bg_bn_gfxPal[16];

#endif // GRIT_TROLL_TOLLS_PAINTED_BG_BN_GFX_H

//}}BLOCK(troll_tolls_painted_bg_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item troll_tolls_painted_bg(
            regular_bg_tiles_item(span<const tile>(troll_tolls_painted_bg_bn_gfxTiles, 92), bpp_mode::BPP_4, compression_type::NONE), 
            bg_palette_item(span<const color>(troll_tolls_painted_bg_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE),
            regular_bg_map_item(troll_tolls_painted_bg_bn_gfxMap[0], size(64, 64), compression_type::NONE, 3, false));
}

#endif

