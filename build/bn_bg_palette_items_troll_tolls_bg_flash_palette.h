#ifndef BN_BG_PALETTE_ITEMS_TROLL_TOLLS_BG_FLASH_PALETTE_H
#define BN_BG_PALETTE_ITEMS_TROLL_TOLLS_BG_FLASH_PALETTE_H

#include "bn_bg_palette_item.h"

//{{BLOCK(troll_tolls_bg_flash_palette_bn_gfx)

//======================================================================
//
//	troll_tolls_bg_flash_palette_bn_gfx, 8x8@4, 
//	+ palette 16 entries, not compressed
//	Total size: 32 = 32
//
//	Time-stamp: 2025-09-21, 17:21:47
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TROLL_TOLLS_BG_FLASH_PALETTE_BN_GFX_H
#define GRIT_TROLL_TOLLS_BG_FLASH_PALETTE_BN_GFX_H

#define troll_tolls_bg_flash_palette_bn_gfxPalLen 32
extern const bn::color troll_tolls_bg_flash_palette_bn_gfxPal[16];

#endif // GRIT_TROLL_TOLLS_BG_FLASH_PALETTE_BN_GFX_H

//}}BLOCK(troll_tolls_bg_flash_palette_bn_gfx)

namespace bn::bg_palette_items
{
    constexpr inline bg_palette_item troll_tolls_bg_flash_palette(span<const color>(troll_tolls_bg_flash_palette_bn_gfxPal, 16), 
            bpp_mode::BPP_4, compression_type::NONE);
}

#endif

