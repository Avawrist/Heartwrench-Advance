#ifndef BN_SPRITE_ITEMS_HUD_JUMP_PLUS_H
#define BN_SPRITE_ITEMS_HUD_JUMP_PLUS_H

#include "bn_sprite_item.h"

//{{BLOCK(hud_jump_plus_bn_gfx)

//======================================================================
//
//	hud_jump_plus_bn_gfx, 16x16@4, 
//	+ palette 16 entries, not compressed
//	+ 4 tiles not compressed
//	Total size: 32 + 128 = 160
//
//	Time-stamp: 2025-10-01, 11:51:26
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_HUD_JUMP_PLUS_BN_GFX_H
#define GRIT_HUD_JUMP_PLUS_BN_GFX_H

#define hud_jump_plus_bn_gfxTilesLen 128
extern const bn::tile hud_jump_plus_bn_gfxTiles[4];

#define hud_jump_plus_bn_gfxPalLen 32
extern const bn::color hud_jump_plus_bn_gfxPal[16];

#endif // GRIT_HUD_JUMP_PLUS_BN_GFX_H

//}}BLOCK(hud_jump_plus_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item hud_jump_plus(sprite_shape_size(sprite_shape::SQUARE, sprite_size::NORMAL), 
            sprite_tiles_item(span<const tile>(hud_jump_plus_bn_gfxTiles, 4), bpp_mode::BPP_4, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(hud_jump_plus_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

