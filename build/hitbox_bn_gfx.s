
@{{BLOCK(hitbox_bn_gfx)

@=======================================================================
@
@	hitbox_bn_gfx, 16x16@4, 
@	+ palette 16 entries, not compressed
@	+ 4 tiles not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2025-08-29, 02:23:07
@	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global hitbox_bn_gfxTiles		@ 128 unsigned chars
	.hidden hitbox_bn_gfxTiles
hitbox_bn_gfxTiles:
	.word 0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD
	.word 0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD
	.word 0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD
	.word 0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD,0xDDDDDDDD

	.section .rodata
	.align	2
	.global hitbox_bn_gfxPal		@ 32 unsigned chars
	.hidden hitbox_bn_gfxPal
hitbox_bn_gfxPal:
	.hword 0x1485,0x20CF,0x1D13,0x1DB9,0x265C,0x2AFF,0x1CA8,0x38ED
	.hword 0x5572,0x7A15,0x76BD,0x3161,0x4621,0x4EA1,0x5F86,0x0000

@}}BLOCK(hitbox_bn_gfx)
