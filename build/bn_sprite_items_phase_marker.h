#ifndef BN_SPRITE_ITEMS_PHASE_MARKER_H
#define BN_SPRITE_ITEMS_PHASE_MARKER_H

#include "bn_sprite_item.h"

//{{BLOCK(phase_marker_bn_gfx)

//======================================================================
//
//	phase_marker_bn_gfx, 16x16@8, 
//	+ palette 16 entries, not compressed
//	+ 4 tiles not compressed
//	Total size: 32 + 256 = 288
//
//	Time-stamp: 2025-04-19, 20:10:34
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PHASE_MARKER_BN_GFX_H
#define GRIT_PHASE_MARKER_BN_GFX_H

#define phase_marker_bn_gfxTilesLen 256
extern const bn::tile phase_marker_bn_gfxTiles[8];

#define phase_marker_bn_gfxPalLen 32
extern const bn::color phase_marker_bn_gfxPal[16];

#endif // GRIT_PHASE_MARKER_BN_GFX_H

//}}BLOCK(phase_marker_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item phase_marker(sprite_shape_size(sprite_shape::SQUARE, sprite_size::NORMAL), 
            sprite_tiles_item(span<const tile>(phase_marker_bn_gfxTiles, 8), bpp_mode::BPP_8, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(phase_marker_bn_gfxPal, 16), bpp_mode::BPP_8, compression_type::NONE));
}

#endif

