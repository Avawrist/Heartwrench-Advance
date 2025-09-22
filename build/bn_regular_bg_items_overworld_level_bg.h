#ifndef BN_REGULAR_BG_ITEMS_OVERWORLD_LEVEL_BG_H
#define BN_REGULAR_BG_ITEMS_OVERWORLD_LEVEL_BG_H

#include "bn_regular_bg_item.h"

//{{BLOCK(overworld_level_bg_bn_gfx)

//======================================================================
//
//	overworld_level_bg_bn_gfx, 1024x3072@8, 
//	+ palette 80 entries, not compressed
//	+ 110 tiles (t reduced) not compressed
//	+ regular map (flat), not compressed, 128x384 
//	Total size: 160 + 7040 + 98304 = 105504
//
//	Time-stamp: 2025-09-21, 17:21:47
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_OVERWORLD_LEVEL_BG_BN_GFX_H
#define GRIT_OVERWORLD_LEVEL_BG_BN_GFX_H

#define overworld_level_bg_bn_gfxTilesLen 7040
extern const bn::tile overworld_level_bg_bn_gfxTiles[220];

#define overworld_level_bg_bn_gfxMapLen 98304
extern const bn::regular_bg_map_cell overworld_level_bg_bn_gfxMap[49152];

#define overworld_level_bg_bn_gfxPalLen 160
extern const bn::color overworld_level_bg_bn_gfxPal[80];

#endif // GRIT_OVERWORLD_LEVEL_BG_BN_GFX_H

//}}BLOCK(overworld_level_bg_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item overworld_level_bg(
            regular_bg_tiles_item(span<const tile>(overworld_level_bg_bn_gfxTiles, 220), bpp_mode::BPP_8, compression_type::NONE), 
            bg_palette_item(span<const color>(overworld_level_bg_bn_gfxPal, 80), bpp_mode::BPP_8, compression_type::NONE),
            regular_bg_map_item(overworld_level_bg_bn_gfxMap[0], size(128, 128), compression_type::NONE, 3, true));
}

#endif

