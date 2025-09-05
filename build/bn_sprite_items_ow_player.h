#ifndef BN_SPRITE_ITEMS_OW_PLAYER_H
#define BN_SPRITE_ITEMS_OW_PLAYER_H

#include "bn_sprite_item.h"

//{{BLOCK(ow_player_bn_gfx)

//======================================================================
//
//	ow_player_bn_gfx, 16x16@4, 
//	+ palette 16 entries, not compressed
//	+ 4 tiles not compressed
//	Total size: 32 + 128 = 160
//
//	Time-stamp: 2025-09-04, 14:34:14
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_OW_PLAYER_BN_GFX_H
#define GRIT_OW_PLAYER_BN_GFX_H

#define ow_player_bn_gfxTilesLen 128
extern const bn::tile ow_player_bn_gfxTiles[4];

#define ow_player_bn_gfxPalLen 32
extern const bn::color ow_player_bn_gfxPal[16];

#endif // GRIT_OW_PLAYER_BN_GFX_H

//}}BLOCK(ow_player_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item ow_player(sprite_shape_size(sprite_shape::SQUARE, sprite_size::NORMAL), 
            sprite_tiles_item(span<const tile>(ow_player_bn_gfxTiles, 4), bpp_mode::BPP_4, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(ow_player_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

