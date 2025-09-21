#ifndef BN_SPRITE_ITEMS_PLAYER_WRENCH_H
#define BN_SPRITE_ITEMS_PLAYER_WRENCH_H

#include "bn_sprite_item.h"

//{{BLOCK(player_wrench_bn_gfx)

//======================================================================
//
//	player_wrench_bn_gfx, 64x192@4, 
//	+ palette 16 entries, not compressed
//	+ 192 tiles not compressed
//	Total size: 32 + 6144 = 6176
//
//	Time-stamp: 2025-09-20, 16:10:50
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PLAYER_WRENCH_BN_GFX_H
#define GRIT_PLAYER_WRENCH_BN_GFX_H

#define player_wrench_bn_gfxTilesLen 6144
extern const bn::tile player_wrench_bn_gfxTiles[192];

#define player_wrench_bn_gfxPalLen 32
extern const bn::color player_wrench_bn_gfxPal[16];

#endif // GRIT_PLAYER_WRENCH_BN_GFX_H

//}}BLOCK(player_wrench_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item player_wrench(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(player_wrench_bn_gfxTiles, 192), bpp_mode::BPP_4, compression_type::NONE, 3), 
            sprite_palette_item(span<const color>(player_wrench_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

