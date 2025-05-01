
@{{BLOCK(game_object_bn_gfx)

@=======================================================================
@
@	game_object_bn_gfx, 8x8@4, 
@	+ palette 16 entries, not compressed
@	+ 1 tiles not compressed
@	Total size: 32 + 32 = 64
@
@	Time-stamp: 2025-05-01, 13:32:15
@	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global game_object_bn_gfxTiles		@ 32 unsigned chars
	.hidden game_object_bn_gfxTiles
game_object_bn_gfxTiles:
	.word 0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111

	.section .rodata
	.align	2
	.global game_object_bn_gfxPal		@ 32 unsigned chars
	.hidden game_object_bn_gfxPal
game_object_bn_gfxPal:
	.hword 0x1485,0x7FFF,0x20C7,0x314C,0x35B2,0x3E55,0x312D,0x45CF
	.hword 0x5AB3,0x6B78,0x108D,0x18F6,0x193D,0x25FE,0x1895,0x1CFD

@}}BLOCK(game_object_bn_gfx)
