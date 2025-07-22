
@{{BLOCK(phase_marker_bn_gfx)

@=======================================================================
@
@	phase_marker_bn_gfx, 16x16@4, 
@	+ palette 16 entries, not compressed
@	+ 4 tiles not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2025-07-21, 17:28:37
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
	.hword 0x1485,0x7FFF,0x20C7,0x314C,0x35B2,0x3E55,0x312D,0x45CF
	.hword 0x5AB3,0x6B78,0x108D,0x18F6,0x193D,0x25FE,0x1895,0x1CFD

@}}BLOCK(phase_marker_bn_gfx)
