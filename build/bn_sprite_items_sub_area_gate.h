#ifndef BN_SPRITE_ITEMS_SUB_AREA_GATE_H
#define BN_SPRITE_ITEMS_SUB_AREA_GATE_H

#include "bn_sprite_item.h"

//{{BLOCK(sub_area_gate_bn_gfx)

//======================================================================
//
//	sub_area_gate_bn_gfx, 64x256@4, 
//	+ palette 16 entries, not compressed
//	+ 256 tiles not compressed
//	Total size: 32 + 8192 = 8224
//
//	Time-stamp: 2025-10-03, 12:06:27
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_SUB_AREA_GATE_BN_GFX_H
#define GRIT_SUB_AREA_GATE_BN_GFX_H

#define sub_area_gate_bn_gfxTilesLen 8192
extern const bn::tile sub_area_gate_bn_gfxTiles[256];

#define sub_area_gate_bn_gfxPalLen 32
extern const bn::color sub_area_gate_bn_gfxPal[16];

#endif // GRIT_SUB_AREA_GATE_BN_GFX_H

//}}BLOCK(sub_area_gate_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item sub_area_gate(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(sub_area_gate_bn_gfxTiles, 256), bpp_mode::BPP_4, compression_type::NONE, 4), 
            sprite_palette_item(span<const color>(sub_area_gate_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

