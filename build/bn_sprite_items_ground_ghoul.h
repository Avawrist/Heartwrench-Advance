#ifndef BN_SPRITE_ITEMS_GROUND_GHOUL_H
#define BN_SPRITE_ITEMS_GROUND_GHOUL_H

#include "bn_sprite_item.h"

//{{BLOCK(ground_ghoul_bn_gfx)

//======================================================================
//
//	ground_ghoul_bn_gfx, 16x16@8, 
//	+ palette 64 entries, not compressed
//	+ 4 tiles not compressed
//	Total size: 128 + 256 = 384
//
//	Time-stamp: 2025-04-30, 18:33:29
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_GROUND_GHOUL_BN_GFX_H
#define GRIT_GROUND_GHOUL_BN_GFX_H

#define ground_ghoul_bn_gfxTilesLen 256
extern const bn::tile ground_ghoul_bn_gfxTiles[8];

#define ground_ghoul_bn_gfxPalLen 128
extern const bn::color ground_ghoul_bn_gfxPal[64];

#endif // GRIT_GROUND_GHOUL_BN_GFX_H

//}}BLOCK(ground_ghoul_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item ground_ghoul(sprite_shape_size(sprite_shape::SQUARE, sprite_size::NORMAL), 
            sprite_tiles_item(span<const tile>(ground_ghoul_bn_gfxTiles, 8), bpp_mode::BPP_8, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(ground_ghoul_bn_gfxPal, 64), bpp_mode::BPP_8, compression_type::NONE));
}

#endif

