#ifndef BN_SPRITE_ITEMS_AUTO_BOUNCE_BELL_H
#define BN_SPRITE_ITEMS_AUTO_BOUNCE_BELL_H

#include "bn_sprite_item.h"

//{{BLOCK(auto_bounce_bell_bn_gfx)

//======================================================================
//
//	auto_bounce_bell_bn_gfx, 64x832@4, 
//	+ palette 16 entries, not compressed
//	+ 832 tiles not compressed
//	Total size: 32 + 26624 = 26656
//
//	Time-stamp: 2025-09-17, 15:19:54
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_AUTO_BOUNCE_BELL_BN_GFX_H
#define GRIT_AUTO_BOUNCE_BELL_BN_GFX_H

#define auto_bounce_bell_bn_gfxTilesLen 26624
extern const bn::tile auto_bounce_bell_bn_gfxTiles[832];

#define auto_bounce_bell_bn_gfxPalLen 32
extern const bn::color auto_bounce_bell_bn_gfxPal[16];

#endif // GRIT_AUTO_BOUNCE_BELL_BN_GFX_H

//}}BLOCK(auto_bounce_bell_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item auto_bounce_bell(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(auto_bounce_bell_bn_gfxTiles, 832), bpp_mode::BPP_4, compression_type::NONE, 13), 
            sprite_palette_item(span<const color>(auto_bounce_bell_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

