#include "macros.S"

.section .sdata, "wa"  # 0x8021B8C0 - 0x8021BB40

.balign 8

.global __sdata_rom_start__
__sdata_rom_start__:

.global InGamePlayerCount
InGamePlayerCount:
	.4byte 0x00000001
.global gu32UserNum
gu32UserNum:
	.4byte 0x00000001
.global gu32ViewportNum
gu32ViewportNum:
	.4byte 0x00000001
.global gu32DefViewportNum
gu32DefViewportNum:
	.4byte 0x00000001
.global lbl_8021B8D0
lbl_8021B8D0:
	.4byte 0x02000000
.global gu32Game_UnPauseFlag
gu32Game_UnPauseFlag:
	.4byte 0x00000001
.global gf32MapMotionEndFrame
gf32MapMotionEndFrame:
	.4byte 0x42700000
.global lbl_8021B8DC
lbl_8021B8DC:
	.4byte 0x00000001
.global geMessageLanguage
geMessageLanguage:
	.4byte 0x00000001
.global GameLanguage
GameLanguage:
	.4byte 0x00000001
.global gu32_SystemHz
gu32_SystemHz:
	.4byte 0x0000003c
.global gf32_SystemHzAdd1
gf32_SystemHzAdd1:
	.4byte 0x3f800000
.global gf32_SystemHzSub1
gf32_SystemHzSub1:
	.4byte 0x3f800000
.global lbl_8021B8F4
lbl_8021B8F4:
	.4byte 0x4a454647
	.4byte 0x53490000
.global lbl_8021B8FC
lbl_8021B8FC:
	.4byte 0x536f6e69
	.4byte 0x63522e73
	.4byte 0x74720000
.global lbl_8021B908
lbl_8021B908:
	.4byte 0x5f4d6169
	.4byte 0x6e2e7265
	.4byte 0x6c000000
	.4byte 0x00000000
.global __OSVersion
__OSVersion:
	.4byte lbl_80117C98
.global lbl_8021B91C
lbl_8021B91C:
	.4byte 0x25303878
	.4byte 0x0a000000
.global lbl_8021B924
lbl_8021B924:
	.4byte 0x25730a00
.global __OSCurrHeap
__OSCurrHeap:
	.4byte 0xffffffff
	.4byte 0x00000000
.global __OSArenaLo
__OSArenaLo:
	.4byte 0xffffffff
	.4byte 0x00000000
.global __OSFpscrEnableBits
__OSFpscrEnableBits:
	.4byte 0x000000f8
.global lbl_8021B93C
lbl_8021B93C:
	.4byte 0x0a000000
.global lbl_8021B940
lbl_8021B940:
	.4byte 0x25640000
	.4byte 0x00000000
.global FontEncode
FontEncode:
	.4byte 0xffff0000
	.4byte 0x00000000
.global SwitchThreadCallback
SwitchThreadCallback:
	.4byte DefaultSwitchThreadCallback
.global lbl_8021B954
lbl_8021B954:
	.4byte 0x00000000
.global DVD_FirstRead
DVD_FirstRead:
	.4byte 0x00000001
	.4byte 0x00000000
.global lbl_8021B960
lbl_8021B960:
	.4byte 0x64766466
	.4byte 0x732e6300
.global __DVDVersion
__DVDVersion:
	.4byte lbl_80119F74 + 0x34
.global lbl_8021B96C
lbl_8021B96C:
	.4byte 0x00000001
.global lbl_8021B970
lbl_8021B970:
	.4byte defaultOptionalCommandChecker
.global lbl_8021B974
lbl_8021B974:
	.4byte 0x6476642e
	.4byte 0x63000000
.global lbl_8021B97C
lbl_8021B97C:
	.4byte 0xffffffff
.global lbl_8021B980
lbl_8021B980:
	.4byte 0x0a000000
.global lbl_8021B984
lbl_8021B984:
	.4byte 0x4f464600
