#ifndef BN_REGULAR_BG_ITEMS_ZIGGURAT_1_PAINTED_BG_H
#define BN_REGULAR_BG_ITEMS_ZIGGURAT_1_PAINTED_BG_H

#include "bn_regular_bg_item.h"

//{{BLOCK(ziggurat_1_painted_bg_bn_gfx)

//======================================================================
//
//	ziggurat_1_painted_bg_bn_gfx, 1280x768@8, 
//	+ palette 64 entries, not compressed
//	+ 66 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 160x96 
//	Total size: 128 + 4224 + 30720 = 35072
//
//	Time-stamp: 2025-07-21, 17:28:38
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_ZIGGURAT_1_PAINTED_BG_BN_GFX_H
#define GRIT_ZIGGURAT_1_PAINTED_BG_BN_GFX_H

#define ziggurat_1_painted_bg_bn_gfxTilesLen 4224
extern const bn::tile ziggurat_1_painted_bg_bn_gfxTiles[132];

#define ziggurat_1_painted_bg_bn_gfxMapLen 30720
extern const bn::regular_bg_map_cell ziggurat_1_painted_bg_bn_gfxMap[15360];

#define ziggurat_1_painted_bg_bn_gfxPalLen 128
extern const bn::color ziggurat_1_painted_bg_bn_gfxPal[64];

#endif // GRIT_ZIGGURAT_1_PAINTED_BG_BN_GFX_H

//}}BLOCK(ziggurat_1_painted_bg_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item ziggurat_1_painted_bg(
            regular_bg_tiles_item(span<const tile>(ziggurat_1_painted_bg_bn_gfxTiles, 132), bpp_mode::BPP_8, compression_type::NONE), 
            bg_palette_item(span<const color>(ziggurat_1_painted_bg_bn_gfxPal, 64), bpp_mode::BPP_8, compression_type::NONE),
            regular_bg_map_item(ziggurat_1_painted_bg_bn_gfxMap[0], size(160, 96), compression_type::NONE, 1, true));
}

#endif

