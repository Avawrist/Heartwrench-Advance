#ifndef BN_SPRITE_ITEMS_FINISH_SEAL_H
#define BN_SPRITE_ITEMS_FINISH_SEAL_H

#include "bn_sprite_item.h"

//{{BLOCK(finish_seal_bn_gfx)

//======================================================================
//
//	finish_seal_bn_gfx, 64x896@4, 
//	+ palette 16 entries, not compressed
//	+ 896 tiles not compressed
//	Total size: 32 + 28672 = 28704
//
//	Time-stamp: 2025-09-18, 16:16:32
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_FINISH_SEAL_BN_GFX_H
#define GRIT_FINISH_SEAL_BN_GFX_H

#define finish_seal_bn_gfxTilesLen 28672
extern const bn::tile finish_seal_bn_gfxTiles[896];

#define finish_seal_bn_gfxPalLen 32
extern const bn::color finish_seal_bn_gfxPal[16];

#endif // GRIT_FINISH_SEAL_BN_GFX_H

//}}BLOCK(finish_seal_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item finish_seal(sprite_shape_size(sprite_shape::SQUARE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(finish_seal_bn_gfxTiles, 896), bpp_mode::BPP_4, compression_type::NONE, 14), 
            sprite_palette_item(span<const color>(finish_seal_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

