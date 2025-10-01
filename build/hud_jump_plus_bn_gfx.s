
@{{BLOCK(hud_jump_plus_bn_gfx)

@=======================================================================
@
@	hud_jump_plus_bn_gfx, 16x16@4, 
@	+ palette 16 entries, not compressed
@	+ 4 tiles not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2025-10-01, 11:51:26
@	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global hud_jump_plus_bn_gfxTiles		@ 128 unsigned chars
	.hidden hud_jump_plus_bn_gfxTiles
hud_jump_plus_bn_gfxTiles:
	.word 0x00000000,0x10000000,0x61000000,0x61000000,0x65100000,0x55100000,0x65100000,0x65510000
	.word 0x00000000,0x00000001,0x00000016,0x00000016,0x00000001,0x01110015,0x11311016,0x13331166
	.word 0x66510000,0x66541100,0x66554410,0x65555541,0x55555551,0x55555110,0x11111000,0x00000000
	.word 0x11311666,0x11115666,0x15555666,0x01555566,0x00115555,0x00001115,0x00000001,0x00000000

	.section .rodata
	.align	2
	.global hud_jump_plus_bn_gfxPal		@ 32 unsigned chars
	.hidden hud_jump_plus_bn_gfxPal
hud_jump_plus_bn_gfxPal:
	.hword 0x1886,0x0000,0x73DB,0x7FFF,0x2CE6,0x4149,0x61AA,0x7EAA
	.hword 0x7FB3,0x1D35,0x21DC,0x2A7F,0x2F3F,0x3072,0x2C9A,0x415F

@}}BLOCK(hud_jump_plus_bn_gfx)
