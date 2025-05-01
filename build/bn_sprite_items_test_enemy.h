#ifndef BN_SPRITE_ITEMS_TEST_ENEMY_H
#define BN_SPRITE_ITEMS_TEST_ENEMY_H

#include "bn_sprite_item.h"

//{{BLOCK(test_enemy_bn_gfx)

//======================================================================
//
//	test_enemy_bn_gfx, 64x64@4, 
//	+ palette 16 entries, not compressed
//	+ 64 tiles not compressed
//	Total size: 32 + 2048 = 2080
//
//	Time-stamp: 2025-05-01, 13:32:15
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TEST_ENEMY_BN_GFX_H
#define GRIT_TEST_ENEMY_BN_GFX_H

#define test_enemy_bn_gfxTilesLen 2048
extern const bn::tile test_enemy_bn_gfxTiles[64];

#define test_enemy_bn_gfxPalLen 32
extern const bn::color test_enemy_bn_gfxPal[16];

#endif // GRIT_TEST_ENEMY_BN_GFX_H

//}}BLOCK(test_enemy_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item test_enemy(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(test_enemy_bn_gfxTiles, 64), bpp_mode::BPP_4, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(test_enemy_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

