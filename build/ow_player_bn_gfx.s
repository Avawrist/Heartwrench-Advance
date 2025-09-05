
@{{BLOCK(ow_player_bn_gfx)

@=======================================================================
@
@	ow_player_bn_gfx, 16x16@4, 
@	+ palette 16 entries, not compressed
@	+ 4 tiles not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2025-09-04, 14:34:14
@	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global ow_player_bn_gfxTiles		@ 128 unsigned chars
	.hidden ow_player_bn_gfxTiles
ow_player_bn_gfxTiles:
	.word 0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF
	.word 0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF
	.word 0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF
	.word 0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF

	.section .rodata
	.align	2
	.global ow_player_bn_gfxPal		@ 32 unsigned chars
	.hidden ow_player_bn_gfxPal
ow_player_bn_gfxPal:
	.hword 0x1485,0x0000,0x6B78,0x7FFF,0x28C6,0x3929,0x5989,0x7269
	.hword 0x7F51,0x1D13,0x1DB9,0x265C,0x2AFF,0x2C70,0x2898,0x3D3E

@}}BLOCK(ow_player_bn_gfx)
