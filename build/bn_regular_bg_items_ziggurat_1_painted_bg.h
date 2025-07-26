#ifndef BN_REGULAR_BG_ITEMS_ZIGGURAT_1_PAINTED_BG_H
#define BN_REGULAR_BG_ITEMS_ZIGGURAT_1_PAINTED_BG_H

#include "bn_regular_bg_item.h"

//{{BLOCK(ziggurat_1_painted_bg_bn_gfx)

//======================================================================
//
//	ziggurat_1_painted_bg_bn_gfx, 512x512@8, 
//	+ palette 32 entries, not compressed
//	+ 27 tiles (t|f reduced) not compressed
//	+ regular map (in SBBs), not compressed, 64x64 
//	Total size: 64 + 1728 + 8192 = 9984
//
//	Time-stamp: 2025-07-25, 17:27:31
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_ZIGGURAT_1_PAINTED_BG_BN_GFX_H
#define GRIT_ZIGGURAT_1_PAINTED_BG_BN_GFX_H

#define ziggurat_1_painted_bg_bn_gfxTilesLen 1728
extern const bn::tile ziggurat_1_painted_bg_bn_gfxTiles[54];

#define ziggurat_1_painted_bg_bn_gfxMapLen 8192
extern const bn::regular_bg_map_cell ziggurat_1_painted_bg_bn_gfxMap[4096];

#define ziggurat_1_painted_bg_bn_gfxPalLen 64
extern const bn::color ziggurat_1_painted_bg_bn_gfxPal[32];

#endif // GRIT_ZIGGURAT_1_PAINTED_BG_BN_GFX_H

//}}BLOCK(ziggurat_1_painted_bg_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item ziggurat_1_painted_bg(
            regular_bg_tiles_item(span<const tile>(ziggurat_1_painted_bg_bn_gfxTiles, 54), bpp_mode::BPP_8, compression_type::NONE), 
            bg_palette_item(span<const color>(ziggurat_1_painted_bg_bn_gfxPal, 32), bpp_mode::BPP_8, compression_type::NONE),
            regular_bg_map_item(ziggurat_1_painted_bg_bn_gfxMap[0], size(64, 64), compression_type::NONE, 1, false));
}

#endif

