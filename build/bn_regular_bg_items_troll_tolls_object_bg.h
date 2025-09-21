#ifndef BN_REGULAR_BG_ITEMS_TROLL_TOLLS_OBJECT_BG_H
#define BN_REGULAR_BG_ITEMS_TROLL_TOLLS_OBJECT_BG_H

#include "bn_regular_bg_item.h"

//{{BLOCK(troll_tolls_object_bg_bn_gfx)

//======================================================================
//
//	troll_tolls_object_bg_bn_gfx, 5120x2560@4, 
//	+ palette 16 entries, not compressed
//	+ 35 tiles (t|p reduced) not compressed
//	+ regular map (flat), not compressed, 640x320 
//	Total size: 32 + 1120 + 409600 = 410752
//
//	Time-stamp: 2025-09-20, 17:14:03
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TROLL_TOLLS_OBJECT_BG_BN_GFX_H
#define GRIT_TROLL_TOLLS_OBJECT_BG_BN_GFX_H

#define troll_tolls_object_bg_bn_gfxTilesLen 1120
extern const bn::tile troll_tolls_object_bg_bn_gfxTiles[35];

#define troll_tolls_object_bg_bn_gfxMapLen 409600
extern const bn::regular_bg_map_cell troll_tolls_object_bg_bn_gfxMap[204800];

#define troll_tolls_object_bg_bn_gfxPalLen 32
extern const bn::color troll_tolls_object_bg_bn_gfxPal[16];

#endif // GRIT_TROLL_TOLLS_OBJECT_BG_BN_GFX_H

//}}BLOCK(troll_tolls_object_bg_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item troll_tolls_object_bg(
            regular_bg_tiles_item(span<const tile>(troll_tolls_object_bg_bn_gfxTiles, 35), bpp_mode::BPP_4, compression_type::NONE), 
            bg_palette_item(span<const color>(troll_tolls_object_bg_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE),
            regular_bg_map_item(troll_tolls_object_bg_bn_gfxMap[0], size(640, 320), compression_type::NONE, 1, true));
}

#endif

