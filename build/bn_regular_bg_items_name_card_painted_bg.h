#ifndef BN_REGULAR_BG_ITEMS_NAME_CARD_PAINTED_BG_H
#define BN_REGULAR_BG_ITEMS_NAME_CARD_PAINTED_BG_H

#include "bn_regular_bg_item.h"

//{{BLOCK(name_card_painted_bg_bn_gfx)

//======================================================================
//
//	name_card_painted_bg_bn_gfx, 256x256@8, 
//	+ palette 80 entries, not compressed
//	+ 28 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 160 + 1792 + 2048 = 4000
//
//	Time-stamp: 2025-09-05, 12:01:04
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_NAME_CARD_PAINTED_BG_BN_GFX_H
#define GRIT_NAME_CARD_PAINTED_BG_BN_GFX_H

#define name_card_painted_bg_bn_gfxTilesLen 1792
extern const bn::tile name_card_painted_bg_bn_gfxTiles[56];

#define name_card_painted_bg_bn_gfxMapLen 2048
extern const bn::regular_bg_map_cell name_card_painted_bg_bn_gfxMap[1024];

#define name_card_painted_bg_bn_gfxPalLen 160
extern const bn::color name_card_painted_bg_bn_gfxPal[80];

#endif // GRIT_NAME_CARD_PAINTED_BG_BN_GFX_H

//}}BLOCK(name_card_painted_bg_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item name_card_painted_bg(
            regular_bg_tiles_item(span<const tile>(name_card_painted_bg_bn_gfxTiles, 56), bpp_mode::BPP_8, compression_type::NONE), 
            bg_palette_item(span<const color>(name_card_painted_bg_bn_gfxPal, 80), bpp_mode::BPP_8, compression_type::NONE),
            regular_bg_map_item(name_card_painted_bg_bn_gfxMap[0], size(32, 32), compression_type::NONE, 1, false));
}

#endif

