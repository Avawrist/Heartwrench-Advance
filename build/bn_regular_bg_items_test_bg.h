#ifndef BN_REGULAR_BG_ITEMS_TEST_BG_H
#define BN_REGULAR_BG_ITEMS_TEST_BG_H

#include "bn_regular_bg_item.h"

//{{BLOCK(test_bg_bn_gfx)

//======================================================================
//
//	test_bg_bn_gfx, 10240x5120@8, 
//	+ palette 80 entries, not compressed
//	+ 147 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 1280x640 
//	Total size: 160 + 9408 + 1638400 = 1647968
//
//	Time-stamp: 2025-04-12, 15:32:32
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TEST_BG_BN_GFX_H
#define GRIT_TEST_BG_BN_GFX_H

#define test_bg_bn_gfxTilesLen 9408
extern const bn::tile test_bg_bn_gfxTiles[294];

#define test_bg_bn_gfxMapLen 1638400
extern const bn::regular_bg_map_cell test_bg_bn_gfxMap[819200];

#define test_bg_bn_gfxPalLen 160
extern const bn::color test_bg_bn_gfxPal[80];

#endif // GRIT_TEST_BG_BN_GFX_H

//}}BLOCK(test_bg_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item test_bg(
            regular_bg_tiles_item(span<const tile>(test_bg_bn_gfxTiles, 294), bpp_mode::BPP_8, compression_type::NONE), 
            bg_palette_item(span<const color>(test_bg_bn_gfxPal, 80), bpp_mode::BPP_8, compression_type::NONE),
            regular_bg_map_item(test_bg_bn_gfxMap[0], size(1280, 640), compression_type::NONE, 1, true));
}

#endif

