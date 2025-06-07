#ifndef BN_SPRITE_ITEMS_PLAYER_H
#define BN_SPRITE_ITEMS_PLAYER_H

#include "bn_sprite_item.h"

//{{BLOCK(player_bn_gfx)

//======================================================================
//
//	player_bn_gfx, 64x4096@4, 
//	+ palette 16 entries, not compressed
//	+ 4096 tiles not compressed
//	Total size: 32 + 131072 = 131104
//
//	Time-stamp: 2025-06-07, 14:37:57
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PLAYER_BN_GFX_H
#define GRIT_PLAYER_BN_GFX_H

#define player_bn_gfxTilesLen 131072
extern const bn::tile player_bn_gfxTiles[4096];

#define player_bn_gfxPalLen 32
extern const bn::color player_bn_gfxPal[16];

#endif // GRIT_PLAYER_BN_GFX_H

//}}BLOCK(player_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item player(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(player_bn_gfxTiles, 4096), bpp_mode::BPP_4, compression_type::NONE, 64), 
            sprite_palette_item(span<const color>(player_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

