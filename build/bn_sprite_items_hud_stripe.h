#ifndef BN_SPRITE_ITEMS_HUD_STRIPE_H
#define BN_SPRITE_ITEMS_HUD_STRIPE_H

#include "bn_sprite_item.h"

//{{BLOCK(hud_stripe_bn_gfx)

//======================================================================
//
//	hud_stripe_bn_gfx, 64x32@4, 
//	+ palette 16 entries, not compressed
//	+ 32 tiles not compressed
//	Total size: 32 + 1024 = 1056
//
//	Time-stamp: 2025-09-17, 15:19:54
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_HUD_STRIPE_BN_GFX_H
#define GRIT_HUD_STRIPE_BN_GFX_H

#define hud_stripe_bn_gfxTilesLen 1024
extern const bn::tile hud_stripe_bn_gfxTiles[32];

#define hud_stripe_bn_gfxPalLen 32
extern const bn::color hud_stripe_bn_gfxPal[16];

#endif // GRIT_HUD_STRIPE_BN_GFX_H

//}}BLOCK(hud_stripe_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item hud_stripe(sprite_shape_size(sprite_shape::WIDE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(hud_stripe_bn_gfxTiles, 32), bpp_mode::BPP_4, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(hud_stripe_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

