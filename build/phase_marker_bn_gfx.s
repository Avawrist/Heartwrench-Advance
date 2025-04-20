
@{{BLOCK(phase_marker_bn_gfx)

@=======================================================================
@
@	phase_marker_bn_gfx, 16x16@8, 
@	+ palette 16 entries, not compressed
@	+ 4 tiles not compressed
@	Total size: 32 + 256 = 288
@
@	Time-stamp: 2025-04-19, 20:10:34
@	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global phase_marker_bn_gfxTiles		@ 256 unsigned chars
	.hidden phase_marker_bn_gfxTiles
phase_marker_bn_gfxTiles:
	.word 0x00000000,0x09090000,0x00000000,0x00000909,0x09000000,0x00000000,0x00090000,0x00000000
	.word 0x00000900,0x00000000,0x00000900,0x00000000,0x00000009,0x00000000,0x00000009,0x00000000
	.word 0x00000909,0x00000000,0x09090000,0x00000000,0x00000000,0x00000009,0x00000000,0x00000900
	.word 0x00000000,0x00090000,0x00000000,0x00090000,0x00000000,0x09000000,0x00000000,0x09000000
	.word 0x00000009,0x00000000,0x00000009,0x00000000,0x00000900,0x00000000,0x00000900,0x00000000
	.word 0x00090000,0x00000000,0x09000000,0x00000000,0x00000000,0x00000909,0x00000000,0x09090000
	.word 0x00000000,0x09000000,0x00000000,0x09000000,0x00000000,0x00090000,0x00000000,0x00090000
	.word 0x00000000,0x00000900,0x00000000,0x00000009,0x09090000,0x00000000,0x00000909,0x00000000

	.section .rodata
	.align	2
	.global phase_marker_bn_gfxPal		@ 32 unsigned chars
	.hidden phase_marker_bn_gfxPal
phase_marker_bn_gfxPal:
	.hword 0x1485,0x20C7,0x314C,0x35B2,0x3E55,0x312D,0x45CF,0x5AB3
	.hword 0x6B78,0x7FFF,0x108D,0x18F6,0x193D,0x25FE,0x1895,0x1CFD

@}}BLOCK(phase_marker_bn_gfx)
