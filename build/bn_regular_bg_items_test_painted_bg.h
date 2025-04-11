#ifndef BN_REGULAR_BG_ITEMS_TEST_PAINTED_BG_H
#define BN_REGULAR_BG_ITEMS_TEST_PAINTED_BG_H

#include "bn_regular_bg_item.h"

//{{BLOCK(test_painted_bg_bn_gfx)

//======================================================================
//
//	test_painted_bg_bn_gfx, 1024x512@8, 
//	+ palette 80 entries, not compressed
//	+ 37 tiles (t reduced) not compressed
//	+ regular map (flat), not compressed, 128x64 
//	Total size: 160 + 2368 + 16384 = 18912
//
//	Time-stamp: 2025-04-11, 12:26:15
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TEST_PAINTED_BG_BN_GFX_H
#define GRIT_TEST_PAINTED_BG_BN_GFX_H

#define test_painted_bg_bn_gfxTilesLen 2368
extern const bn::tile test_painted_bg_bn_gfxTiles[74];

#define test_painted_bg_bn_gfxMapLen 16384
extern const bn::regular_bg_map_cell test_painted_bg_bn_gfxMap[8192];

#define test_painted_bg_bn_gfxPalLen 160
extern const bn::color test_painted_bg_bn_gfxPal[80];

#endif // GRIT_TEST_PAINTED_BG_BN_GFX_H

//}}BLOCK(test_painted_bg_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item test_painted_bg(
            regular_bg_tiles_item(span<const tile>(test_painted_bg_bn_gfxTiles, 74), bpp_mode::BPP_8, compression_type::NONE), 
            bg_palette_item(span<const color>(test_painted_bg_bn_gfxPal, 80), bpp_mode::BPP_8, compression_type::NONE),
            regular_bg_map_item(test_painted_bg_bn_gfxMap[0], size(128, 64), compression_type::NONE, 1, true));
}

#endif

