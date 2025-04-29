#ifndef BN_SPRITE_ITEMS_CEILING_GHOUL_H
#define BN_SPRITE_ITEMS_CEILING_GHOUL_H

#include "bn_sprite_item.h"

//{{BLOCK(ceiling_ghoul_bn_gfx)

//======================================================================
//
//	ceiling_ghoul_bn_gfx, 16x16@8, 
//	+ palette 16 entries, not compressed
//	+ 4 tiles not compressed
//	Total size: 32 + 256 = 288
//
//	Time-stamp: 2025-04-29, 16:48:49
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_CEILING_GHOUL_BN_GFX_H
#define GRIT_CEILING_GHOUL_BN_GFX_H

#define ceiling_ghoul_bn_gfxTilesLen 256
extern const bn::tile ceiling_ghoul_bn_gfxTiles[8];

#define ceiling_ghoul_bn_gfxPalLen 32
extern const bn::color ceiling_ghoul_bn_gfxPal[16];

#endif // GRIT_CEILING_GHOUL_BN_GFX_H

//}}BLOCK(ceiling_ghoul_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item ceiling_ghoul(sprite_shape_size(sprite_shape::SQUARE, sprite_size::NORMAL), 
            sprite_tiles_item(span<const tile>(ceiling_ghoul_bn_gfxTiles, 8), bpp_mode::BPP_8, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(ceiling_ghoul_bn_gfxPal, 16), bpp_mode::BPP_8, compression_type::NONE));
}

#endif

