#ifndef BN_SPRITE_ITEMS_ENEMY_DEATH_H
#define BN_SPRITE_ITEMS_ENEMY_DEATH_H

#include "bn_sprite_item.h"

//{{BLOCK(enemy_death_bn_gfx)

//======================================================================
//
//	enemy_death_bn_gfx, 64x448@4, 
//	+ palette 16 entries, not compressed
//	+ 448 tiles not compressed
//	Total size: 32 + 14336 = 14368
//
//	Time-stamp: 2025-08-23, 17:05:11
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_ENEMY_DEATH_BN_GFX_H
#define GRIT_ENEMY_DEATH_BN_GFX_H

#define enemy_death_bn_gfxTilesLen 14336
extern const bn::tile enemy_death_bn_gfxTiles[448];

#define enemy_death_bn_gfxPalLen 32
extern const bn::color enemy_death_bn_gfxPal[16];

#endif // GRIT_ENEMY_DEATH_BN_GFX_H

//}}BLOCK(enemy_death_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item enemy_death(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(enemy_death_bn_gfxTiles, 448), bpp_mode::BPP_4, compression_type::NONE, 7), 
            sprite_palette_item(span<const color>(enemy_death_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

