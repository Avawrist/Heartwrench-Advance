#ifndef BN_SPRITE_ITEMS_PLAYER_IDLE_H
#define BN_SPRITE_ITEMS_PLAYER_IDLE_H

#include "bn_sprite_item.h"

//{{BLOCK(player_idle_bn_gfx)

//======================================================================
//
//	player_idle_bn_gfx, 64x256@4, 
//	+ palette 16 entries, not compressed
//	+ 256 tiles not compressed
//	Total size: 32 + 8192 = 8224
//
//	Time-stamp: 2025-09-28, 14:35:42
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PLAYER_IDLE_BN_GFX_H
#define GRIT_PLAYER_IDLE_BN_GFX_H

#define player_idle_bn_gfxTilesLen 8192
extern const bn::tile player_idle_bn_gfxTiles[256];

#define player_idle_bn_gfxPalLen 32
extern const bn::color player_idle_bn_gfxPal[16];

#endif // GRIT_PLAYER_IDLE_BN_GFX_H

//}}BLOCK(player_idle_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item player_idle(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(player_idle_bn_gfxTiles, 256), bpp_mode::BPP_4, compression_type::NONE, 4), 
            sprite_palette_item(span<const color>(player_idle_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

