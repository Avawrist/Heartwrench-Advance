
@{{BLOCK(hitbox_bn_gfx)

@=======================================================================
@
@	hitbox_bn_gfx, 16x16@4, 
@	+ palette 16 entries, not compressed
@	+ 4 tiles not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2025-09-28, 14:35:41
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
	.hword 0x1886,0x24D0,0x1D35,0x21DC,0x2A7F,0x2F3F,0x20A9,0x410E
	.hword 0x5D93,0x7E57,0x7EFF,0x3581,0x4E61,0x5701,0x67C6,0x0000

@}}BLOCK(hitbox_bn_gfx)
