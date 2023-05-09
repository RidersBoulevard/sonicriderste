.include "macros.inc"

.section .dtors, "wa"  # 0x800E2F40 - 0x800E2F60

.balign 8

.global __dtors_rom_start__
__dtors_rom_start__:

.global lbl_800E2F40
lbl_800E2F40:
	.4byte 0x00000000

.global __dtors_rom_end__
__dtors_rom_end__: