#ifndef BN_REGULAR_BG_ITEMS_ZIGGURAT_1_OBJECT_BG_H
#define BN_REGULAR_BG_ITEMS_ZIGGURAT_1_OBJECT_BG_H

#include "bn_regular_bg_item.h"

//{{BLOCK(ziggurat_1_object_bg_bn_gfx)

//======================================================================
//
//	ziggurat_1_object_bg_bn_gfx, 10240x5120@8, 
//	+ palette 16 entries, not compressed
//	+ 23 tiles (t reduced) not compressed
//	+ regular map (flat), not compressed, 1280x640 
//	Total size: 32 + 1472 + 1638400 = 1639904
//
//	Time-stamp: 2025-08-01, 20:37:06
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_ZIGGURAT_1_OBJECT_BG_BN_GFX_H
#define GRIT_ZIGGURAT_1_OBJECT_BG_BN_GFX_H

#define ziggurat_1_object_bg_bn_gfxTilesLen 1472
extern const bn::tile ziggurat_1_object_bg_bn_gfxTiles[46];

#define ziggurat_1_object_bg_bn_gfxMapLen 1638400
extern const bn::regular_bg_map_cell ziggurat_1_object_bg_bn_gfxMap[819200];

#define ziggurat_1_object_bg_bn_gfxPalLen 32
extern const bn::color ziggurat_1_object_bg_bn_gfxPal[16];

#endif // GRIT_ZIGGURAT_1_OBJECT_BG_BN_GFX_H

//}}BLOCK(ziggurat_1_object_bg_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item ziggurat_1_object_bg(
            regular_bg_tiles_item(span<const tile>(ziggurat_1_object_bg_bn_gfxTiles, 46), bpp_mode::BPP_8, compression_type::NONE), 
            bg_palette_item(span<const color>(ziggurat_1_object_bg_bn_gfxPal, 16), bpp_mode::BPP_8, compression_type::NONE),
            regular_bg_map_item(ziggurat_1_object_bg_bn_gfxMap[0], size(1280, 640), compression_type::NONE, 1, true));
}

#endif

