#ifndef BN_SPRITE_PALETTE_ITEMS_SPRITE_FLASH_PALETTE_H
#define BN_SPRITE_PALETTE_ITEMS_SPRITE_FLASH_PALETTE_H

#include "bn_sprite_palette_item.h"

//{{BLOCK(sprite_flash_palette_bn_gfx)

//======================================================================
//
//	sprite_flash_palette_bn_gfx, 8x8@4, 
//	+ palette 16 entries, not compressed
//	Total size: 32 = 32
//
//	Time-stamp: 2025-07-21, 17:28:37
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_SPRITE_FLASH_PALETTE_BN_GFX_H
#define GRIT_SPRITE_FLASH_PALETTE_BN_GFX_H

#define sprite_flash_palette_bn_gfxPalLen 32
extern const bn::color sprite_flash_palette_bn_gfxPal[16];

#endif // GRIT_SPRITE_FLASH_PALETTE_BN_GFX_H

//}}BLOCK(sprite_flash_palette_bn_gfx)

namespace bn::sprite_palette_items
{
    constexpr inline sprite_palette_item sprite_flash_palette(span<const color>(sprite_flash_palette_bn_gfxPal, 16), 
            bpp_mode::BPP_4, compression_type::NONE);
}

#endif

