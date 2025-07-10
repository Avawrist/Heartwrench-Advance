#ifndef BN_SPRITE_ITEMS_ENEMY_HP_BAR_H
#define BN_SPRITE_ITEMS_ENEMY_HP_BAR_H

#include "bn_sprite_item.h"

//{{BLOCK(enemy_hp_bar_bn_gfx)

//======================================================================
//
//	enemy_hp_bar_bn_gfx, 16x96@4, 
//	+ palette 16 entries, not compressed
//	+ 24 tiles not compressed
//	Total size: 32 + 768 = 800
//
//	Time-stamp: 2025-07-10, 12:03:38
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_ENEMY_HP_BAR_BN_GFX_H
#define GRIT_ENEMY_HP_BAR_BN_GFX_H

#define enemy_hp_bar_bn_gfxTilesLen 768
extern const bn::tile enemy_hp_bar_bn_gfxTiles[24];

#define enemy_hp_bar_bn_gfxPalLen 32
extern const bn::color enemy_hp_bar_bn_gfxPal[16];

#endif // GRIT_ENEMY_HP_BAR_BN_GFX_H

//}}BLOCK(enemy_hp_bar_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item enemy_hp_bar(sprite_shape_size(sprite_shape::SQUARE, sprite_size::NORMAL), 
            sprite_tiles_item(span<const tile>(enemy_hp_bar_bn_gfxTiles, 24), bpp_mode::BPP_4, compression_type::NONE, 6), 
            sprite_palette_item(span<const color>(enemy_hp_bar_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

