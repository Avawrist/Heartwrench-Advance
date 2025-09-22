
@{{BLOCK(game_object_bn_gfx)

@=======================================================================
@
@	game_object_bn_gfx, 8x8@4, 
@	+ palette 16 entries, not compressed
@	+ 1 tiles not compressed
@	Total size: 32 + 32 = 64
@
@	Time-stamp: 2025-09-21, 17:21:47
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
	.hword 0x1886,0x7FFF,0x24E8,0x356D,0x39D4,0x4297,0x354E,0x4DF1
	.hword 0x62F5,0x73DB,0x14AF,0x18F8,0x1D5F,0x2A3F,0x1C97,0x211F

@}}BLOCK(game_object_bn_gfx)
