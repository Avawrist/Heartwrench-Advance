#ifndef BN_SPRITE_ITEMS_GAME_OBJECT_H
#define BN_SPRITE_ITEMS_GAME_OBJECT_H

#include "bn_sprite_item.h"

//{{BLOCK(game_object_bn_gfx)

//======================================================================
//
//	game_object_bn_gfx, 8x8@4, 
//	+ palette 16 entries, not compressed
//	+ 1 tiles not compressed
//	Total size: 32 + 32 = 64
//
//	Time-stamp: 2025-07-15, 17:49:50
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_GAME_OBJECT_BN_GFX_H
#define GRIT_GAME_OBJECT_BN_GFX_H

#define game_object_bn_gfxTilesLen 32
extern const bn::tile game_object_bn_gfxTiles[1];

#define game_object_bn_gfxPalLen 32
extern const bn::color game_object_bn_gfxPal[16];

#endif // GRIT_GAME_OBJECT_BN_GFX_H

//}}BLOCK(game_object_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item game_object(sprite_shape_size(sprite_shape::SQUARE, sprite_size::SMALL), 
            sprite_tiles_item(span<const tile>(game_object_bn_gfxTiles, 1), bpp_mode::BPP_4, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(game_object_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

