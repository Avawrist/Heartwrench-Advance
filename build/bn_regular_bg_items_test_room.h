#ifndef BN_REGULAR_BG_ITEMS_TEST_ROOM_H
#define BN_REGULAR_BG_ITEMS_TEST_ROOM_H

#include "bn_regular_bg_item.h"

//{{BLOCK(test_room_bn_gfx)

//======================================================================
//
//	test_room_bn_gfx, 512x512@8, 
//	+ palette 48 entries, not compressed
//	+ 3 tiles (t|f reduced) not compressed
//	+ regular map (in SBBs), not compressed, 64x64 
//	Total size: 96 + 192 + 8192 = 8480
//
//	Time-stamp: 2024-11-10, 21:36:09
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TEST_ROOM_BN_GFX_H
#define GRIT_TEST_ROOM_BN_GFX_H

#define test_room_bn_gfxTilesLen 192
extern const bn::tile test_room_bn_gfxTiles[6];

#define test_room_bn_gfxMapLen 8192
extern const bn::regular_bg_map_cell test_room_bn_gfxMap[4096];

#define test_room_bn_gfxPalLen 96
extern const bn::color test_room_bn_gfxPal[48];

#endif // GRIT_TEST_ROOM_BN_GFX_H

//}}BLOCK(test_room_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item test_room(
            regular_bg_tiles_item(span<const tile>(test_room_bn_gfxTiles, 6), bpp_mode::BPP_8, compression_type::NONE), 
            bg_palette_item(span<const color>(test_room_bn_gfxPal, 48), bpp_mode::BPP_8, compression_type::NONE),
            regular_bg_map_item(test_room_bn_gfxMap[0], size(64, 64), compression_type::NONE, 1, false));
}

#endif

