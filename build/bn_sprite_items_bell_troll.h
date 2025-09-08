#ifndef BN_SPRITE_ITEMS_BELL_TROLL_H
#define BN_SPRITE_ITEMS_BELL_TROLL_H

#include "bn_sprite_item.h"

//{{BLOCK(bell_troll_bn_gfx)

//======================================================================
//
//	bell_troll_bn_gfx, 64x192@4, 
//	+ palette 16 entries, not compressed
//	+ 192 tiles not compressed
//	Total size: 32 + 6144 = 6176
//
//	Time-stamp: 2025-09-08, 04:55:11
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BELL_TROLL_BN_GFX_H
#define GRIT_BELL_TROLL_BN_GFX_H

#define bell_troll_bn_gfxTilesLen 6144
extern const bn::tile bell_troll_bn_gfxTiles[192];

#define bell_troll_bn_gfxPalLen 32
extern const bn::color bell_troll_bn_gfxPal[16];

#endif // GRIT_BELL_TROLL_BN_GFX_H

//}}BLOCK(bell_troll_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item bell_troll(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(bell_troll_bn_gfxTiles, 192), bpp_mode::BPP_4, compression_type::NONE, 3), 
            sprite_palette_item(span<const color>(bell_troll_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

