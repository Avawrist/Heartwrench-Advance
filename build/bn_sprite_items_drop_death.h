#ifndef BN_SPRITE_ITEMS_DROP_DEATH_H
#define BN_SPRITE_ITEMS_DROP_DEATH_H

#include "bn_sprite_item.h"

//{{BLOCK(drop_death_bn_gfx)

//======================================================================
//
//	drop_death_bn_gfx, 16x80@4, 
//	+ palette 16 entries, not compressed
//	+ 20 tiles not compressed
//	Total size: 32 + 640 = 672
//
//	Time-stamp: 2025-09-30, 23:19:19
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_DROP_DEATH_BN_GFX_H
#define GRIT_DROP_DEATH_BN_GFX_H

#define drop_death_bn_gfxTilesLen 640
extern const bn::tile drop_death_bn_gfxTiles[20];

#define drop_death_bn_gfxPalLen 32
extern const bn::color drop_death_bn_gfxPal[16];

#endif // GRIT_DROP_DEATH_BN_GFX_H

//}}BLOCK(drop_death_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item drop_death(sprite_shape_size(sprite_shape::SQUARE, sprite_size::NORMAL), 
            sprite_tiles_item(span<const tile>(drop_death_bn_gfxTiles, 20), bpp_mode::BPP_4, compression_type::NONE, 5), 
            sprite_palette_item(span<const color>(drop_death_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

