#ifndef BN_REGULAR_BG_ITEMS_TEST_ROOM_H
#define BN_REGULAR_BG_ITEMS_TEST_ROOM_H

#include "bn_regular_bg_item.h"

//{{BLOCK(test_room_bn_gfx)

//======================================================================
//
//	test_room_bn_gfx, 512x512@8, 
//	+ palette 64 entries, not compressed
//	+ 7 tiles (t|f reduced) not compressed
//	+ regular map (in SBBs), not compressed, 64x64 
//	Total size: 128 + 448 + 8192 = 8768
//
//	Time-stamp: 2024-12-08, 21:34:56
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TEST_ROOM_BN_GFX_H
#define GRIT_TEST_ROOM_BN_GFX_H

#define test_room_bn_gfxTilesLen 448
extern const bn::tile test_room_bn_gfxTiles[14];

#define test_room_bn_gfxMapLen 8192
extern const bn::regular_bg_map_cell test_room_bn_gfxMap[4096];

#define test_room_bn_gfxPalLen 128
extern const bn::color test_room_bn_gfxPal[64];

#endif // GRIT_TEST_ROOM_BN_GFX_H

//}}BLOCK(test_room_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item test_room(
            regular_bg_tiles_item(span<const tile>(test_room_bn_gfxTiles, 14), bpp_mode::BPP_8, compression_type::NONE), 
            bg_palette_item(span<const color>(test_room_bn_gfxPal, 64), bpp_mode::BPP_8, compression_type::NONE),
            regular_bg_map_item(test_room_bn_gfxMap[0], size(64, 64), compression_type::NONE, 1, false));
}

#endif

