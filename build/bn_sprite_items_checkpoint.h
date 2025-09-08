#ifndef BN_SPRITE_ITEMS_CHECKPOINT_H
#define BN_SPRITE_ITEMS_CHECKPOINT_H

#include "bn_sprite_item.h"

//{{BLOCK(checkpoint_bn_gfx)

//======================================================================
//
//	checkpoint_bn_gfx, 32x224@4, 
//	+ palette 16 entries, not compressed
//	+ 112 tiles not compressed
//	Total size: 32 + 3584 = 3616
//
//	Time-stamp: 2025-09-08, 00:26:34
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_CHECKPOINT_BN_GFX_H
#define GRIT_CHECKPOINT_BN_GFX_H

#define checkpoint_bn_gfxTilesLen 3584
extern const bn::tile checkpoint_bn_gfxTiles[112];

#define checkpoint_bn_gfxPalLen 32
extern const bn::color checkpoint_bn_gfxPal[16];

#endif // GRIT_CHECKPOINT_BN_GFX_H

//}}BLOCK(checkpoint_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item checkpoint(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(checkpoint_bn_gfxTiles, 112), bpp_mode::BPP_4, compression_type::NONE, 7), 
            sprite_palette_item(span<const color>(checkpoint_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

