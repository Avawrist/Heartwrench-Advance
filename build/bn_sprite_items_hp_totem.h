#ifndef BN_SPRITE_ITEMS_HP_TOTEM_H
#define BN_SPRITE_ITEMS_HP_TOTEM_H

#include "bn_sprite_item.h"

//{{BLOCK(hp_totem_bn_gfx)

//======================================================================
//
//	hp_totem_bn_gfx, 64x384@4, 
//	+ palette 16 entries, not compressed
//	+ 384 tiles not compressed
//	Total size: 32 + 12288 = 12320
//
//	Time-stamp: 2025-07-21, 17:28:37
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_HP_TOTEM_BN_GFX_H
#define GRIT_HP_TOTEM_BN_GFX_H

#define hp_totem_bn_gfxTilesLen 12288
extern const bn::tile hp_totem_bn_gfxTiles[384];

#define hp_totem_bn_gfxPalLen 32
extern const bn::color hp_totem_bn_gfxPal[16];

#endif // GRIT_HP_TOTEM_BN_GFX_H

//}}BLOCK(hp_totem_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item hp_totem(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(hp_totem_bn_gfxTiles, 384), bpp_mode::BPP_4, compression_type::NONE, 6), 
            sprite_palette_item(span<const color>(hp_totem_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

