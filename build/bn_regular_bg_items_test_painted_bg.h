#ifndef BN_REGULAR_BG_ITEMS_TEST_PAINTED_BG_H
#define BN_REGULAR_BG_ITEMS_TEST_PAINTED_BG_H

#include "bn_regular_bg_item.h"

//{{BLOCK(test_painted_bg_bn_gfx)

//======================================================================
//
//	test_painted_bg_bn_gfx, 1280x768@8, 
//	+ palette 64 entries, not compressed
//	+ 73 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 160x96 
//	Total size: 128 + 4672 + 30720 = 35520
//
//	Time-stamp: 2025-07-14, 22:19:08
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TEST_PAINTED_BG_BN_GFX_H
#define GRIT_TEST_PAINTED_BG_BN_GFX_H

#define test_painted_bg_bn_gfxTilesLen 4672
extern const bn::tile test_painted_bg_bn_gfxTiles[146];

#define test_painted_bg_bn_gfxMapLen 30720
extern const bn::regular_bg_map_cell test_painted_bg_bn_gfxMap[15360];

#define test_painted_bg_bn_gfxPalLen 128
extern const bn::color test_painted_bg_bn_gfxPal[64];

#endif // GRIT_TEST_PAINTED_BG_BN_GFX_H

//}}BLOCK(test_painted_bg_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item test_painted_bg(
            regular_bg_tiles_item(span<const tile>(test_painted_bg_bn_gfxTiles, 146), bpp_mode::BPP_8, compression_type::NONE), 
            bg_palette_item(span<const color>(test_painted_bg_bn_gfxPal, 64), bpp_mode::BPP_8, compression_type::NONE),
            regular_bg_map_item(test_painted_bg_bn_gfxMap[0], size(160, 96), compression_type::NONE, 1, true));
}

#endif

