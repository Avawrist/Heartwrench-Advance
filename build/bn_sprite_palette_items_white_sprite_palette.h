#ifndef BN_SPRITE_PALETTE_ITEMS_WHITE_SPRITE_PALETTE_H
#define BN_SPRITE_PALETTE_ITEMS_WHITE_SPRITE_PALETTE_H

#include "bn_sprite_palette_item.h"

//{{BLOCK(white_sprite_palette_bn_gfx)

//======================================================================
//
//	white_sprite_palette_bn_gfx, 64x8@8, 
//	+ palette 32 entries, not compressed
//	Total size: 64 = 64
//
//	Time-stamp: 2025-04-25, 11:31:38
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_WHITE_SPRITE_PALETTE_BN_GFX_H
#define GRIT_WHITE_SPRITE_PALETTE_BN_GFX_H

#define white_sprite_palette_bn_gfxPalLen 64
extern const bn::color white_sprite_palette_bn_gfxPal[32];

#endif // GRIT_WHITE_SPRITE_PALETTE_BN_GFX_H

//}}BLOCK(white_sprite_palette_bn_gfx)

namespace bn::sprite_palette_items
{
    constexpr inline sprite_palette_item white_sprite_palette(span<const color>(white_sprite_palette_bn_gfxPal, 32), 
            bpp_mode::BPP_8, compression_type::NONE);
}

#endif

