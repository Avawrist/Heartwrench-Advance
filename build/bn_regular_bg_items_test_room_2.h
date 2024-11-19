#ifndef BN_REGULAR_BG_ITEMS_TEST_ROOM_2_H
#define BN_REGULAR_BG_ITEMS_TEST_ROOM_2_H

#include "bn_regular_bg_item.h"

//{{BLOCK(test_room_2_bn_gfx)

//======================================================================
//
//	test_room_2_bn_gfx, 2048x512@8, 
//	+ palette 48 entries, not compressed
//	+ 3 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 256x64 
//	Total size: 96 + 192 + 32768 = 33056
//
//	Time-stamp: 2024-11-13, 20:50:27
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TEST_ROOM_2_BN_GFX_H
#define GRIT_TEST_ROOM_2_BN_GFX_H

#define test_room_2_bn_gfxTilesLen 192
extern const bn::tile test_room_2_bn_gfxTiles[6];

#define test_room_2_bn_gfxMapLen 32768
extern const bn::regular_bg_map_cell test_room_2_bn_gfxMap[16384];

#define test_room_2_bn_gfxPalLen 96
extern const bn::color test_room_2_bn_gfxPal[48];

#endif // GRIT_TEST_ROOM_2_BN_GFX_H

//}}BLOCK(test_room_2_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item test_room_2(
            regular_bg_tiles_item(span<const tile>(test_room_2_bn_gfxTiles, 6), bpp_mode::BPP_8, compression_type::NONE), 
            bg_palette_item(span<const color>(test_room_2_bn_gfxPal, 48), bpp_mode::BPP_8, compression_type::NONE),
            regular_bg_map_item(test_room_2_bn_gfxMap[0], size(256, 64), compression_type::NONE, 1, true));
}

#endif

