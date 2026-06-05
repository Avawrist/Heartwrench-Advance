#ifndef BN_BG_PALETTE_ITEMS_LEVEL_5_BG_FLASH_PALETTE_H
#define BN_BG_PALETTE_ITEMS_LEVEL_5_BG_FLASH_PALETTE_H

#include "bn_bg_palette_item.h"

//{{BLOCK(level_5_bg_flash_palette_bn_gfx)

//======================================================================
//
//	level_5_bg_flash_palette_bn_gfx, 8x8@4, 
//	+ palette 16 entries, not compressed
//	Total size: 32 = 32
//
//	Time-stamp: 2026-06-04, 17:02:05
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_LEVEL_5_BG_FLASH_PALETTE_BN_GFX_H
#define GRIT_LEVEL_5_BG_FLASH_PALETTE_BN_GFX_H

#define level_5_bg_flash_palette_bn_gfxPalLen 32
extern const bn::color level_5_bg_flash_palette_bn_gfxPal[16];

#endif // GRIT_LEVEL_5_BG_FLASH_PALETTE_BN_GFX_H

//}}BLOCK(level_5_bg_flash_palette_bn_gfx)

namespace bn::bg_palette_items
{
    constexpr inline bg_palette_item level_5_bg_flash_palette(span<const color>(level_5_bg_flash_palette_bn_gfxPal, 16), 
            bpp_mode::BPP_4, compression_type::NONE);
}

#endif

