#ifndef BN_SPRITE_ITEMS_EXIT_H
#define BN_SPRITE_ITEMS_EXIT_H

#include "bn_sprite_item.h"

//{{BLOCK(exit_bn_gfx)

//======================================================================
//
//	exit_bn_gfx, 16x16@8, 
//	+ palette 16 entries, not compressed
//	+ 4 tiles not compressed
//	Total size: 32 + 256 = 288
//
//	Time-stamp: 2024-12-29, 17:39:14
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_EXIT_BN_GFX_H
#define GRIT_EXIT_BN_GFX_H

#define exit_bn_gfxTilesLen 256
extern const bn::tile exit_bn_gfxTiles[8];

#define exit_bn_gfxPalLen 32
extern const bn::color exit_bn_gfxPal[16];

#endif // GRIT_EXIT_BN_GFX_H

//}}BLOCK(exit_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item exit(sprite_shape_size(sprite_shape::SQUARE, sprite_size::NORMAL), 
            sprite_tiles_item(span<const tile>(exit_bn_gfxTiles, 8), bpp_mode::BPP_8, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(exit_bn_gfxPal, 16), bpp_mode::BPP_8, compression_type::NONE));
}

#endif

