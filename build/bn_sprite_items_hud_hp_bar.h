#ifndef BN_SPRITE_ITEMS_HUD_HP_BAR_H
#define BN_SPRITE_ITEMS_HUD_HP_BAR_H

#include "bn_sprite_item.h"

//{{BLOCK(hud_hp_bar_bn_gfx)

//======================================================================
//
//	hud_hp_bar_bn_gfx, 64x256@4, 
//	+ palette 16 entries, not compressed
//	+ 256 tiles not compressed
//	Total size: 32 + 8192 = 8224
//
//	Time-stamp: 2025-08-20, 13:54:10
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_HUD_HP_BAR_BN_GFX_H
#define GRIT_HUD_HP_BAR_BN_GFX_H

#define hud_hp_bar_bn_gfxTilesLen 8192
extern const bn::tile hud_hp_bar_bn_gfxTiles[256];

#define hud_hp_bar_bn_gfxPalLen 32
extern const bn::color hud_hp_bar_bn_gfxPal[16];

#endif // GRIT_HUD_HP_BAR_BN_GFX_H

//}}BLOCK(hud_hp_bar_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item hud_hp_bar(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(hud_hp_bar_bn_gfxTiles, 256), bpp_mode::BPP_4, compression_type::NONE, 4), 
            sprite_palette_item(span<const color>(hud_hp_bar_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

