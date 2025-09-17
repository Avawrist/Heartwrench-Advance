#ifndef BN_SPRITE_ITEMS_PLAYER_OW_H
#define BN_SPRITE_ITEMS_PLAYER_OW_H

#include "bn_sprite_item.h"

//{{BLOCK(player_ow_bn_gfx)

//======================================================================
//
//	player_ow_bn_gfx, 64x256@4, 
//	+ palette 16 entries, not compressed
//	+ 256 tiles not compressed
//	Total size: 32 + 8192 = 8224
//
//	Time-stamp: 2025-09-17, 12:53:06
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PLAYER_OW_BN_GFX_H
#define GRIT_PLAYER_OW_BN_GFX_H

#define player_ow_bn_gfxTilesLen 8192
extern const bn::tile player_ow_bn_gfxTiles[256];

#define player_ow_bn_gfxPalLen 32
extern const bn::color player_ow_bn_gfxPal[16];

#endif // GRIT_PLAYER_OW_BN_GFX_H

//}}BLOCK(player_ow_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item player_ow(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(player_ow_bn_gfxTiles, 256), bpp_mode::BPP_4, compression_type::NONE, 4), 
            sprite_palette_item(span<const color>(player_ow_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

