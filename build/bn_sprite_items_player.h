#ifndef BN_SPRITE_ITEMS_PLAYER_H
#define BN_SPRITE_ITEMS_PLAYER_H

#include "bn_sprite_item.h"

//{{BLOCK(player_bn_gfx)

//======================================================================
//
//	player_bn_gfx, 64x2752@4, 
//	+ palette 16 entries, not compressed
//	+ 2752 tiles not compressed
//	Total size: 32 + 88064 = 88096
//
//	Time-stamp: 2025-05-28, 21:21:05
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PLAYER_BN_GFX_H
#define GRIT_PLAYER_BN_GFX_H

#define player_bn_gfxTilesLen 88064
extern const bn::tile player_bn_gfxTiles[2752];

#define player_bn_gfxPalLen 32
extern const bn::color player_bn_gfxPal[16];

#endif // GRIT_PLAYER_BN_GFX_H

//}}BLOCK(player_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item player(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(player_bn_gfxTiles, 2752), bpp_mode::BPP_4, compression_type::NONE, 43), 
            sprite_palette_item(span<const color>(player_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

