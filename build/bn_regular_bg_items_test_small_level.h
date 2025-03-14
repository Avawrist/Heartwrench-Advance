#ifndef BN_REGULAR_BG_ITEMS_TEST_SMALL_LEVEL_H
#define BN_REGULAR_BG_ITEMS_TEST_SMALL_LEVEL_H

#include "bn_regular_bg_item.h"

//{{BLOCK(test_small_level_bn_gfx)

//======================================================================
//
//	test_small_level_bn_gfx, 512x512@8, 
//	+ palette 64 entries, not compressed
//	+ 21 tiles (t reduced) not compressed
//	+ regular map (in SBBs), not compressed, 64x64 
//	Total size: 128 + 1344 + 8192 = 9664
//
//	Time-stamp: 2025-03-14, 15:48:48
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TEST_SMALL_LEVEL_BN_GFX_H
#define GRIT_TEST_SMALL_LEVEL_BN_GFX_H

#define test_small_level_bn_gfxTilesLen 1344
extern const bn::tile test_small_level_bn_gfxTiles[42];

#define test_small_level_bn_gfxMapLen 8192
extern const bn::regular_bg_map_cell test_small_level_bn_gfxMap[4096];

#define test_small_level_bn_gfxPalLen 128
extern const bn::color test_small_level_bn_gfxPal[64];

#endif // GRIT_TEST_SMALL_LEVEL_BN_GFX_H

//}}BLOCK(test_small_level_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item test_small_level(
            regular_bg_tiles_item(span<const tile>(test_small_level_bn_gfxTiles, 42), bpp_mode::BPP_8, compression_type::NONE), 
            bg_palette_item(span<const color>(test_small_level_bn_gfxPal, 64), bpp_mode::BPP_8, compression_type::NONE),
            regular_bg_map_item(test_small_level_bn_gfxMap[0], size(64, 64), compression_type::NONE, 1, false));
}

#endif

