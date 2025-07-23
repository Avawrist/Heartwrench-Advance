#ifndef BN_REGULAR_BG_ITEMS_ZIGGURAT_1_LEVEL_BG_H
#define BN_REGULAR_BG_ITEMS_ZIGGURAT_1_LEVEL_BG_H

#include "bn_regular_bg_item.h"

//{{BLOCK(ziggurat_1_level_bg_bn_gfx)

//======================================================================
//
//	ziggurat_1_level_bg_bn_gfx, 10240x5120@4, 
//	+ palette 16 entries, not compressed
//	+ 237 tiles (t|p reduced) not compressed
//	+ regular map (flat), not compressed, 1280x640 
//	Total size: 32 + 7584 + 1638400 = 1646016
//
//	Time-stamp: 2025-07-23, 12:48:34
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_ZIGGURAT_1_LEVEL_BG_BN_GFX_H
#define GRIT_ZIGGURAT_1_LEVEL_BG_BN_GFX_H

#define ziggurat_1_level_bg_bn_gfxTilesLen 7584
extern const bn::tile ziggurat_1_level_bg_bn_gfxTiles[237];

#define ziggurat_1_level_bg_bn_gfxMapLen 1638400
extern const bn::regular_bg_map_cell ziggurat_1_level_bg_bn_gfxMap[819200];

#define ziggurat_1_level_bg_bn_gfxPalLen 32
extern const bn::color ziggurat_1_level_bg_bn_gfxPal[16];

#endif // GRIT_ZIGGURAT_1_LEVEL_BG_BN_GFX_H

//}}BLOCK(ziggurat_1_level_bg_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item ziggurat_1_level_bg(
            regular_bg_tiles_item(span<const tile>(ziggurat_1_level_bg_bn_gfxTiles, 237), bpp_mode::BPP_4, compression_type::NONE), 
            bg_palette_item(span<const color>(ziggurat_1_level_bg_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE),
            regular_bg_map_item(ziggurat_1_level_bg_bn_gfxMap[0], size(1280, 640), compression_type::NONE, 1, true));
}

#endif

