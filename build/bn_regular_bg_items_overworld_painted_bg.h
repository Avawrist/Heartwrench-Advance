#ifndef BN_REGULAR_BG_ITEMS_OVERWORLD_PAINTED_BG_H
#define BN_REGULAR_BG_ITEMS_OVERWORLD_PAINTED_BG_H

#include "bn_regular_bg_item.h"

//{{BLOCK(overworld_painted_bg_bn_gfx)

//======================================================================
//
//	overworld_painted_bg_bn_gfx, 512x512@4, 
//	+ palette 16 entries, not compressed
//	+ 1 tiles (t|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 64x64 
//	Total size: 32 + 32 + 8192 = 8256
//
//	Time-stamp: 2025-09-17, 15:19:54
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_OVERWORLD_PAINTED_BG_BN_GFX_H
#define GRIT_OVERWORLD_PAINTED_BG_BN_GFX_H

#define overworld_painted_bg_bn_gfxTilesLen 32
extern const bn::tile overworld_painted_bg_bn_gfxTiles[1];

#define overworld_painted_bg_bn_gfxMapLen 8192
extern const bn::regular_bg_map_cell overworld_painted_bg_bn_gfxMap[4096];

#define overworld_painted_bg_bn_gfxPalLen 32
extern const bn::color overworld_painted_bg_bn_gfxPal[16];

#endif // GRIT_OVERWORLD_PAINTED_BG_BN_GFX_H

//}}BLOCK(overworld_painted_bg_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item overworld_painted_bg(
            regular_bg_tiles_item(span<const tile>(overworld_painted_bg_bn_gfxTiles, 1), bpp_mode::BPP_4, compression_type::NONE), 
            bg_palette_item(span<const color>(overworld_painted_bg_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE),
            regular_bg_map_item(overworld_painted_bg_bn_gfxMap[0], size(64, 64), compression_type::NONE, 1, false));
}

#endif

