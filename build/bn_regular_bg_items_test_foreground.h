#ifndef BN_REGULAR_BG_ITEMS_TEST_FOREGROUND_H
#define BN_REGULAR_BG_ITEMS_TEST_FOREGROUND_H

#include "bn_regular_bg_item.h"

//{{BLOCK(test_foreground_bn_gfx)

//======================================================================
//
//	test_foreground_bn_gfx, 1024x512@8, 
//	+ palette 16 entries, not compressed
//	+ 1 tiles (t reduced) not compressed
//	+ regular map (flat), not compressed, 128x64 
//	Total size: 32 + 64 + 16384 = 16480
//
//	Time-stamp: 2025-04-25, 11:31:38
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TEST_FOREGROUND_BN_GFX_H
#define GRIT_TEST_FOREGROUND_BN_GFX_H

#define test_foreground_bn_gfxTilesLen 64
extern const bn::tile test_foreground_bn_gfxTiles[2];

#define test_foreground_bn_gfxMapLen 16384
extern const bn::regular_bg_map_cell test_foreground_bn_gfxMap[8192];

#define test_foreground_bn_gfxPalLen 32
extern const bn::color test_foreground_bn_gfxPal[16];

#endif // GRIT_TEST_FOREGROUND_BN_GFX_H

//}}BLOCK(test_foreground_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item test_foreground(
            regular_bg_tiles_item(span<const tile>(test_foreground_bn_gfxTiles, 2), bpp_mode::BPP_8, compression_type::NONE), 
            bg_palette_item(span<const color>(test_foreground_bn_gfxPal, 16), bpp_mode::BPP_8, compression_type::NONE),
            regular_bg_map_item(test_foreground_bn_gfxMap[0], size(128, 64), compression_type::NONE, 1, true));
}

#endif