.global lbl_8021B988
lbl_8021B988:
	.4byte 0x4f4e0000
	.4byte 0x00000000
.global lbl_8021B990
lbl_8021B990:
	.4byte lbl_8011A1E8
.global lbl_8021B994
lbl_8021B994:
	.4byte 0x76692e63
	.4byte 0x00000000
	.4byte 0x00000000
.global lbl_8021B9A0
lbl_8021B9A0:
	.4byte lbl_8011A5AC + 0x24
.global lbl_8021B9A4
lbl_8021B9A4:
	.4byte 0x00000020
.global lbl_8021B9A8
lbl_8021B9A8:
	.4byte 0xf0000000
.global lbl_8021B9AC
lbl_8021B9AC:
	.4byte 0x00000300
.global lbl_8021B9B0
lbl_8021B9B0:
	.4byte 0x00000005
.global lbl_8021B9B4
lbl_8021B9B4:
	.4byte SPEC2_MakeStatus
.global lbl_8021B9B8
lbl_8021B9B8:
	.4byte 0x41000000
.global lbl_8021B9BC
lbl_8021B9BC:
	.4byte 0x42000000
.global lbl_8021B9C0
lbl_8021B9C0:
	.4byte lbl_8011A618 + 0x10
	.skip 0x4
.global lbl_8021B9C8
lbl_8021B9C8:
	.4byte lbl_8011A618 + 0x58
	.skip 0x4
.global lbl_8021B9D0
lbl_8021B9D0:
	.4byte lbl_8011A618 + 0xA0
	.skip 0x4
.global lbl_8021B9D8
lbl_8021B9D8:
	.4byte lbl_8011A618 + 0xE8
	.skip 0x4
.global lbl_8021B9E0
lbl_8021B9E0:
	.4byte 0x1f000000
	.4byte 0x00000000
.global lbl_8021B9E8
lbl_8021B9E8:
	.4byte __AXFXAllocFunction
.global lbl_8021B9EC
lbl_8021B9EC:
	.4byte __AXFXFreeFunction
.global lbl_8021B9F0
lbl_8021B9F0:
	.4byte lbl_801208D0
	.skip 0x4
.global lbl_8021B9F8
lbl_8021B9F8:
	.4byte lbl_80120950 + 0x140
	.skip 0x4
.global lbl_8021BA00
lbl_8021BA00:
	.4byte 0x00000001
	.4byte 0x00000000
.global lbl_8021BA08
lbl_8021BA08:
	.2byte 0xffff
.global lbl_8021BA0A
lbl_8021BA0A:
	.2byte 0x1c00
	.2byte 0x0000
	.2byte 0x0000
.global lbl_8021BA10
lbl_8021BA10:
	.4byte lbl_80120CA0
	.skip 0x4
.global lbl_8021BA18
lbl_8021BA18:
	.4byte 0x00040102
.global lbl_8021BA1C
lbl_8021BA1C:
	.4byte 0x00080102
.global lbl_8021BA20
lbl_8021BA20:
	.4byte 0x000c0102
	.4byte 0x00000000
.global lbl_8021BA28
lbl_8021BA28:
	.4byte 0x80818283
	.4byte 0xa0a1a2a3
.global lbl_8021BA30
lbl_8021BA30:
	.4byte 0x84858687
	.4byte 0xa4a5a6a7
.global lbl_8021BA38
lbl_8021BA38:
	.4byte 0x88898a8b
	.4byte 0xa8a9aaab
.global lbl_8021BA40
lbl_8021BA40:
	.4byte 0x8c8d8e8f
	.4byte 0xacadaeaf
.global lbl_8021BA48
lbl_8021BA48:
	.4byte 0x90919293
	.4byte 0xb0b1b2b3
.global lbl_8021BA50
lbl_8021BA50:
	.4byte 0x94959697
	.4byte 0xb4b5b6b7
.global lbl_8021BA58
lbl_8021BA58:
	.4byte 0x98999a9b
	.4byte 0xb8b9babb
