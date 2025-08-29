#ifndef BN_REGULAR_BG_ITEMS_ZIGGURAT_1_PAINTED_BG_H
#define BN_REGULAR_BG_ITEMS_ZIGGURAT_1_PAINTED_BG_H

#include "bn_regular_bg_item.h"

//{{BLOCK(ziggurat_1_painted_bg_bn_gfx)

//======================================================================
//
//	ziggurat_1_painted_bg_bn_gfx, 512x1536@4, 
//	+ palette 16 entries, not compressed
//	+ 229 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 64x192 
//	Total size: 32 + 7328 + 24576 = 31936
//
//	Time-stamp: 2025-08-29, 03:16:06
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_ZIGGURAT_1_PAINTED_BG_BN_GFX_H
#define GRIT_ZIGGURAT_1_PAINTED_BG_BN_GFX_H

#define ziggurat_1_painted_bg_bn_gfxTilesLen 7328
extern const bn::tile ziggurat_1_painted_bg_bn_gfxTiles[229];

#define ziggurat_1_painted_bg_bn_gfxMapLen 24576
extern const bn::regular_bg_map_cell ziggurat_1_painted_bg_bn_gfxMap[12288];

#define ziggurat_1_painted_bg_bn_gfxPalLen 32
extern const bn::color ziggurat_1_painted_bg_bn_gfxPal[16];

#endif // GRIT_ZIGGURAT_1_PAINTED_BG_BN_GFX_H

//}}BLOCK(ziggurat_1_painted_bg_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item ziggurat_1_painted_bg(
            regular_bg_tiles_item(span<const tile>(ziggurat_1_painted_bg_bn_gfxTiles, 229), bpp_mode::BPP_4, compression_type::NONE), 
            bg_palette_item(span<const color>(ziggurat_1_painted_bg_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE),
            regular_bg_map_item(ziggurat_1_painted_bg_bn_gfxMap[0], size(64, 64), compression_type::NONE, 3, false));
}

#endif

