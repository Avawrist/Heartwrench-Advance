
@{{BLOCK(flash_palette_bn_gfx)

@=======================================================================
@
@	flash_palette_bn_gfx, 8x8@4, 
@	+ palette 16 entries, not compressed
@	Total size: 32 = 32
@
@	Time-stamp: 2025-05-01, 21:13:59
@	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global flash_palette_bn_gfxPal		@ 32 unsigned chars
	.hidden flash_palette_bn_gfxPal
flash_palette_bn_gfxPal:
	.hword 0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF
	.hword 0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x0000,0x0000

@}}BLOCK(flash_palette_bn_gfx)