.global lbl_8021BA60
lbl_8021BA60:
	.4byte 0x00040105
	.4byte 0x02060000
.global lbl_8021BA68
lbl_8021BA68:
	.4byte 0x00020400
	.4byte 0x01030500
.global lbl_8021BA70
lbl_8021BA70:
	.4byte 0x00000000
	.4byte 0x00000000
.global lbl_8021BA78
lbl_8021BA78:
	.4byte 0x00000001
	.4byte 0x00000000
.global lbl_8021BA80
lbl_8021BA80:
	.4byte 0x7fffffff
.global lbl_8021BA84
lbl_8021BA84:
	.4byte 0x7f800000
.global lbl_8021BA88
lbl_8021BA88:
	.4byte 0x7fefffff
	.4byte 0xffffffff
.global lbl_8021BA90
lbl_8021BA90:
	.4byte 0x7ff00000
	.4byte 0x00000000
.global lbl_8021BA98
lbl_8021BA98:
	.4byte 0x00100000
	.4byte 0x00000000
.global lbl_8021BAA0
lbl_8021BAA0:
	.4byte 0x7fefffff
	.4byte 0xffffffff
.global lbl_8021BAA8
lbl_8021BAA8:
	.4byte 0x7f7fffff
.global lbl_8021BAAC
lbl_8021BAAC:
	.4byte 0x34000000
.global lbl_8021BAB0
lbl_8021BAB0:
	.4byte lbl_80121E64 + 0x44
	.skip 0x4
.global lbl_8021BAB8
lbl_8021BAB8:
	.4byte lbl_80121FB8
	.skip 0x4
.global lbl_8021BAC0
lbl_8021BAC0:
	.4byte 0x00000000
	.4byte 0x3f800000
.global lbl_8021BAC8
lbl_8021BAC8:
	.4byte 0x80000000
	.4byte 0x00000000
.global lbl_8021BAD0
lbl_8021BAD0:
	.4byte 0x0000001e
.global lbl_8021BAD4
lbl_8021BAD4:
	.4byte 0x00000008
.global lbl_8021BAD8
lbl_8021BAD8:
	.4byte 0x3f800000
.global lbl_8021BADC
lbl_8021BADC:
	.4byte lbl_8021CB8C + 0x4
.global lbl_8021BAE0
lbl_8021BAE0:
	.4byte 0xffffffff
.global lbl_8021BAE4
lbl_8021BAE4:
	.4byte 0x3f800000
.global lbl_8021BAE8
lbl_8021BAE8:
	.4byte 0x461c4000
.global lbl_8021BAEC
lbl_8021BAEC:
	.4byte 0x00000002
.global lbl_8021BAF0
lbl_8021BAF0:
	.4byte 0x3f800000
	.4byte 0x00000000
.global lbl_8021BAF8
lbl_8021BAF8:
	.4byte 0x3f800000
	.4byte 0x00000000
.global lbl_8021BB00
lbl_8021BB00:
	.4byte 0x3f800000
	.4byte 0x00000000
.global lbl_8021BB08
lbl_8021BB08:
	.4byte 0x3f800000
	.4byte 0x00000000
.global lbl_8021BB10
lbl_8021BB10:
	.4byte 0x3f800000
	.4byte 0x00000000
.global lbl_8021BB18
lbl_8021BB18:
	.4byte 0x00000001
	.4byte 0x437f0000
.global lbl_8021BB20
lbl_8021BB20:
	.4byte 0x00000000
	.4byte 0x00000000
.global lbl_8021BB28
lbl_8021BB28:
	.4byte 0x00000002
.global lbl_8021BB2C
lbl_8021BB2C:
	.4byte 0x00000002
.global lbl_8021BB30
lbl_8021BB30:
	.4byte 0x00000001
	.4byte 0x00000000
.global lbl_8021BB38
lbl_8021BB38:
	.4byte 0xffffffff

.global __sdata_rom_end__
__sdata_rom_end__: