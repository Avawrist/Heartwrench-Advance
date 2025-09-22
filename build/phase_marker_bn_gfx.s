
@{{BLOCK(phase_marker_bn_gfx)

@=======================================================================
@
@	phase_marker_bn_gfx, 16x16@4, 
@	+ palette 16 entries, not compressed
@	+ 4 tiles not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2025-09-21, 17:21:47
@	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global phase_marker_bn_gfxTiles		@ 128 unsigned chars
	.hidden phase_marker_bn_gfxTiles
phase_marker_bn_gfxTiles:
	.word 0x11000000,0x00110000,0x00001000,0x00000100,0x00000010,0x00000010,0x00000001,0x00000001
	.word 0x00000011,0x00001100,0x00010000,0x00100000,0x01000000,0x01000000,0x10000000,0x10000000
	.word 0x00000001,0x00000001,0x00000010,0x00000010,0x00000100,0x00001000,0x00110000,0x11000000
	.word 0x10000000,0x10000000,0x01000000,0x01000000,0x00100000,0x00010000,0x00001100,0x00000011

	.section .rodata
	.align	2
	.global phase_marker_bn_gfxPal		@ 32 unsigned chars
	.hidden phase_marker_bn_gfxPal
phase_marker_bn_gfxPal:
	.hword 0x1886,0x7FFF,0x24E8,0x356D,0x39D4,0x4297,0x354E,0x4DF1
	.hword 0x62F5,0x73DB,0x14AF,0x18F8,0x1D5F,0x2A3F,0x1C97,0x211F

@}}BLOCK(phase_marker_bn_gfx)
