#ifndef BN_SPRITE_ITEMS_PLAYER_BELL_JUMP_H
#define BN_SPRITE_ITEMS_PLAYER_BELL_JUMP_H

#include "bn_sprite_item.h"

//{{BLOCK(player_bell_jump_bn_gfx)

//======================================================================
//
//	player_bell_jump_bn_gfx, 64x448@4, 
//	+ palette 16 entries, not compressed
//	+ 448 tiles not compressed
//	Total size: 32 + 14336 = 14368
//
//	Time-stamp: 2025-09-17, 15:19:55
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PLAYER_BELL_JUMP_BN_GFX_H
#define GRIT_PLAYER_BELL_JUMP_BN_GFX_H

#define player_bell_jump_bn_gfxTilesLen 14336
extern const bn::tile player_bell_jump_bn_gfxTiles[448];

#define player_bell_jump_bn_gfxPalLen 32
extern const bn::color player_bell_jump_bn_gfxPal[16];

#endif // GRIT_PLAYER_BELL_JUMP_BN_GFX_H

//}}BLOCK(player_bell_jump_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item player_bell_jump(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(player_bell_jump_bn_gfxTiles, 448), bpp_mode::BPP_4, compression_type::NONE, 7), 
            sprite_palette_item(span<const color>(player_bell_jump_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

