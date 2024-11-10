#ifndef BN_REGULAR_BG_ITEMS_TEST_BG_H
#define BN_REGULAR_BG_ITEMS_TEST_BG_H

#include "bn_regular_bg_item.h"

//{{BLOCK(test_bg_bn_gfx)

//======================================================================
//
//	test_bg_bn_gfx, 512x512@8, 
//	+ palette 48 entries, not compressed
//	+ 2 tiles (t|f reduced) not compressed
//	+ regular map (in SBBs), not compressed, 64x64 
//	Total size: 96 + 128 + 8192 = 8416
//
//	Time-stamp: 2024-11-09, 15:44:50
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TEST_BG_BN_GFX_H
#define GRIT_TEST_BG_BN_GFX_H

#define test_bg_bn_gfxTilesLen 128
extern const bn::tile test_bg_bn_gfxTiles[4];

#define test_bg_bn_gfxMapLen 8192
extern const bn::regular_bg_map_cell test_bg_bn_gfxMap[4096];

#define test_bg_bn_gfxPalLen 96
extern const bn::color test_bg_bn_gfxPal[48];

#endif // GRIT_TEST_BG_BN_GFX_H

//}}BLOCK(test_bg_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item test_bg(
            regular_bg_tiles_item(span<const tile>(test_bg_bn_gfxTiles, 4), bpp_mode::BPP_8, compression_type::NONE), 
            bg_palette_item(span<const color>(test_bg_bn_gfxPal, 48), bpp_mode::BPP_8, compression_type::NONE),
            regular_bg_map_item(test_bg_bn_gfxMap[0], size(64, 64), compression_type::NONE, 1, false));
}

#endif

