#ifndef BN_SPRITE_ITEMS_PLAYER_H
#define BN_SPRITE_ITEMS_PLAYER_H

#include "bn_sprite_item.h"

//{{BLOCK(player_bn_gfx)

//======================================================================
//
//	player_bn_gfx, 64x384@8, 
//	+ palette 64 entries, not compressed
//	+ 384 tiles not compressed
//	Total size: 128 + 24576 = 24704
//
//	Time-stamp: 2025-04-28, 13:18:17
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PLAYER_BN_GFX_H
#define GRIT_PLAYER_BN_GFX_H

#define player_bn_gfxTilesLen 24576
extern const bn::tile player_bn_gfxTiles[768];

#define player_bn_gfxPalLen 128
extern const bn::color player_bn_gfxPal[64];

#endif // GRIT_PLAYER_BN_GFX_H

//}}BLOCK(player_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item player(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(player_bn_gfxTiles, 768), bpp_mode::BPP_8, compression_type::NONE, 6), 
            sprite_palette_item(span<const color>(player_bn_gfxPal, 64), bpp_mode::BPP_8, compression_type::NONE));
}

#endif

