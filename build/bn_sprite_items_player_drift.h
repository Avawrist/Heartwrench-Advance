#ifndef BN_SPRITE_ITEMS_PLAYER_DRIFT_H
#define BN_SPRITE_ITEMS_PLAYER_DRIFT_H

#include "bn_sprite_item.h"

//{{BLOCK(player_drift_bn_gfx)

//======================================================================
//
//	player_drift_bn_gfx, 64x128@4, 
//	+ palette 16 entries, not compressed
//	+ 128 tiles not compressed
//	Total size: 32 + 4096 = 4128
//
//	Time-stamp: 2025-10-03, 16:13:35
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PLAYER_DRIFT_BN_GFX_H
#define GRIT_PLAYER_DRIFT_BN_GFX_H

#define player_drift_bn_gfxTilesLen 4096
extern const bn::tile player_drift_bn_gfxTiles[128];

#define player_drift_bn_gfxPalLen 32
extern const bn::color player_drift_bn_gfxPal[16];

#endif // GRIT_PLAYER_DRIFT_BN_GFX_H

//}}BLOCK(player_drift_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item player_drift(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(player_drift_bn_gfxTiles, 128), bpp_mode::BPP_4, compression_type::NONE, 2), 
            sprite_palette_item(span<const color>(player_drift_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

