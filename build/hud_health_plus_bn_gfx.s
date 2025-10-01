
@{{BLOCK(hud_health_plus_bn_gfx)

@=======================================================================
@
@	hud_health_plus_bn_gfx, 16x16@4, 
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
	.global hud_health_plus_bn_gfxTiles		@ 128 unsigned chars
	.hidden hud_health_plus_bn_gfxTiles
hud_health_plus_bn_gfxTiles:
	.word 0x00000000,0x00000000,0x00111000,0x01CCC100,0x1CCCCC10,0xCBBBCAA1,0xBBBBAAA1,0xBBBBAAA1
	.word 0x00000000,0x00000000,0x00001110,0x0001CCC1,0x001CCCCC,0x0111CBBB,0x11311BBB,0x13331BBB
	.word 0xBBBBAA10,0xBBBBAA10,0xBB999100,0x99991000,0x99110000,0x11000000,0x00000000,0x00000000
	.word 0x11311BBB,0x0111ABBB,0x0001999B,0x00001999,0x00000119,0x00000001,0x00000000,0x00000000

	.section .rodata
	.align	2
	.global hud_health_plus_bn_gfxPal		@ 32 unsigned chars
	.hidden hud_health_plus_bn_gfxPal
hud_health_plus_bn_gfxPal:
	.hword 0x1886,0x0000,0x73DB,0x7FFF,0x2CE6,0x4149,0x61AA,0x7EAA
	.hword 0x7FB3,0x1D35,0x21DC,0x2A7F,0x2F3F,0x3072,0x2C9A,0x415F

@}}BLOCK(hud_health_plus_bn_gfx)
