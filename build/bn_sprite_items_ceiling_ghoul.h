#ifndef BN_SPRITE_ITEMS_CEILING_GHOUL_H
#define BN_SPRITE_ITEMS_CEILING_GHOUL_H

#include "bn_sprite_item.h"

//{{BLOCK(ceiling_ghoul_bn_gfx)

//======================================================================
//
//	ceiling_ghoul_bn_gfx, 32x608@4, 
//	+ palette 16 entries, not compressed
//	+ 304 tiles not compressed
//	Total size: 32 + 9728 = 9760
//
//	Time-stamp: 2025-05-06, 14:07:50
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_CEILING_GHOUL_BN_GFX_H
#define GRIT_CEILING_GHOUL_BN_GFX_H

#define ceiling_ghoul_bn_gfxTilesLen 9728
extern const bn::tile ceiling_ghoul_bn_gfxTiles[304];

#define ceiling_ghoul_bn_gfxPalLen 32
extern const bn::color ceiling_ghoul_bn_gfxPal[16];

#endif // GRIT_CEILING_GHOUL_BN_GFX_H

//}}BLOCK(ceiling_ghoul_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item ceiling_ghoul(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(ceiling_ghoul_bn_gfxTiles, 304), bpp_mode::BPP_4, compression_type::NONE, 19), 
            sprite_palette_item(span<const color>(ceiling_ghoul_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

