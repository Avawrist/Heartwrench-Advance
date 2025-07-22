#ifndef BN_REGULAR_BG_ITEMS_PAUSE_SCREEN_H
#define BN_REGULAR_BG_ITEMS_PAUSE_SCREEN_H

#include "bn_regular_bg_item.h"

//{{BLOCK(pause_screen_bn_gfx)

//======================================================================
//
//	pause_screen_bn_gfx, 256x256@4, 
//	+ palette 16 entries, not compressed
//	+ 2 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 32 + 64 + 2048 = 2144
//
//	Time-stamp: 2025-07-21, 17:28:37
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PAUSE_SCREEN_BN_GFX_H
#define GRIT_PAUSE_SCREEN_BN_GFX_H

#define pause_screen_bn_gfxTilesLen 64
extern const bn::tile pause_screen_bn_gfxTiles[2];

#define pause_screen_bn_gfxMapLen 2048
extern const bn::regular_bg_map_cell pause_screen_bn_gfxMap[1024];

#define pause_screen_bn_gfxPalLen 32
extern const bn::color pause_screen_bn_gfxPal[16];

#endif // GRIT_PAUSE_SCREEN_BN_GFX_H

//}}BLOCK(pause_screen_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item pause_screen(
            regular_bg_tiles_item(span<const tile>(pause_screen_bn_gfxTiles, 2), bpp_mode::BPP_4, compression_type::NONE), 
            bg_palette_item(span<const color>(pause_screen_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE),
            regular_bg_map_item(pause_screen_bn_gfxMap[0], size(32, 32), compression_type::NONE, 1, false));
}

#endif

