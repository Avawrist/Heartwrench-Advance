#ifndef BN_SPRITE_ITEMS_VASE_DEATH_H
#define BN_SPRITE_ITEMS_VASE_DEATH_H

#include "bn_sprite_item.h"

//{{BLOCK(vase_death_bn_gfx)

//======================================================================
//
//	vase_death_bn_gfx, 64x256@4, 
//	+ palette 16 entries, not compressed
//	+ 256 tiles not compressed
//	Total size: 32 + 8192 = 8224
//
//	Time-stamp: 2025-09-30, 23:19:19
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_VASE_DEATH_BN_GFX_H
#define GRIT_VASE_DEATH_BN_GFX_H

#define vase_death_bn_gfxTilesLen 8192
extern const bn::tile vase_death_bn_gfxTiles[256];

#define vase_death_bn_gfxPalLen 32
extern const bn::color vase_death_bn_gfxPal[16];

#endif // GRIT_VASE_DEATH_BN_GFX_H

//}}BLOCK(vase_death_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item vase_death(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(vase_death_bn_gfxTiles, 256), bpp_mode::BPP_4, compression_type::NONE, 4), 
            sprite_palette_item(span<const color>(vase_death_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

