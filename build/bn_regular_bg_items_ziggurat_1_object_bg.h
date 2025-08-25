#ifndef BN_REGULAR_BG_ITEMS_ZIGGURAT_1_OBJECT_BG_H
#define BN_REGULAR_BG_ITEMS_ZIGGURAT_1_OBJECT_BG_H

#include "bn_regular_bg_item.h"

//{{BLOCK(ziggurat_1_object_bg_bn_gfx)

//======================================================================
//
//	ziggurat_1_object_bg_bn_gfx, 5120x2560@8, 
//	+ palette 16 entries, not compressed
//	+ 32 tiles (t reduced) not compressed
//	+ regular map (flat), not compressed, 640x320 
//	Total size: 32 + 2048 + 409600 = 411680
//
//	Time-stamp: 2025-08-24, 17:01:09
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_ZIGGURAT_1_OBJECT_BG_BN_GFX_H
#define GRIT_ZIGGURAT_1_OBJECT_BG_BN_GFX_H

#define ziggurat_1_object_bg_bn_gfxTilesLen 2048
extern const bn::tile ziggurat_1_object_bg_bn_gfxTiles[64];

#define ziggurat_1_object_bg_bn_gfxMapLen 409600
extern const bn::regular_bg_map_cell ziggurat_1_object_bg_bn_gfxMap[204800];

#define ziggurat_1_object_bg_bn_gfxPalLen 32
extern const bn::color ziggurat_1_object_bg_bn_gfxPal[16];

#endif // GRIT_ZIGGURAT_1_OBJECT_BG_BN_GFX_H

//}}BLOCK(ziggurat_1_object_bg_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item ziggurat_1_object_bg(
            regular_bg_tiles_item(span<const tile>(ziggurat_1_object_bg_bn_gfxTiles, 64), bpp_mode::BPP_8, compression_type::NONE), 
            bg_palette_item(span<const color>(ziggurat_1_object_bg_bn_gfxPal, 16), bpp_mode::BPP_8, compression_type::NONE),
            regular_bg_map_item(ziggurat_1_object_bg_bn_gfxMap[0], size(640, 320), compression_type::NONE, 1, true));
}

#endif

