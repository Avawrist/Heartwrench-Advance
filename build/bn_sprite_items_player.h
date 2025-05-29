#ifndef BN_SPRITE_ITEMS_PLAYER_H
#define BN_SPRITE_ITEMS_PLAYER_H

#include "bn_sprite_item.h"

//{{BLOCK(player_bn_gfx)

//======================================================================
//
//	player_bn_gfx, 64x2816@4, 
//	+ palette 16 entries, not compressed
//	+ 2816 tiles not compressed
//	Total size: 32 + 90112 = 90144
//
//	Time-stamp: 2025-05-28, 18:36:47
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PLAYER_BN_GFX_H
#define GRIT_PLAYER_BN_GFX_H

#define player_bn_gfxTilesLen 90112
extern const bn::tile player_bn_gfxTiles[2816];

#define player_bn_gfxPalLen 32
extern const bn::color player_bn_gfxPal[16];

#endif // GRIT_PLAYER_BN_GFX_H

//}}BLOCK(player_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item player(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(player_bn_gfxTiles, 2816), bpp_mode::BPP_4, compression_type::NONE, 44), 
            sprite_palette_item(span<const color>(player_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

