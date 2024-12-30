#ifndef BN_SPRITE_PALETTE_ITEMS_DEFAULT_SPRITE_PALETTE_H
#define BN_SPRITE_PALETTE_ITEMS_DEFAULT_SPRITE_PALETTE_H

#include "bn_sprite_palette_item.h"

//{{BLOCK(default_sprite_palette_bn_gfx)

//======================================================================
//
//	default_sprite_palette_bn_gfx, 64x8@8, 
//	+ palette 64 entries, not compressed
//	Total size: 128 = 128
//
//	Time-stamp: 2024-12-29, 17:39:14
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_DEFAULT_SPRITE_PALETTE_BN_GFX_H
#define GRIT_DEFAULT_SPRITE_PALETTE_BN_GFX_H

#define default_sprite_palette_bn_gfxPalLen 128
extern const bn::color default_sprite_palette_bn_gfxPal[64];

#endif // GRIT_DEFAULT_SPRITE_PALETTE_BN_GFX_H

//}}BLOCK(default_sprite_palette_bn_gfx)

namespace bn::sprite_palette_items
{
    constexpr inline sprite_palette_item default_sprite_palette(span<const color>(default_sprite_palette_bn_gfxPal, 64), 
            bpp_mode::BPP_8, compression_type::NONE);
}

#endif

