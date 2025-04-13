#ifndef BN_SPRITE_ITEMS_PLAYER_H
#define BN_SPRITE_ITEMS_PLAYER_H

#include "bn_sprite_item.h"

//{{BLOCK(player_bn_gfx)

//======================================================================
//
//	player_bn_gfx, 64x64@8, 
//	+ palette 80 entries, not compressed
//	+ 64 tiles not compressed
//	Total size: 160 + 4096 = 4256
//
//	Time-stamp: 2025-04-13, 12:37:51
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PLAYER_BN_GFX_H
#define GRIT_PLAYER_BN_GFX_H

#define player_bn_gfxTilesLen 4096
extern const bn::tile player_bn_gfxTiles[128];

#define player_bn_gfxPalLen 160
extern const bn::color player_bn_gfxPal[80];

#endif // GRIT_PLAYER_BN_GFX_H

//}}BLOCK(player_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item player(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(player_bn_gfxTiles, 128), bpp_mode::BPP_8, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(player_bn_gfxPal, 80), bpp_mode::BPP_8, compression_type::NONE));
}

#endif

