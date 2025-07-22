#ifndef BN_REGULAR_BG_ITEMS_TITLE_SCREEN_PAINTED_BG_H
#define BN_REGULAR_BG_ITEMS_TITLE_SCREEN_PAINTED_BG_H

#include "bn_regular_bg_item.h"

//{{BLOCK(title_screen_painted_bg_bn_gfx)

//======================================================================
//
//	title_screen_painted_bg_bn_gfx, 256x512@8, 
//	+ palette 80 entries, not compressed
//	+ 25 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 32x64 
//	Total size: 160 + 1600 + 4096 = 5856
//
//	Time-stamp: 2025-07-22, 14:46:47
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TITLE_SCREEN_PAINTED_BG_BN_GFX_H
#define GRIT_TITLE_SCREEN_PAINTED_BG_BN_GFX_H

#define title_screen_painted_bg_bn_gfxTilesLen 1600
extern const bn::tile title_screen_painted_bg_bn_gfxTiles[50];

#define title_screen_painted_bg_bn_gfxMapLen 4096
extern const bn::regular_bg_map_cell title_screen_painted_bg_bn_gfxMap[2048];

#define title_screen_painted_bg_bn_gfxPalLen 160
extern const bn::color title_screen_painted_bg_bn_gfxPal[80];

#endif // GRIT_TITLE_SCREEN_PAINTED_BG_BN_GFX_H

//}}BLOCK(title_screen_painted_bg_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item title_screen_painted_bg(
            regular_bg_tiles_item(span<const tile>(title_screen_painted_bg_bn_gfxTiles, 50), bpp_mode::BPP_8, compression_type::NONE), 
            bg_palette_item(span<const color>(title_screen_painted_bg_bn_gfxPal, 80), bpp_mode::BPP_8, compression_type::NONE),
            regular_bg_map_item(title_screen_painted_bg_bn_gfxMap[0], size(32, 32), compression_type::NONE, 2, false));
}

#endif

