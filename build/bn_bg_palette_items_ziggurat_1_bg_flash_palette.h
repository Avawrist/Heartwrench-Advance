#ifndef BN_BG_PALETTE_ITEMS_ZIGGURAT_1_BG_FLASH_PALETTE_H
#define BN_BG_PALETTE_ITEMS_ZIGGURAT_1_BG_FLASH_PALETTE_H

#include "bn_bg_palette_item.h"

//{{BLOCK(ziggurat_1_bg_flash_palette_bn_gfx)

//======================================================================
//
//	ziggurat_1_bg_flash_palette_bn_gfx, 8x8@4, 
//	+ palette 16 entries, not compressed
//	Total size: 32 = 32
//
//	Time-stamp: 2025-08-18, 15:10:56
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_ZIGGURAT_1_BG_FLASH_PALETTE_BN_GFX_H
#define GRIT_ZIGGURAT_1_BG_FLASH_PALETTE_BN_GFX_H

#define ziggurat_1_bg_flash_palette_bn_gfxPalLen 32
extern const bn::color ziggurat_1_bg_flash_palette_bn_gfxPal[16];

#endif // GRIT_ZIGGURAT_1_BG_FLASH_PALETTE_BN_GFX_H

//}}BLOCK(ziggurat_1_bg_flash_palette_bn_gfx)

namespace bn::bg_palette_items
{
    constexpr inline bg_palette_item ziggurat_1_bg_flash_palette(span<const color>(ziggurat_1_bg_flash_palette_bn_gfxPal, 16), 
            bpp_mode::BPP_4, compression_type::NONE);
}

#endif

