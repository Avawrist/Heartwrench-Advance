#ifndef BN_SPRITE_ITEMS_WALL_LEFT_GHOUL_H
#define BN_SPRITE_ITEMS_WALL_LEFT_GHOUL_H

#include "bn_sprite_item.h"

//{{BLOCK(wall_left_ghoul_bn_gfx)

//======================================================================
//
//	wall_left_ghoul_bn_gfx, 16x16@4, 
//	+ palette 16 entries, not compressed
//	+ 4 tiles not compressed
//	Total size: 32 + 128 = 160
//
//	Time-stamp: 2025-05-01, 13:32:15
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_WALL_LEFT_GHOUL_BN_GFX_H
#define GRIT_WALL_LEFT_GHOUL_BN_GFX_H

#define wall_left_ghoul_bn_gfxTilesLen 128
extern const bn::tile wall_left_ghoul_bn_gfxTiles[4];

#define wall_left_ghoul_bn_gfxPalLen 32
extern const bn::color wall_left_ghoul_bn_gfxPal[16];

#endif // GRIT_WALL_LEFT_GHOUL_BN_GFX_H

//}}BLOCK(wall_left_ghoul_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item wall_left_ghoul(sprite_shape_size(sprite_shape::SQUARE, sprite_size::NORMAL), 
            sprite_tiles_item(span<const tile>(wall_left_ghoul_bn_gfxTiles, 4), bpp_mode::BPP_4, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(wall_left_ghoul_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

