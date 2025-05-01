
@{{BLOCK(flash_palette_bn_gfx)

@=======================================================================
@
@	flash_palette_bn_gfx, 16x16@4, 
@	+ palette 16 entries, not compressed
@	Total size: 32 = 32
@
@	Time-stamp: 2025-04-30, 19:26:54
@	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global flash_palette_bn_gfxPal		@ 32 unsigned chars
	.hidden flash_palette_bn_gfxPal
flash_palette_bn_gfxPal:
	.hword 0x1485,0x20C7,0x314C,0x35B2,0x3E55,0x312D,0x45CF,0x5AB3
	.hword 0x6B78,0x7FFF,0x108D,0x18F6,0x193D,0x25FE,0x1895,0x1CFD

@}}BLOCK(flash_palette_bn_gfx)
