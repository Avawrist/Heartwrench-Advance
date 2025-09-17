#ifndef BN_SPRITE_ITEMS_WINGED_TROLL_H
#define BN_SPRITE_ITEMS_WINGED_TROLL_H

#include "bn_sprite_item.h"

//{{BLOCK(winged_troll_bn_gfx)

//======================================================================
//
//	winged_troll_bn_gfx, 64x128@4, 
//	+ palette 16 entries, not compressed
//	+ 128 tiles not compressed
//	Total size: 32 + 4096 = 4128
//
//	Time-stamp: 2025-09-17, 15:19:55
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_WINGED_TROLL_BN_GFX_H
#define GRIT_WINGED_TROLL_BN_GFX_H

#define winged_troll_bn_gfxTilesLen 4096
extern const bn::tile winged_troll_bn_gfxTiles[128];

#define winged_troll_bn_gfxPalLen 32
extern const bn::color winged_troll_bn_gfxPal[16];

#endif // GRIT_WINGED_TROLL_BN_GFX_H

//}}BLOCK(winged_troll_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item winged_troll(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(winged_troll_bn_gfxTiles, 128), bpp_mode::BPP_4, compression_type::NONE, 2), 
            sprite_palette_item(span<const color>(winged_troll_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

