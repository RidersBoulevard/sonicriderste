#include "macros.S"

.section .ctors, "wa"  # 0x800E2F20 - 0x800E2F40

.balign 8

.global __ctors_rom_start__
__ctors_rom_start__:

.global lbl_800E2F20
lbl_800E2F20:
	.4byte 0x00000000

.global __ctors_rom_end__
__ctors_rom_end__: