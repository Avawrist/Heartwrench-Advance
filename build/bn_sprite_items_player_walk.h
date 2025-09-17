#ifndef BN_SPRITE_ITEMS_PLAYER_WALK_H
#define BN_SPRITE_ITEMS_PLAYER_WALK_H

#include "bn_sprite_item.h"

//{{BLOCK(player_walk_bn_gfx)

//======================================================================
//
//	player_walk_bn_gfx, 64x384@4, 
//	+ palette 16 entries, not compressed
//	+ 384 tiles not compressed
//	Total size: 32 + 12288 = 12320
//
//	Time-stamp: 2025-09-17, 15:19:55
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PLAYER_WALK_BN_GFX_H
#define GRIT_PLAYER_WALK_BN_GFX_H

#define player_walk_bn_gfxTilesLen 12288
extern const bn::tile player_walk_bn_gfxTiles[384];

#define player_walk_bn_gfxPalLen 32
extern const bn::color player_walk_bn_gfxPal[16];

#endif // GRIT_PLAYER_WALK_BN_GFX_H

//}}BLOCK(player_walk_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item player_walk(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(player_walk_bn_gfxTiles, 384), bpp_mode::BPP_4, compression_type::NONE, 6), 
            sprite_palette_item(span<const color>(player_walk_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

