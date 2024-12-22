#ifndef BN_REGULAR_BG_ITEMS_TEST_ROOM_2_H
#define BN_REGULAR_BG_ITEMS_TEST_ROOM_2_H

#include "bn_regular_bg_item.h"

//{{BLOCK(test_room_2_bn_gfx)

//======================================================================
//
//	test_room_2_bn_gfx, 2048x512@8, 
//	+ palette 64 entries, not compressed
//	+ 9 tiles (t reduced) not compressed
//	+ regular map (flat), not compressed, 256x64 
//	Total size: 128 + 576 + 32768 = 33472
//
//	Time-stamp: 2024-12-21, 18:17:37
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TEST_ROOM_2_BN_GFX_H
#define GRIT_TEST_ROOM_2_BN_GFX_H

#define test_room_2_bn_gfxTilesLen 576
extern const bn::tile test_room_2_bn_gfxTiles[18];

#define test_room_2_bn_gfxMapLen 32768
extern const bn::regular_bg_map_cell test_room_2_bn_gfxMap[16384];

#define test_room_2_bn_gfxPalLen 128
extern const bn::color test_room_2_bn_gfxPal[64];

#endif // GRIT_TEST_ROOM_2_BN_GFX_H

//}}BLOCK(test_room_2_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item test_room_2(
            regular_bg_tiles_item(span<const tile>(test_room_2_bn_gfxTiles, 18), bpp_mode::BPP_8, compression_type::NONE), 
            bg_palette_item(span<const color>(test_room_2_bn_gfxPal, 64), bpp_mode::BPP_8, compression_type::NONE),
            regular_bg_map_item(test_room_2_bn_gfxMap[0], size(256, 64), compression_type::NONE, 1, true));
}

#endif

