#ifndef BN_SPRITE_ITEMS_GROUND_GHOUL_H
#define BN_SPRITE_ITEMS_GROUND_GHOUL_H

#include "bn_sprite_item.h"

//{{BLOCK(ground_ghoul_bn_gfx)

//======================================================================
//
//	ground_ghoul_bn_gfx, 64x2048@4, 
//	+ palette 16 entries, not compressed
//	+ 2048 tiles not compressed
//	Total size: 32 + 65536 = 65568
//
//	Time-stamp: 2025-06-13, 18:18:15
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_GROUND_GHOUL_BN_GFX_H
#define GRIT_GROUND_GHOUL_BN_GFX_H

#define ground_ghoul_bn_gfxTilesLen 65536
extern const bn::tile ground_ghoul_bn_gfxTiles[2048];

#define ground_ghoul_bn_gfxPalLen 32
extern const bn::color ground_ghoul_bn_gfxPal[16];

#endif // GRIT_GROUND_GHOUL_BN_GFX_H

//}}BLOCK(ground_ghoul_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item ground_ghoul(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(ground_ghoul_bn_gfxTiles, 2048), bpp_mode::BPP_4, compression_type::NONE, 32), 
            sprite_palette_item(span<const color>(ground_ghoul_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

