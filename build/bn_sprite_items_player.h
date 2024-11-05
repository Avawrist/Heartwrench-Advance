#ifndef BN_SPRITE_ITEMS_PLAYER_H
#define BN_SPRITE_ITEMS_PLAYER_H

#include "bn_sprite_item.h"

//{{BLOCK(player_bn_gfx)

//======================================================================
//
//	player_bn_gfx, 16x16@8, 
//	+ palette 32 entries, not compressed
//	+ 4 tiles not compressed
//	Total size: 64 + 256 = 320
//
//	Time-stamp: 2024-11-04, 15:08:05
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PLAYER_BN_GFX_H
#define GRIT_PLAYER_BN_GFX_H

#define player_bn_gfxTilesLen 256
extern const bn::tile player_bn_gfxTiles[8];

#define player_bn_gfxPalLen 64
extern const bn::color player_bn_gfxPal[32];

#endif // GRIT_PLAYER_BN_GFX_H

//}}BLOCK(player_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item player(sprite_shape_size(sprite_shape::SQUARE, sprite_size::NORMAL), 
            sprite_tiles_item(span<const tile>(player_bn_gfxTiles, 8), bpp_mode::BPP_8, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(player_bn_gfxPal, 32), bpp_mode::BPP_8, compression_type::NONE));
}

#endif

