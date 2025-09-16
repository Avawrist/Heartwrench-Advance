#ifndef BN_SPRITE_ITEMS_CHECKPOINT_H
#define BN_SPRITE_ITEMS_CHECKPOINT_H

#include "bn_sprite_item.h"

//{{BLOCK(checkpoint_bn_gfx)

//======================================================================
//
//	checkpoint_bn_gfx, 32x384@4, 
//	+ palette 16 entries, not compressed
//	+ 192 tiles not compressed
//	Total size: 32 + 6144 = 6176
//
//	Time-stamp: 2025-09-16, 00:04:01
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_CHECKPOINT_BN_GFX_H
#define GRIT_CHECKPOINT_BN_GFX_H

#define checkpoint_bn_gfxTilesLen 6144
extern const bn::tile checkpoint_bn_gfxTiles[192];

#define checkpoint_bn_gfxPalLen 32
extern const bn::color checkpoint_bn_gfxPal[16];

#endif // GRIT_CHECKPOINT_BN_GFX_H

//}}BLOCK(checkpoint_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item checkpoint(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(checkpoint_bn_gfxTiles, 192), bpp_mode::BPP_4, compression_type::NONE, 12), 
            sprite_palette_item(span<const color>(checkpoint_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

