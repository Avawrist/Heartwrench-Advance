#ifndef BN_REGULAR_BG_ITEMS_TEST_LEVEL_H
#define BN_REGULAR_BG_ITEMS_TEST_LEVEL_H

#include "bn_regular_bg_item.h"

//{{BLOCK(test_level_bn_gfx)

//======================================================================
//
//	test_level_bn_gfx, 10240x5120@4, 
//	+ palette 16 entries, not compressed
//	+ 246 tiles (t|p reduced) not compressed
//	+ regular map (flat), not compressed, 1280x640 
//	Total size: 32 + 7872 + 1638400 = 1646304
//
//	Time-stamp: 2025-06-29, 12:45:09
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TEST_LEVEL_BN_GFX_H
#define GRIT_TEST_LEVEL_BN_GFX_H

#define test_level_bn_gfxTilesLen 7872
extern const bn::tile test_level_bn_gfxTiles[246];

#define test_level_bn_gfxMapLen 1638400
extern const bn::regular_bg_map_cell test_level_bn_gfxMap[819200];

#define test_level_bn_gfxPalLen 32
extern const bn::color test_level_bn_gfxPal[16];

#endif // GRIT_TEST_LEVEL_BN_GFX_H

//}}BLOCK(test_level_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item test_level(
            regular_bg_tiles_item(span<const tile>(test_level_bn_gfxTiles, 246), bpp_mode::BPP_4, compression_type::NONE), 
            bg_palette_item(span<const color>(test_level_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE),
            regular_bg_map_item(test_level_bn_gfxMap[0], size(1280, 640), compression_type::NONE, 1, true));
}

#endif

