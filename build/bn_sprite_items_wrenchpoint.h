#ifndef BN_SPRITE_ITEMS_WRENCHPOINT_H
#define BN_SPRITE_ITEMS_WRENCHPOINT_H

#include "bn_sprite_item.h"

//{{BLOCK(wrenchpoint_bn_gfx)

//======================================================================
//
//	wrenchpoint_bn_gfx, 32x384@4, 
//	+ palette 16 entries, not compressed
//	+ 192 tiles not compressed
//	Total size: 32 + 6144 = 6176
//
//	Time-stamp: 2025-09-30, 12:20:53
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_WRENCHPOINT_BN_GFX_H
#define GRIT_WRENCHPOINT_BN_GFX_H

#define wrenchpoint_bn_gfxTilesLen 6144
extern const bn::tile wrenchpoint_bn_gfxTiles[192];

#define wrenchpoint_bn_gfxPalLen 32
extern const bn::color wrenchpoint_bn_gfxPal[16];

#endif // GRIT_WRENCHPOINT_BN_GFX_H

//}}BLOCK(wrenchpoint_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item wrenchpoint(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(wrenchpoint_bn_gfxTiles, 192), bpp_mode::BPP_4, compression_type::NONE, 12), 
            sprite_palette_item(span<const color>(wrenchpoint_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

