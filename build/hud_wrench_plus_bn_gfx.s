
@{{BLOCK(hud_wrench_plus_bn_gfx)

@=======================================================================
@
@	hud_wrench_plus_bn_gfx, 16x16@4, 
@	+ palette 16 entries, not compressed
@	+ 4 tiles not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2025-10-01, 14:25:19
@	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global hud_wrench_plus_bn_gfxTiles		@ 128 unsigned chars
	.hidden hud_wrench_plus_bn_gfxTiles
hud_wrench_plus_bn_gfxTiles:
	.word 0x00111000,0x001BB100,0x001CCB10,0x111CCC10,0xBBBCCC10,0xCCCCCC10,0xCCCCCC10,0xCCCCC100
	.word 0x00000111,0x00001BB1,0x0111BCC1,0x11311CC1,0x13331CCB,0x11311CCC,0x0111CCCC,0x00001CCC
	.word 0xCCCC1000,0xBBE10000,0xEEE10000,0xEEB10000,0xBBB10000,0xBBB10000,0xBB100000,0x11000000
	.word 0x000001CC,0x0000001E,0x0000001E,0x0000001B,0x0000001B,0x0000001B,0x00000001,0x00000000

	.section .rodata
	.align	2
	.global hud_wrench_plus_bn_gfxPal		@ 32 unsigned chars
	.hidden hud_wrench_plus_bn_gfxPal
hud_wrench_plus_bn_gfxPal:
	.hword 0x1886,0x0000,0x73DB,0x7FFF,0x2CE6,0x4149,0x61AA,0x7EAA
	.hword 0x7FB3,0x1D35,0x21DC,0x2A7F,0x2F3F,0x3072,0x2C9A,0x415F

@}}BLOCK(hud_wrench_plus_bn_gfx)
