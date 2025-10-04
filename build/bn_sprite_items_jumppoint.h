#ifndef BN_SPRITE_ITEMS_JUMPPOINT_H
#define BN_SPRITE_ITEMS_JUMPPOINT_H

#include "bn_sprite_item.h"

//{{BLOCK(jumppoint_bn_gfx)

//======================================================================
//
//	jumppoint_bn_gfx, 32x384@4, 
//	+ palette 16 entries, not compressed
//	+ 192 tiles not compressed
//	Total size: 32 + 6144 = 6176
//
//	Time-stamp: 2025-10-03, 22:50:08
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_JUMPPOINT_BN_GFX_H
#define GRIT_JUMPPOINT_BN_GFX_H

#define jumppoint_bn_gfxTilesLen 6144
extern const bn::tile jumppoint_bn_gfxTiles[192];

#define jumppoint_bn_gfxPalLen 32
extern const bn::color jumppoint_bn_gfxPal[16];

#endif // GRIT_JUMPPOINT_BN_GFX_H

//}}BLOCK(jumppoint_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item jumppoint(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(jumppoint_bn_gfxTiles, 192), bpp_mode::BPP_4, compression_type::NONE, 12), 
            sprite_palette_item(span<const color>(jumppoint_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

