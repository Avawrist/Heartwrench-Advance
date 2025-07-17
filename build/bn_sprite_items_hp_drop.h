#ifndef BN_SPRITE_ITEMS_HP_DROP_H
#define BN_SPRITE_ITEMS_HP_DROP_H

#include "bn_sprite_item.h"

//{{BLOCK(hp_drop_bn_gfx)

//======================================================================
//
//	hp_drop_bn_gfx, 32x128@4, 
//	+ palette 16 entries, not compressed
//	+ 64 tiles not compressed
//	Total size: 32 + 2048 = 2080
//
//	Time-stamp: 2025-07-16, 14:04:08
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_HP_DROP_BN_GFX_H
#define GRIT_HP_DROP_BN_GFX_H

#define hp_drop_bn_gfxTilesLen 2048
extern const bn::tile hp_drop_bn_gfxTiles[64];

#define hp_drop_bn_gfxPalLen 32
extern const bn::color hp_drop_bn_gfxPal[16];

#endif // GRIT_HP_DROP_BN_GFX_H

//}}BLOCK(hp_drop_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item hp_drop(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(hp_drop_bn_gfxTiles, 64), bpp_mode::BPP_4, compression_type::NONE, 4), 
            sprite_palette_item(span<const color>(hp_drop_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

