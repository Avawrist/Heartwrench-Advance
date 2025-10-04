
@{{BLOCK(hud_jump_plus_bn_gfx)

@=======================================================================
@
@	hud_jump_plus_bn_gfx, 16x16@4, 
@	+ palette 16 entries, not compressed
@	+ 4 tiles not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2025-10-03, 22:50:08
@	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global hud_jump_plus_bn_gfxTiles		@ 128 unsigned chars
	.hidden hud_jump_plus_bn_gfxTiles
hud_jump_plus_bn_gfxTiles:
	.word 0x00000000,0x11100000,0x55510000,0x55551000,0x55551000,0x11555100,0x00155100,0x11015100
	.word 0x00000000,0x00000001,0x00000015,0x00000155,0x00000155,0x01111555,0x11311551,0x13331510
	.word 0xFF115100,0xFFF11000,0xCCFC1000,0x66310000,0x666D1000,0x11110000,0x00000000,0x00000000
	.word 0x11311511,0x0111011F,0x000001CF,0x00000013,0x000001D6,0x00000011,0x00000000,0x00000000

	.section .rodata
	.align	2
	.global hud_jump_plus_bn_gfxPal		@ 32 unsigned chars
	.hidden hud_jump_plus_bn_gfxPal
hud_jump_plus_bn_gfxPal:
	.hword 0x1886,0x0000,0x73DB,0x7FFF,0x2CE6,0x4149,0x61AA,0x7EAA
	.hword 0x7FB3,0x1D35,0x21DC,0x2A7F,0x2F3F,0x3072,0x2C9A,0x415F

@}}BLOCK(hud_jump_plus_bn_gfx)
