#ifndef BN_REGULAR_BG_ITEMS_LEVEL_2_LEVEL_BG_H
#define BN_REGULAR_BG_ITEMS_LEVEL_2_LEVEL_BG_H

#include "bn_regular_bg_item.h"

//{{BLOCK(level_2_level_bg_bn_gfx)

//======================================================================
//
//	level_2_level_bg_bn_gfx, 5120x2560@4, 
//	+ palette 16 entries, not compressed
//	+ 288 tiles (t|p reduced) not compressed
//	+ regular map (flat), not compressed, 640x320 
//	Total size: 32 + 9216 + 409600 = 418848
//
//	Time-stamp: 2026-06-13, 11:37:23
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_LEVEL_2_LEVEL_BG_BN_GFX_H
#define GRIT_LEVEL_2_LEVEL_BG_BN_GFX_H

#define level_2_level_bg_bn_gfxTilesLen 9216
extern const bn::tile level_2_level_bg_bn_gfxTiles[288];

#define level_2_level_bg_bn_gfxMapLen 409600
extern const bn::regular_bg_map_cell level_2_level_bg_bn_gfxMap[204800];

#define level_2_level_bg_bn_gfxPalLen 32
extern const bn::color level_2_level_bg_bn_gfxPal[16];

#endif // GRIT_LEVEL_2_LEVEL_BG_BN_GFX_H

//}}BLOCK(level_2_level_bg_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item level_2_level_bg(
            regular_bg_tiles_item(span<const tile>(level_2_level_bg_bn_gfxTiles, 288), bpp_mode::BPP_4, compression_type::NONE), 
            bg_palette_item(span<const color>(level_2_level_bg_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE),
            regular_bg_map_item(level_2_level_bg_bn_gfxMap[0], size(640, 320), compression_type::NONE, 1, true));
}

#endif

