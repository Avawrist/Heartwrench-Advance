#ifndef BN_SPRITE_ITEMS_TEST_ENEMY_H
#define BN_SPRITE_ITEMS_TEST_ENEMY_H

#include "bn_sprite_item.h"

//{{BLOCK(test_enemy_bn_gfx)

//======================================================================
//
//	test_enemy_bn_gfx, 32x32@8, 
//	+ palette 64 entries, not compressed
//	+ 16 tiles not compressed
//	Total size: 128 + 1024 = 1152
//
//	Time-stamp: 2025-03-19, 15:44:12
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TEST_ENEMY_BN_GFX_H
#define GRIT_TEST_ENEMY_BN_GFX_H

#define test_enemy_bn_gfxTilesLen 1024
extern const bn::tile test_enemy_bn_gfxTiles[32];

#define test_enemy_bn_gfxPalLen 128
extern const bn::color test_enemy_bn_gfxPal[64];

#endif // GRIT_TEST_ENEMY_BN_GFX_H

//}}BLOCK(test_enemy_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item test_enemy(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(test_enemy_bn_gfxTiles, 32), bpp_mode::BPP_8, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(test_enemy_bn_gfxPal, 64), bpp_mode::BPP_8, compression_type::NONE));
}

#endif

