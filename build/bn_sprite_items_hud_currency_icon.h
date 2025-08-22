#ifndef BN_SPRITE_ITEMS_HUD_CURRENCY_ICON_H
#define BN_SPRITE_ITEMS_HUD_CURRENCY_ICON_H

#include "bn_sprite_item.h"

//{{BLOCK(hud_currency_icon_bn_gfx)

//======================================================================
//
//	hud_currency_icon_bn_gfx, 32x32@4, 
//	+ palette 16 entries, not compressed
//	+ 16 tiles not compressed
//	Total size: 32 + 512 = 544
//
//	Time-stamp: 2025-08-21, 20:33:30
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_HUD_CURRENCY_ICON_BN_GFX_H
#define GRIT_HUD_CURRENCY_ICON_BN_GFX_H

#define hud_currency_icon_bn_gfxTilesLen 512
extern const bn::tile hud_currency_icon_bn_gfxTiles[16];

#define hud_currency_icon_bn_gfxPalLen 32
extern const bn::color hud_currency_icon_bn_gfxPal[16];

#endif // GRIT_HUD_CURRENCY_ICON_BN_GFX_H

//}}BLOCK(hud_currency_icon_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item hud_currency_icon(sprite_shape_size(sprite_shape::SQUARE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(hud_currency_icon_bn_gfxTiles, 16), bpp_mode::BPP_4, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(hud_currency_icon_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

