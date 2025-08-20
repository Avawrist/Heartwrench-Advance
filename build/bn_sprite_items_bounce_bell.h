#ifndef BN_SPRITE_ITEMS_BOUNCE_BELL_H
#define BN_SPRITE_ITEMS_BOUNCE_BELL_H

#include "bn_sprite_item.h"

//{{BLOCK(bounce_bell_bn_gfx)

//======================================================================
//
//	bounce_bell_bn_gfx, 64x704@4, 
//	+ palette 16 entries, not compressed
//	+ 704 tiles not compressed
//	Total size: 32 + 22528 = 22560
//
//	Time-stamp: 2025-08-19, 17:24:23
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BOUNCE_BELL_BN_GFX_H
#define GRIT_BOUNCE_BELL_BN_GFX_H

#define bounce_bell_bn_gfxTilesLen 22528
extern const bn::tile bounce_bell_bn_gfxTiles[704];

#define bounce_bell_bn_gfxPalLen 32
extern const bn::color bounce_bell_bn_gfxPal[16];

#endif // GRIT_BOUNCE_BELL_BN_GFX_H

//}}BLOCK(bounce_bell_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item bounce_bell(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(bounce_bell_bn_gfxTiles, 704), bpp_mode::BPP_4, compression_type::NONE, 11), 
            sprite_palette_item(span<const color>(bounce_bell_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

