#ifndef BN_SPRITE_ITEMS_STAR_JAR_H
#define BN_SPRITE_ITEMS_STAR_JAR_H

#include "bn_sprite_item.h"

//{{BLOCK(star_jar_bn_gfx)

//======================================================================
//
//	star_jar_bn_gfx, 32x288@4, 
//	+ palette 16 entries, not compressed
//	+ 144 tiles not compressed
//	Total size: 32 + 4608 = 4640
//
//	Time-stamp: 2025-09-16, 13:33:37
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_STAR_JAR_BN_GFX_H
#define GRIT_STAR_JAR_BN_GFX_H

#define star_jar_bn_gfxTilesLen 4608
extern const bn::tile star_jar_bn_gfxTiles[144];

#define star_jar_bn_gfxPalLen 32
extern const bn::color star_jar_bn_gfxPal[16];

#endif // GRIT_STAR_JAR_BN_GFX_H

//}}BLOCK(star_jar_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item star_jar(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(star_jar_bn_gfxTiles, 144), bpp_mode::BPP_4, compression_type::NONE, 9), 
            sprite_palette_item(span<const color>(star_jar_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

