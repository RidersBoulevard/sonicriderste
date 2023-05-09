.include "macros.inc"

.section .sbss2, "", @nobits # 0x80125E00 - 0x8021CFA4

.balign 8

.global __sbss2_rom_start__
__sbss2_rom_start__:

.global lbl_8021CFA0
lbl_8021CFA0:
	.skip 0x4

.global __sbss2_rom_end__
__sbss2_rom_end__: