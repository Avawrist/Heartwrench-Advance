#ifndef BN_SPRITE_ITEMS_PLAYER_CLIMB_H
#define BN_SPRITE_ITEMS_PLAYER_CLIMB_H

#include "bn_sprite_item.h"

//{{BLOCK(player_climb_bn_gfx)

//======================================================================
//
//	player_climb_bn_gfx, 64x320@4, 
//	+ palette 16 entries, not compressed
//	+ 320 tiles not compressed
//	Total size: 32 + 10240 = 10272
//
//	Time-stamp: 2025-09-18, 16:16:32
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PLAYER_CLIMB_BN_GFX_H
#define GRIT_PLAYER_CLIMB_BN_GFX_H

#define player_climb_bn_gfxTilesLen 10240
extern const bn::tile player_climb_bn_gfxTiles[320];

#define player_climb_bn_gfxPalLen 32
extern const bn::color player_climb_bn_gfxPal[16];

#endif // GRIT_PLAYER_CLIMB_BN_GFX_H

//}}BLOCK(player_climb_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item player_climb(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(player_climb_bn_gfxTiles, 320), bpp_mode::BPP_4, compression_type::NONE, 5), 
            sprite_palette_item(span<const color>(player_climb_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

