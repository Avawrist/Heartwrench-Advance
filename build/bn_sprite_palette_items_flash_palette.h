#ifndef BN_SPRITE_PALETTE_ITEMS_FLASH_PALETTE_H
#define BN_SPRITE_PALETTE_ITEMS_FLASH_PALETTE_H

#include "bn_sprite_palette_item.h"

//{{BLOCK(flash_palette_bn_gfx)

//======================================================================
//
//	flash_palette_bn_gfx, 16x16@4, 
//	+ palette 16 entries, not compressed
//	Total size: 32 = 32
//
//	Time-stamp: 2025-04-30, 19:26:54
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_FLASH_PALETTE_BN_GFX_H
#define GRIT_FLASH_PALETTE_BN_GFX_H

#define flash_palette_bn_gfxPalLen 32
extern const bn::color flash_palette_bn_gfxPal[16];

#endif // GRIT_FLASH_PALETTE_BN_GFX_H

//}}BLOCK(flash_palette_bn_gfx)

namespace bn::sprite_palette_items
{
    constexpr inline sprite_palette_item flash_palette(span<const color>(flash_palette_bn_gfxPal, 16), 
            bpp_mode::BPP_4, compression_type::NONE);
}

#endif

