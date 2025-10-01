#ifndef BN_SPRITE_ITEMS_HP_TOTEM_H
#define BN_SPRITE_ITEMS_HP_TOTEM_H

#include "bn_sprite_item.h"

//{{BLOCK(hp_totem_bn_gfx)

//======================================================================
//
//	hp_totem_bn_gfx, 32x32@4, 
//	+ palette 16 entries, not compressed
//	+ 16 tiles not compressed
//	Total size: 32 + 512 = 544
//
//	Time-stamp: 2025-10-01, 00:36:31
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_HP_TOTEM_BN_GFX_H
#define GRIT_HP_TOTEM_BN_GFX_H

#define hp_totem_bn_gfxTilesLen 512
extern const bn::tile hp_totem_bn_gfxTiles[16];

#define hp_totem_bn_gfxPalLen 32
extern const bn::color hp_totem_bn_gfxPal[16];

#endif // GRIT_HP_TOTEM_BN_GFX_H

//}}BLOCK(hp_totem_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item hp_totem(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(hp_totem_bn_gfxTiles, 16), bpp_mode::BPP_4, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(hp_totem_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

