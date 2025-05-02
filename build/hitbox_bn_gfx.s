
@{{BLOCK(hitbox_bn_gfx)

@=======================================================================
@
@	hitbox_bn_gfx, 16x16@4, 
@	+ palette 16 entries, not compressed
@	+ 4 tiles not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2025-05-01, 20:18:00
@	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global hitbox_bn_gfxTiles		@ 128 unsigned chars
	.hidden hitbox_bn_gfxTiles
hitbox_bn_gfxTiles:
	.word 0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111
	.word 0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111
	.word 0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111
	.word 0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111,0x11111111

	.section .rodata
	.align	2
	.global hitbox_bn_gfxPal		@ 32 unsigned chars
	.hidden hitbox_bn_gfxPal
hitbox_bn_gfxPal:
	.hword 0x1485,0x3AE3,0x20C7,0x314C,0x35B2,0x3E55,0x312D,0x45CF
	.hword 0x5AB3,0x6B78,0x7FFF,0x108D,0x18F6,0x193D,0x25FE,0x1895

@}}BLOCK(hitbox_bn_gfx)
