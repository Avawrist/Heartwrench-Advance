#ifndef BN_SPRITE_ITEMS_PHASE_ORB_UP_H
#define BN_SPRITE_ITEMS_PHASE_ORB_UP_H

#include "bn_sprite_item.h"

//{{BLOCK(phase_orb_up_bn_gfx)

//======================================================================
//
//	phase_orb_up_bn_gfx, 32x32@8, 
//	+ palette 16 entries, not compressed
//	+ 16 tiles not compressed
//	Total size: 32 + 1024 = 1056
//
//	Time-stamp: 2025-04-26, 13:34:19
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PHASE_ORB_UP_BN_GFX_H
#define GRIT_PHASE_ORB_UP_BN_GFX_H

#define phase_orb_up_bn_gfxTilesLen 1024
extern const bn::tile phase_orb_up_bn_gfxTiles[32];

#define phase_orb_up_bn_gfxPalLen 32
extern const bn::color phase_orb_up_bn_gfxPal[16];

#endif // GRIT_PHASE_ORB_UP_BN_GFX_H

//}}BLOCK(phase_orb_up_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item phase_orb_up(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(phase_orb_up_bn_gfxTiles, 32), bpp_mode::BPP_8, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(phase_orb_up_bn_gfxPal, 16), bpp_mode::BPP_8, compression_type::NONE));
}

#endif

