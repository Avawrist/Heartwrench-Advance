#ifndef BN_SPRITE_ITEMS_CANDELABRA_H
#define BN_SPRITE_ITEMS_CANDELABRA_H

#include "bn_sprite_item.h"

//{{BLOCK(candelabra_bn_gfx)

//======================================================================
//
//	candelabra_bn_gfx, 64x256@4, 
//	+ palette 16 entries, not compressed
//	+ 256 tiles not compressed
//	Total size: 32 + 8192 = 8224
//
//	Time-stamp: 2025-07-15, 00:49:11
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_CANDELABRA_BN_GFX_H
#define GRIT_CANDELABRA_BN_GFX_H

#define candelabra_bn_gfxTilesLen 8192
extern const bn::tile candelabra_bn_gfxTiles[256];

#define candelabra_bn_gfxPalLen 32
extern const bn::color candelabra_bn_gfxPal[16];

#endif // GRIT_CANDELABRA_BN_GFX_H

//}}BLOCK(candelabra_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item candelabra(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(candelabra_bn_gfxTiles, 256), bpp_mode::BPP_4, compression_type::NONE, 4), 
            sprite_palette_item(span<const color>(candelabra_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

